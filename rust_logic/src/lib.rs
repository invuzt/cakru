use std::sync::atomic::{AtomicI32, Ordering};

static TICK: AtomicI32 = AtomicI32::new(0);

#[no_mangle]
pub extern "C" fn get_cpu_usage() -> f32 {
    let t = TICK.fetch_add(1, Ordering::SeqCst);
    ((t % 100) as f32 / 100.0) // Simulasi bar bergerak
}

#[no_mangle]
pub extern "C" fn get_ram_usage() -> f32 {
    0.65 // Simulasi RAM 65%
}

#[no_mangle]
pub extern "C" fn get_bat_level() -> f32 {
    0.85 // Simulasi Baterai 85%
}

#[no_mangle]
pub extern "C" fn get_temp_level() -> f32 {
    0.40 // Simulasi Suhu Adem
}

#[no_mangle] pub extern "C" fn load_stok() {}
#[no_mangle] pub extern "C" fn save_stok() {}
#[no_mangle] pub extern "C" fn get_stok() -> i32 { 0 }
#[no_mangle] pub extern "C" fn update_game(_x: f32) -> i32 { 0 }
#[no_mangle] pub extern "C" fn check_game_over() -> i32 { 0 }
#[no_mangle] pub extern "C" fn tambah_stok() {}
#[no_mangle] pub extern "C" fn kurangi_stok() {}
