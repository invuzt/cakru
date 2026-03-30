#include <jni.h>
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <android_native_app_glue.h>

extern void set_rust_touch(float x, float y);
extern float get_box_x();
extern float get_box_y();
extern float get_rust_color_r(float t);

struct engine {
    struct android_app* app;
    EGLDisplay display;
    EGLSurface surface;
    EGLContext context;
    float tick;
};

static int32_t handle_input(struct android_app* app, AInputEvent* event) {
    if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
        int32_t w = ANativeWindow_getWidth(app->window);
        int32_t h = ANativeWindow_getHeight(app->window);
        if (w > 0 && h > 0) {
            set_rust_touch(AMotionEvent_getX(event, 0) / (float)w, 
                           AMotionEvent_getY(event, 0) / (float)h);
        }
        return 1;
    }
    return 0;
}

static void draw_frame(struct engine* eng) {
    // Validasi super ketat agar tidak SIGSEGV
    if (eng->display == EGL_NO_DISPLAY || eng->surface == EGL_NO_SURFACE) return;

    int32_t w = ANativeWindow_getWidth(eng->app->window);
    int32_t h = ANativeWindow_getHeight(eng->app->window);
    if (w <= 0 || h <= 0) return;

    glClearColor(get_rust_color_r(eng->tick), 0.1f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_SCISSOR_TEST);
    int box_size = 200;
    int bx = (int)(get_box_x() * w) - (box_size / 2);
    int by = (int)((1.0f - get_box_y()) * h) - (box_size / 2);
    
    glScissor(bx, by, box_size, box_size);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);

    eglSwapBuffers(eng->display, eng->surface);
    eng->tick += 0.01f;
}

static void handle_cmd(struct android_app* app, int32_t cmd) {
    struct engine* eng = (struct engine*)app->userData;
    switch (cmd) {
        case APP_CMD_INIT_WINDOW:
            if (app->window != NULL) {
                eng->display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
                eglInitialize(eng->display, 0, 0);
                EGLConfig cfg; EGLint n;
                eglChooseConfig(eng->display, (EGLint[]){EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT, EGL_NONE}, &cfg, 1, &n);
                eng->surface = eglCreateWindowSurface(eng->display, cfg, app->window, NULL);
                eng->context = eglCreateContext(eng->display, cfg, NULL, (EGLint[]){EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE});
                eglMakeCurrent(eng->display, eng->surface, eng->surface, eng->context);
                glViewport(0, 0, ANativeWindow_getWidth(app->window), ANativeWindow_getHeight(app->window));
            }
            break;
        case APP_CMD_TERM_WINDOW:
            // Bersihkan agar tidak mencoba draw saat window hilang
            eng->display = EGL_NO_DISPLAY;
            eng->surface = EGL_NO_SURFACE;
            break;
    }
}

void android_main(struct android_app* state) {
    struct engine eng = {0};
    eng.display = EGL_NO_DISPLAY;
    eng.surface = EGL_NO_SURFACE;
    
    state->userData = &eng;
    state->onAppCmd = handle_cmd;
    state->onInputEvent = handle_input;
    eng.app = state;

    while (1) {
        int id, ev;
        struct android_poll_source* src;
        // Gunakan timeout -1 saat tidak ada display agar CPU istirahat
        while ((id = ALooper_pollOnce(eng.display != EGL_NO_DISPLAY ? 0 : -1, NULL, &ev, (void**)&src)) >= 0) {
            if (src != NULL) src->process(state, src);
            if (state->destroyRequested != 0) return;
        }
        if (eng.display != EGL_NO_DISPLAY && eng.surface != EGL_NO_SURFACE) {
            draw_frame(&eng);
        }
    }
}
