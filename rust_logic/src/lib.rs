use std::sync::atomic::{AtomicI32, Ordering};

// Pakai Atomic agar aman diakses dari thread UI Android
static STOK: AtomicI32 = AtomicI32::new(10); 

#[no_mangle]
pub extern "C" fn tambah_stok() {
    STOK.fetch_add(1, Ordering::SeqCst);
}

#[no_mangle]
pub extern "C" fn kurangi_stok() {
    let current = STOK.load(Ordering::SeqCst);
    if current > 0 {
        STOK.fetch_sub(1, Ordering::SeqCst);
    }
}

#[no_mangle]
pub extern "C" fn get_stok() -> i32 {
    STOK.load(Ordering::SeqCst)
}

// Stub agar linker tidak error karena Makefile lama mencari ini
#[no_mangle] pub extern "C" fn update_game(_x: f32) -> i32 { get_stok() }
#[no_mangle] pub extern "C" fn get_player_x() -> f32 { 0.0 }
#[no_mangle] pub extern "C" fn get_enemy_x() -> f32 { 0.0 }
#[no_mangle] pub extern "C" fn get_enemy_y() -> f32 { 0.0 }
#[no_mangle] pub extern "C" fn get_p_x(_i: i32) -> f32 { 0.0 }
#[no_mangle] pub extern "C" fn get_p_y(_i: i32) -> f32 { 0.0 }
#[no_mangle] pub extern "C" fn check_game_over() -> i32 { 0 }
