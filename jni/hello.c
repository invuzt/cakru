#include <jni.h>

// Deklarasi fungsi-fungsi yang ada di Rust (lib.rs)
extern char* get_hello_rust();
extern char* proses_teks_rust(const char* input);
extern void free_rust_string(char* s);

// FUNGSI 1: Ambil sapaan standar dari Rust
JNIEXPORT jstring JNICALL
Java_com_cakru_dodge_MainActivity_stringFromRust(JNIEnv* env, jobject thiz) {
    char* str = get_hello_rust();
    jstring result = (*env)->NewStringUTF(env, str);
    // Jika Rust kamu menggunakan CString::into_raw, sebaiknya di-free di sini
    // free_rust_string(str); 
    return result;
}

// FUNGSI 2: Kirim input dari EditText ke Rust dan ambil hasilnya
JNIEXPORT jstring JNICALL
Java_com_cakru_dodge_MainActivity_prosesDiRust(JNIEnv *env, jobject thiz, jstring input) {
    // 1. Konversi jstring (Java) ke char* (C)
    const char *native_input = (*env)->GetStringUTFChars(env, input, 0);

    // 2. Kirim ke Rust untuk diproses
    char *rust_result = proses_teks_rust(native_input);

    // 3. Lepaskan memori input Java setelah selesai digunakan
    (*env)->ReleaseStringUTFChars(env, input, native_input);

    // 4. Ubah hasil dari Rust menjadi jstring untuk dikembalikan ke UI Java
    jstring result = (*env)->NewStringUTF(env, rust_result);

    // 5. Bebaskan memori yang dialokasikan oleh Rust (agar RAM tidak penuh/leak)
    free_rust_string(rust_result);

    return result;
}

