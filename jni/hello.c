#include <jni.h>
#include <android_native_app_glue.h>
#include <android/log.h>
#include <stdlib.h>

#define LOG_TAG "CAKRU_NATIVE"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

// Import fungsi dari Rust
extern void cakru_process_image(unsigned char* buffer, int width, int height);

// Fungsi pembantu untuk memproses foto
void process_photo_buffer(unsigned char* raw_pixels, int w, int h) {
    if (raw_pixels != NULL) {
        LOGI("Memulai Watermarking pada resolusi: %dx%d", w, h);
        cakru_process_image(raw_pixels, w, h);
        LOGI("Watermarking selesai!");
    }
}

// Handler perintah aplikasi (Lifecycle)
static void handle_cmd(struct android_app* app, int32_t cmd) {
    switch (cmd) {
        case APP_CMD_INIT_WINDOW:
            LOGI("Window Inisialisasi - Cakru Siap");
            break;
        case APP_CMD_TERM_WINDOW:
            LOGI("Window Ditutup");
            break;
    }
}

// Handler input (Sentuhan)
static int32_t handle_input(struct android_app* app, AInputEvent* event) {
    return 0; // Sementara kita tidak butuh input sentuhan untuk watermark
}

// INI ADALAH FUNGSI UTAMA YANG DICARI LINKER
void android_main(struct android_app* state) {
    LOGI("Aplikasi Cakru Dimulai via android_main");

    state->onAppCmd = handle_cmd;
    state->onInputEvent = handle_input;

    // Loop utama aplikasi
    while (1) {
        int id, events;
        struct android_poll_source* source;

        // Tunggu event (seperti inisialisasi window)
        while ((id = ALooper_pollOnce(-1, NULL, &events, (void**)&source)) >= 0) {
            if (source != NULL) {
                source->process(state, source);
            }

            // Jika aplikasi diminta berhenti
            if (state->destroyRequested != 0) {
                LOGI("Menutup Aplikasi Cakru...");
                return;
            }
        }
    }
}
