#include <jni.h>
#include <android_native_app_glue.h>
#include <android/log.h>

#define LOG_TAG "CAKRU_NATIVE"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

// Import fungsi dari Rust
extern void cakru_process_image(unsigned char* buffer, int width, int height);

void process_photo_buffer(unsigned char* raw_pixels, int w, int h) {
    LOGI("Memulai Watermarking pada resolusi: %dx%d", w, h);
    
    // Panggil mesin utama di Rust
    cakru_process_image(raw_pixels, w, h);
    
    LOGI("Watermarking selesai!");
}

// ... sisanya adalah boiler plate android_main yang sudah kita buat sebelumnya
