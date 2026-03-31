// ... (include dan struct sama seperti sebelumnya) ...

extern int update_game(float touch_x);
extern float get_player_x();
extern float get_enemy_x();
extern float get_enemy_y();
extern int check_game_over();

static void draw_frame(struct engine* engine) {
    if (engine->display == NULL) return;
    int32_t w, h;
    eglQuerySurface(engine->display, engine->surface, EGL_WIDTH, &w);
    eglQuerySurface(engine->display, engine->surface, EGL_HEIGHT, &h);

    update_game(engine->last_touch_x);

    // LOGIKA VISUAL KALAH
    if (check_game_over()) {
        glClearColor(0.8f, 0.0f, 0.0f, 1.0f); // Layar Merah tanda Kalah
    } else {
        glClearColor(0.1f, 0.1f, 0.15f, 1.0f); // Warna Normal
    }
    
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_SCISSOR_TEST);

    // Player (Biru)
    glScissor((int)(get_player_x() * w) - 60, (int)(0.15 * h), 120, 120);
    glClearColor(0.0f, 0.6f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Musuh (Kuning Emas agar kontras)
    glScissor((int)(get_enemy_x() * w) - 50, (int)(get_enemy_y() * h), 100, 100);
    glClearColor(1.0f, 0.8f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glDisable(GL_SCISSOR_TEST);
    eglSwapBuffers(engine->display, engine->surface);
}

// ... (android_main dan handler lainnya tetap sama) ...
