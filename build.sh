#!/bin/bash
set -e # Berhenti jika ada error

# --- KONFIGURASI (Sesuaikan path jika perlu) ---
SDK_PATH=$ANDROID_HOME
NDK_PATH=$ANDROID_NDK_HOME
BUILD_TOOLS=$SDK_PATH/build-tools/34.0.0
PLATFORM_JAR=$SDK_PATH/platforms/android-34/android.jar
KEYSTORE="my-release-key.jks"
PACKAGE_PATH="com/cakru/dodge"

echo "🚀 Memulai proses build Odfiz/Zamera..."

# 1. Bersihkan folder lama
rm -rf obj libs output classes.dex
mkdir -p obj/java output lib/arm64-v8a

# 2. Build Rust (Otak)
echo "🦀 Building Rust Logic..."
cd rust_logic && cargo build --target aarch64-linux-android --release && cd ..
cp rust_logic/target/aarch64-linux-android/release/libcakru_core.a jni/

# 3. Build NDK (Jembatan C ke Rust)
echo "🛠️ Running NDK-Build..."
$NDK_PATH/ndk-build NDK_PROJECT_PATH=. APP_BUILD_SCRIPT=jni/Android.mk NDK_APPLICATION_MK=jni/Application.mk

# 4. Kompilasi Java (Wajah)
echo "☕ Compiling Java to DEX..."
javac -d obj/java -bootclasspath $PLATFORM_JAR src/$PACKAGE_PATH/MainActivity.java
$BUILD_TOOLS/d8 --release --lib $PLATFORM_JAR --output . obj/java/$PACKAGE_PATH/*.class

# 5. Packaging (Bungkus APK)
echo "📦 Packaging APK..."
$BUILD_TOOLS/aapt package -f -M AndroidManifest.xml -S res -I $PLATFORM_JAR -F output/unsigned.apk
zip -uj output/unsigned.apk classes.dex
cp libs/arm64-v8a/libhello.so lib/arm64-v8a/
zip -ur output/unsigned.apk lib/arm64-v8a/libhello.so

# 6. Signing (Izin Jalan)
echo "✍️ Signing APK..."
$BUILD_TOOLS/zipalign -f 4 output/unsigned.apk output/aligned.apk
$BUILD_TOOLS/apksigner sign --ks $KEYSTORE --out output/final.apk output/aligned.apk

echo "✅ SELESAI! APK siap di-install: output/final.apk"

