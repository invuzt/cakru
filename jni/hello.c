#include <jni.h>
#include <android_native_app_glue.h>
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <android/log.h>

extern float get_cpu_usage();
extern float get_ram_usage();
extern float get_bat_level();
extern float get_temp_level();

struct engine {
    EGLDisplay display;
    EGLSurface surface;
    EGLContext context;
    int32_t width;
    int32_t height;
};

static void draw_frame(struct engine* engine) {
    if (engine->display == EGL_NO_DISPLAY) return;

    // Bersihkan layar jadi Biru Tua
    glClearColor(0.05f, 0.05f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glEnable(GL_SCISSOR_TEST);
    
    // Gambar Bar Sederhana (Hanya satu dulu untuk tes)
    float val = get_cpu_usage();
    glScissor(100, 500, (int)(val * 500.0f), 100);
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glDisable(GL_SCISSOR_TEST);
    eglSwapBuffers(engine->display, engine->surface);
}

static void handle_cmd(struct android_app* app, int32_t cmd) {
    struct engine* engine = (struct engine*)app->userData;
    switch (cmd) {
        case APP_CMD_INIT_WINDOW:
            if (app->window != NULL) {
                engine->display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
                eglInitialize(engine->display, 0, 0);
                EGLConfig config; EGLint num;
                eglChooseConfig(engine->display, (EGLint[]){EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT, EGL_NONE}, &config, 1, &num);
                engine->surface = eglCreateWindowSurface(engine->display, config, app->window, NULL);
                engine->context = eglCreateContext(engine->display, config, NULL, (EGLint[]){EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE});
                eglMakeCurrent(engine->display, engine->surface, engine->surface, engine->context);
                eglQuerySurface(engine->display, engine->surface, EGL_WIDTH, &engine->width);
                eglQuerySurface(engine->display, engine->surface, EGL_HEIGHT, &engine->height);
            }
            break;
        case APP_CMD_TERM_WINDOW:
            engine->display = EGL_NO_DISPLAY;
            break;
    }
}

void android_main(struct android_app* state) {
    struct engine engine = {0};
    engine.display = EGL_NO_DISPLAY;
    state->userData = &engine;
    state->onAppCmd = handle_cmd;

    while (1) {
        int id, events;
        struct android_poll_source* source;
        while ((id = ALooper_pollOnce(0, NULL, &events, (void**)&source)) >= 0) {
            if (source != NULL) source->process(state, source);
            if (state->destroyRequested != 0) return;
        }
        if (engine.display != EGL_NO_DISPLAY) draw_frame(&engine);
    }
}
