#include <jni.h>
#include <android_native_app_glue.h>
#include <GLES2/gl2.h>

extern int update_game(float touch_x);
extern float get_player_x();
extern float get_enemy_x();
extern float get_enemy_y();

float last_touch_x = 0.5f;

static void draw_frame(struct android_app* app) {
    int32_t w = ANativeWindow_getWidth(app->window);
    int32_t h = ANativeWindow_getHeight(app->window);

    update_game(last_touch_x);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_SCISSOR_TEST);

    // Gambar Player (Biru) di bawah
    glScissor((int)(get_player_x() * w) - 50, (int)(0.1 * h), 100, 100);
    glClearColor(0.0f, 0.5f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Gambar Musuh (Merah) jatuh
    glScissor((int)(get_enemy_x() * w) - 50, (int)(get_enemy_y() * h), 80, 80);
    glClearColor(1.0f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glDisable(GL_SCISSOR_TEST);
}

void android_main(struct android_app* state) {
    state->onInputEvent = NULL; // Nanti kita tambahkan handler sentuhan
    while (1) {
        int id, events; struct android_poll_source* source;
        while ((id = ALooper_pollOnce(0, NULL, &events, (void**)&source)) >= 0) {
            if (source != NULL) source->process(state, source);
            if (state->destroyRequested != 0) return;
        }
        if (state->window != NULL) {
            // Setup OpenGL sederhana (省略) dan panggil draw_frame
            draw_frame(state);
            // eglSwapBuffers...
        }
    }
}
