#include <jni.h>
#include <string.h>
#include <stdlib.h>

// Deklarasi fungsi dari Rust
extern char* rust_engine(const char* input);
extern void rust_free_string(char* s);

JNIEXPORT jstring JNICALL
Java_com_cakru_dodge_MainActivity_prosesDiRust(JNIEnv* env, jobject thiz, jstring input) {
    // 1. Ambil string dari Java
    const char *nativeString = (*env)->GetStringUTFChars(env, input, 0);
    if (nativeString == NULL) return (*env)->NewStringUTF(env, "Error Input");

    // 2. Oper ke Rust
    char* hasilRust = rust_engine(nativeString);

    // 3. Bebaskan memori input Java
    (*env)->ReleaseStringUTFChars(env, input, nativeString);

    // 4. Konversi hasil Rust ke Java String
    jstring result = (*env)->NewStringUTF(env, hasilRust);

    // 5. BERSIHKAN RAM: Panggil fungsi pembebas memori di Rust
    if (hasilRust != NULL) {
        rust_free_string(hasilRust);
    }

    return result;
}
