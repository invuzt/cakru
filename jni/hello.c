#include <jni.h>

// Deklarasi fungsi yang ada di rust_logic/src/lib.rs
// Fungsi ini harus mengembalikan char* yang dialokasikan di Rust
extern char* get_hello_rust();

JNIEXPORT jstring JNICALL
Java_com_cakru_dodge_MainActivity_stringFromRust(JNIEnv* env, jobject thiz) {
    
    // 1. Ambil pointer string dari Rust
    char* str = get_hello_rust();

    // 2. Ubah ke jstring agar bisa dibaca oleh Java
    jstring result = (*env)->NewStringUTF(env, str);

    // Catatan: Jika Rust menggunakan CString::into_raw, 
    // idealnya kamu butuh fungsi khusus untuk membebaskan memori ini.
    // Tapi untuk string "Hello" sederhana, ini sudah cukup untuk testing.

    return result;
}

