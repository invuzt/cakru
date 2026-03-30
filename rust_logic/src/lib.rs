use std::os::raw::c_float;

const COUNT: usize = 2000; // 2000 butir pasir!

static mut SAND_X: [f32; COUNT] = [0.0; COUNT];
static mut SAND_Y: [f32; COUNT] = [0.0; COUNT];
static mut VEL_Y: [f32; COUNT] = [0.0; COUNT];
static mut TOUCH_X: f32 = -1.0;
static mut TOUCH_Y: f32 = -1.0;
static mut INITIALIZED: bool = false;

fn init() {
    unsafe {
        for i in 0..COUNT {
            SAND_X[i] = (i as f32 * 0.123) % 1.0;
            SAND_Y[i] = 1.0 + (i as f32 * 0.01); // Mulai dari atas layar
            VEL_Y[i] = -0.005 - (i as f32 * 0.00001);
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
            // Gravitasi sederhana
            SAND_Y[i] += VEL_Y[i];

            // Interaksi dengan jari (Jari jadi Payung)
            if TOUCH_X > 0.0 {
                let dx = SAND_X[i] - TOUCH_X;
                let dy = SAND_Y[i] - TOUCH_Y;
                let dist = (dx*dx + dy*dy).sqrt();
                
                if dist < 0.15 && SAND_Y[i] > TOUCH_Y {
                    // Pasir terpental ke samping saat kena jari
                    SAND_Y[i] = TOUCH_Y + 0.15; 
                    SAND_X[i] += if dx > 0.0 { 0.02 } else { -0.02 };
                }
            }

            // Reset pasir jika sudah jatuh ke bawah
            if SAND_Y[i] < -0.05 {
                SAND_Y[i] = 1.05;
                SAND_X[i] = (i as f32 * 0.789) % 1.0;
            }
        }
    }
}

#[no_mangle] pub extern "C" fn get_box_x(i: i32) -> f32 { unsafe { SAND_X[i as usize] } }
#[no_mangle] pub extern "C" fn get_box_y(i: i32) -> f32 { unsafe { SAND_Y[i as usize] } }
