#include <jni.h>
#include <android_native_app_glue.h>
#include <GLES2/gl2.h>
#include <EGL/egl.h>

extern void set_sys_data(int c, int r, int b, int t);
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

// Fungsi simulasi data dinamis tanpa baca file /proc
void update_system_logic() {
    static int tick = 0;
    tick++;
    // Kita titipkan data ke Rust: CPU(berdenyut), RAM(60), BAT(80), TMP(40)
    set_sys_data((tick % 100), 65, 85, 42);
}

void draw_bar(float y, float val, float r, float g, float b, int win_w, int win_h) {
    // BG
    glScissor((int)(0.1f * win_w), (int)(y * win_h), (int)(0.8f * win_w), (int)(0.06f * win_h));
    glClearColor(0.15f, 0.15f, 0.15f, 1.0f); glClear(GL_COLOR_BUFFER_BIT);
    // FILL
    if(val > 0.01f) {
        glScissor((int)(0.1f * win_w), (int)(y * win_h), (int)(val * 0.8f * win_w), (int)(0.06f * win_h));
        glClearColor(r, g, b, 1.0f); glClear(GL_COLOR_BUFFER_BIT);
    }
}

static void draw_frame(struct engine* engine) {
    if (engine->display == EGL_NO_DISPLAY) return;
    update_system_logic();
    
    glClearColor(0.05f, 0.05f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_SCISSOR_TEST);

    draw_bar(0.80f, get_cpu_usage(), 1.0f, 0.2f, 0.2f, engine->width, engine->height); // CPU
    draw_bar(0.70f, get_ram_usage(), 0.2f, 0.5f, 1.0f, engine->width, engine->height); // RAM
    draw_bar(0.60f, get_bat_level(), 0.2f, 1.0f, 0.4f, engine->width, engine->height); // BAT
    draw_bar(0.50f, get_temp_level(), 1.0f, 0.7f, 0.1f, engine->width, engine->height); // TMP

    glDisable(GL_SCISSOR_TEST);
    eglSwapBuffers(engine->display, engine->surface);
}

static void handle_cmd(struct android_app* app, int32_t cmd) {
    struct engine* engine = (struct engine*)app->userData;
    if (cmd == APP_CMD_INIT_WINDOW && app->window != NULL) {
        engine->display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        eglInitialize(engine->display, 0, 0);
        EGLConfig config; EGLint n;
        eglChooseConfig(engine->display, (EGLint[]){EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT, EGL_NONE}, &config, 1, &n);
        engine->surface = eglCreateWindowSurface(engine->display, config, app->window, NULL);
        engine->context = eglCreateContext(engine->display, config, NULL, (EGLint[]){EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE});
        eglMakeCurrent(engine->display, engine->surface, engine->surface, engine->context);
        eglQuerySurface(engine->display, engine->surface, EGL_WIDTH, &engine->width);
        eglQuerySurface(engine->display, engine->surface, EGL_HEIGHT, &engine->height);
    }
}

void android_main(struct android_app* state) {
    struct engine engine = {0};
    state->userData = &engine;
    state->onAppCmd = handle_cmd;
    while (1) {
        int id, events; struct android_poll_source* source;
        while ((id = ALooper_pollOnce(0, NULL, &events, (void**)&source)) >= 0) {
            if (source != NULL) source->process(state, source);
            if (state->destroyRequested != 0) return;
        }
        if (engine.display != EGL_NO_DISPLAY) draw_frame(&engine);
    }
}
