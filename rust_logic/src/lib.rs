use std::os::raw::c_float;

const COUNT: usize = 100;
static mut BOXES_X: [f32; COUNT] = [0.5; COUNT];
static mut BOXES_Y: [f32; COUNT] = [0.5; COUNT];
static mut VEL_X: [f32; COUNT] = [0.0; COUNT];
static mut VEL_Y: [f32; COUNT] = [0.0; COUNT];
static mut TOUCH_X: f32 = -1.0;
static mut TOUCH_Y: f32 = -1.0;
static mut INITIALIZED: bool = false;

const GRAVITY: f32 = 0.0005;
const FRICTION: f32 = 0.98; // Biar tidak terlalu liar gerakannya

fn init() {
    unsafe {
        for i in 0..COUNT {
            BOXES_X[i] = (i as f32 / COUNT as f32);
            BOXES_Y[i] = 0.8; // Mulai dari atas
            VEL_X[i] = ((i as f32).sin() * 0.01);
            VEL_Y[i] = 0.0;
        }
        INITIALIZED = true;
    }
}

#[no_mangle]
pub extern "C" fn set_rust_touch(x: c_float, y: c_float) {
    unsafe { TOUCH_X = x; TOUCH_Y = y; }
}

#[no_mangle]
pub extern "C" fn update_physics() {
    unsafe {
        if !INITIALIZED { init(); }
        for i in 0..COUNT {
            // Terapkan Gravitasi (Y ditarik ke bawah/1.0)
            VEL_Y[i] += GRAVITY;
            
            // Update posisi
            BOXES_X[i] += VEL_X[i];
            BOXES_Y[i] += VEL_Y[i];

            // Gesekan udara
            VEL_X[i] *= FRICTION;
            VEL_Y[i] *= FRICTION;

            // Pantulan lantai & dinding
            if BOXES_X[i] < 0.0 { BOXES_X[i] = 0.0; VEL_X[i] *= -0.8; }
            if BOXES_X[i] > 1.0 { BOXES_X[i] = 1.0; VEL_X[i] *= -0.8; }
            if BOXES_Y[i] > 1.0 { BOXES_Y[i] = 1.0; VEL_Y[i] *= -0.5; } // Membal di bawah
            if BOXES_Y[i] < 0.0 { BOXES_Y[i] = 0.0; VEL_Y[i] *= -0.8; }

            // Interaksi jari (Magnet kuat)
            let dx = TOUCH_X - BOXES_X[i];
            let dy = TOUCH_Y - BOXES_Y[i];
            let dist = (dx*dx + dy*dy).sqrt();
            if dist < 0.4 && TOUCH_X > 0.0 {
                VEL_X[i] += dx * 0.005;
                VEL_Y[i] += dy * 0.005;
            }
        }
    }
}

#[no_mangle] pub extern "C" fn get_box_x(i: i32) -> f32 { unsafe { BOXES_X[i as usize] } }
#[no_mangle] pub extern "C" fn get_box_y(i: i32) -> f32 { unsafe { BOXES_Y[i as usize] } }
#[no_mangle] pub extern "C" fn get_rust_color_r(t: f32) -> f32 { (t * 0.5).sin().abs() * 0.1 }
