#include <jni.h>
#include <string.h>

// Deklarasi fungsi dari Rust (rust_logic/src/lib.rs)
extern char* rust_engine(const char* input);

JNIEXPORT jstring JNICALL
Java_com_cakru_dodge_MainActivity_prosesDiRust(JNIEnv* env, jobject thiz, jstring input) {
    // 1. Ambil string dari Java
    const char *nativeString = (*env)->GetStringUTFChars(env, input, 0);

    // 2. Oper ke "Otak" Rust
    char* hasilRust = rust_engine(nativeString);

    // 3. Bebaskan memori input Java
    (*env)->ReleaseStringUTFChars(env, input, nativeString);

    // 4. Kirim balik hasil Rust ke Java
    jstring result = (*env)->NewStringUTF(env, hasilRust);
    
    return result;
}
