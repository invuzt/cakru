#include <jni.h>
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <android_native_app_glue.h>

extern void set_rust_touch(float x, float y);
extern void update_physics();
extern float get_box_x(int i);
extern float get_box_y(int i);
extern float get_rust_color_r(float t);

struct engine {
    struct android_app* app;
    EGLDisplay display; EGLSurface surface; EGLContext context;
    float tick;
};

static int32_t handle_input(struct android_app* app, AInputEvent* event) {
    if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
        int32_t w = ANativeWindow_getWidth(app->window);
        int32_t h = ANativeWindow_getHeight(app->window);
        set_rust_touch(AMotionEvent_getX(event, 0) / (float)w, 
                       AMotionEvent_getY(event, 0) / (float)h);
        return 1;
    }
    // Jika jari dilepas, reset posisi sentuhan ke luar layar
    if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_KEY) {
        set_rust_touch(-1.0f, -1.0f);
    }
    return 0;
}

static void draw_frame(struct engine* eng) {
    if (eng->display == EGL_NO_DISPLAY) return;
    update_physics();

    int32_t w = ANativeWindow_getWidth(eng->app->window);
    int32_t h = ANativeWindow_getHeight(eng->app->window);

    glClearColor(get_rust_color_r(eng->tick), 0.05f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_SCISSOR_TEST);
    for(int i = 0; i < 100; i++) {
        int bx = (int)(get_box_x(i) * w) - 10;
        int by = (int)((1.0f - get_box_y(i)) * h) - 10;
        glScissor(bx, by, 20, 20);
        glClearColor(0.2f, 1.0f, 0.6f, 1.0f); // Hijau neon terang
        glClear(GL_COLOR_BUFFER_BIT);
    }
    glDisable(GL_SCISSOR_TEST);

    eglSwapBuffers(eng->display, eng->surface);
    eng->tick += 0.01f;
}

static void handle_cmd(struct android_app* app, int32_t cmd) {
    struct engine* eng = (struct engine*)app->userData;
    if (cmd == APP_CMD_INIT_WINDOW) {
        eng->display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        eglInitialize(eng->display, 0, 0);
        EGLConfig cfg; EGLint n;
        eglChooseConfig(eng->display, (EGLint[]){EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT, EGL_NONE}, &cfg, 1, &n);
        eng->surface = eglCreateWindowSurface(eng->display, cfg, app->window, NULL);
        eng->context = eglCreateContext(eng->display, cfg, NULL, (EGLint[]){EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE});
        eglMakeCurrent(eng->display, eng->surface, eng->surface, eng->context);
        glViewport(0, 0, ANativeWindow_getWidth(app->window), ANativeWindow_getHeight(app->window));
    } else if (cmd == APP_CMD_TERM_WINDOW) {
        eng->display = EGL_NO_DISPLAY;
    }
}

void android_main(struct android_app* state) {
    struct engine eng = {0};
    state->userData = &eng;
    state->onAppCmd = handle_cmd;
    state->onInputEvent = handle_input;
    while (1) {
        int id, ev; struct android_poll_source* src;
        while ((id = ALooper_pollOnce(eng.display != EGL_NO_DISPLAY ? 0 : -1, NULL, &ev, (void**)&src)) >= 0) {
            if (src != NULL) src->process(state, src);
            if (state->destroyRequested != 0) return;
        }
        if (eng.display != EGL_NO_DISPLAY) draw_frame(&eng);
    }
}
