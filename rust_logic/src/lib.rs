static mut PLAYER_X: f32 = 0.5;
static mut ENEMY_X: f32 = 0.5;
static mut ENEMY_Y: f32 = 1.0;
static mut SCORE: i32 = 0;

#[no_mangle]
pub extern "C" fn update_game(touch_x: f32) -> i32 {
    unsafe {
        // 1. Gerakkan Player mengikuti jari
        if touch_x >= 0.0 { PLAYER_X = touch_x; }

        // 2. Gerakkan Musuh jatuh
        ENEMY_Y -= 0.02;

        // 3. Jika musuh sampai bawah, reset ke atas (Dapat Poin!)
        if ENEMY_Y < 0.0 {
            ENEMY_Y = 1.0;
            ENEMY_X = (PLAYER_X + 0.3) % 1.0; // Acak posisi sedikit
            SCORE += 1;
        }

        // 4. Cek Tabrakan (Game Over sederhana)
        let dx = (PLAYER_X - ENEMY_X).abs();
        let dy = (0.1 - ENEMY_Y).abs();
        if dx < 0.1 && dy < 0.1 {
            SCORE = 0; // Reset score jika kena
            ENEMY_Y = 1.0;
        }
        
        SCORE
    }
}

#[no_mangle] pub extern "C" fn get_player_x() -> f32 { unsafe { PLAYER_X } }
#[no_mangle] pub extern "C" fn get_enemy_x() -> f32 { unsafe { ENEMY_X } }
#[no_mangle] pub extern "C" fn get_enemy_y() -> f32 { unsafe { ENEMY_Y } }
