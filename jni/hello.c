#include <jni.h>

// Beritahu C kalau fungsi ini ada di Rust (lib.rs)
extern char* get_hello_rust();

JNIEXPORT jstring JNICALL
Java_com_cakru_dodge_MainActivity_stringFromRust(JNIEnv* env, jobject thiz) {
    
    // Panggil Rust
    char* str = get_hello_rust();
    
    // Bungkus jadi String Java
    return (*env)->NewStringUTF(env, str);
}
