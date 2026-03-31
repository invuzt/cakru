static mut PLAYER_X: f32 = 0.5;
static mut ENEMY_X: f32 = 0.5;
static mut ENEMY_Y: f32 = 1.2;
static mut ENEMY_SPEED: f32 = 0.015;
static mut SCORE: i32 = 0;
static mut IS_GAMEOVER: bool = false;

#[no_mangle]
pub extern "C" fn update_game(touch_x: f32) -> i32 {
    unsafe {
        if IS_GAMEOVER {
            // Jika Game Over, sentuhan di area tengah akan Reset Game
            if touch_x > 0.4 && touch_x < 0.6 {
                IS_GAMEOVER = false;
                SCORE = 0;
                ENEMY_Y = 1.2;
                ENEMY_SPEED = 0.015;
            }
            return -1; // Sinyal ke C bahwa ini Game Over
        }

        // 1. Update Posisi Player (mengikuti jari)
        if touch_x >= 0.0 { PLAYER_X = touch_x; }

        // 2. Musuh Jatuh
        ENEMY_Y -= ENEMY_SPEED;

        // 3. Logika Menang (Point/Loop)
        if ENEMY_Y < -0.1 {
            ENEMY_Y = 1.2;
            // Acak posisi musuh berdasarkan sisa bagi (modulus)
            ENEMY_X = ((SCORE as f32 * 0.7) % 0.8) + 0.1; 
            SCORE += 1;
            
            // Leveling: Setiap 5 poin, kecepatan bertambah!
            if SCORE % 5 == 0 { ENEMY_SPEED += 0.005; }
        }

        // 4. Logika Kalah (Collision Detection)
        let dx = (PLAYER_X - ENEMY_X).abs();
        let dy = (0.15 - ENEMY_Y).abs(); // 0.15 adalah posisi Y Player
        
        if dx < 0.12 && dy < 0.12 {
            IS_GAMEOVER = true;
        }
        
        SCORE
    }
}

#[no_mangle] pub extern "C" fn get_player_x() -> f32 { unsafe { PLAYER_X } }
#[no_mangle] pub extern "C" fn get_enemy_x() -> f32 { unsafe { ENEMY_X } }
#[no_mangle] pub extern "C" fn get_enemy_y() -> f32 { unsafe { ENEMY_Y } }
#[no_mangle] pub extern "C" fn check_game_over() -> bool { unsafe { IS_GAMEOVER } }
