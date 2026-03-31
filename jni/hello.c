#include <jni.h>
#include <android_native_app_glue.h>
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <android/log.h>
#include "sound.h"

struct engine {
    struct android_app* app;
    EGLDisplay display;
    EGLSurface surface;
    EGLContext context;
    int animating;
};

void draw_box(float x, float y, float w_pct, float h_pct, float r, float g, float b, int win_w, int win_h) {
    glScissor((int)(x * win_w), (int)(y * win_h), (int)(w_pct * win_w), (int)(h_pct * win_h));
    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

static void draw_frame(struct engine* engine) {
    if (engine->display == NULL) return;
    int32_t w, h;
    eglQuerySurface(engine->display, engine->surface, EGL_WIDTH, &w);
    eglQuerySurface(engine->display, engine->surface, EGL_HEIGHT, &h);

    glClearColor(0.05f, 0.05f, 0.07f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_SCISSOR_TEST);

    // 1. Tombol Merah (Kiri Bawah) - KURANGI
    draw_box(0.05f, 0.05f, 0.42f, 0.15f, 0.8f, 0.2f, 0.2f, w, h);
    
    // 2. Tombol Hijau (Kanan Bawah) - TAMBAH
    draw_box(0.53f, 0.05f, 0.42f, 0.15f, 0.2f, 0.7f, 0.3f, w, h);

    // 3. Visualisasi Stok (Barisan Kotak di Tengah)
    int stok = get_stok();
    for(int i=0; i<stok; i++) {
        float row = i / 10;
        float col = i % 10;
        draw_box(0.1f + (col * 0.08f), 0.7f - (row * 0.06f), 0.06f, 0.04f, 1.0f, 1.0f, 1.0f, w, h);
    }

    glDisable(GL_SCISSOR_TEST);
    eglSwapBuffers(engine->display, engine->surface);
}

static int32_t handle_input(struct android_app* app, AInputEvent* event) {
    struct engine* engine = (struct engine*)app->userData;
    if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
        if (AMotionEvent_getAction(event) == AMOTION_EVENT_ACTION_DOWN) {
            float x = AMotionEvent_getX(event, 0) / ANativeWindow_getWidth(app->window);
            if (x < 0.5f) {
                kurangi_stok();
                play_crash_sound(); // Bunyi feedback klik
            } else {
                tambah_stok();
                // Nanti bisa tambah bunyi beda
            }
        }
        return 1;
    }
    return 0;
}

static int init_display(struct engine* engine) {
    const EGLint attribs[] = { EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT, EGL_BLUE_SIZE, 8, EGL_GREEN_SIZE, 8, EGL_RED_SIZE, 8, EGL_NONE };
    EGLConfig config; EGLint numConfigs, format;
    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    eglInitialize(display, 0, 0);
    eglChooseConfig(display, attribs, &config, 1, &numConfigs);
    eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);
    ANativeWindow_setBuffersGeometry(engine->app->window, 0, 0, format);
    engine->surface = eglCreateWindowSurface(display, config, engine->app->window, NULL);
    engine->context = eglCreateContext(display, config, NULL, (EGLint[]){EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE});
    eglMakeCurrent(display, engine->surface, engine->surface, engine->context);
    engine->display = display;
    return 0;
}

static void handle_cmd(struct android_app* app, int32_t cmd) {
    struct engine* engine = (struct engine*)app->userData;
    switch (cmd) {
        case APP_CMD_INIT_WINDOW:
            if (app->window != NULL) { init_display(engine); engine->animating = 1; }
            break;
        case APP_CMD_TERM_WINDOW:
            engine->animating = 0;
            break;
    }
}

void android_main(struct android_app* state) {
    struct engine engine = {0};
    engine.app = state;
    state->userData = &engine;
    state->onAppCmd = handle_cmd;
    state->onInputEvent = handle_input;
    init_sound();

    while (1) {
        int id, events; struct android_poll_source* source;
        while ((id = ALooper_pollOnce(engine.animating ? 0 : -1, NULL, &events, (void**)&source)) >= 0) {
            if (source != NULL) source->process(state, source);
            if (state->destroyRequested != 0) return;
        }
        if (engine.animating) draw_frame(&engine);
    }
}
