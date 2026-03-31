# --- Konfigurasi Path ---
NDK_PATH ?= $(ANDROID_NDK_HOME)
SDK_PATH ?= $(ANDROID_HOME)
PLATFORM ?= 34
BUILD_TOOLS ?= $(SDK_PATH)/build-tools/34.0.0

# --- Nama Project ---
PACKAGE_PATH = com/cakru/dodge

build: clean
	# 1. Build Rust Logic
	cd rust_logic && cargo build --target aarch64-linux-android --release
	cp rust_logic/target/aarch64-linux-android/release/libcakru_core.a jni/

	# 2. Jalankan NDK Build (Hasilnya libhello.so)
	$(NDK_PATH)/ndk-build NDK_PROJECT_PATH=. APP_BUILD_SCRIPT=jni/Android.mk NDK_APPLICATION_MK=jni/Application.mk

	# 3. Kompilasi Java ke Class (PENTING!)
	mkdir -p obj/java
	javac -d obj/java \
		-bootclasspath $(SDK_PATH)/platforms/android-$(PLATFORM)/android.jar \
		src/$(PACKAGE_PATH)/MainActivity.java

	# 4. Convert Class ke DEX (Agar bisa dibaca Android)
	$(BUILD_TOOLS)/d8 --release \
		--lib $(SDK_PATH)/platforms/android-$(PLATFORM)/android.jar \
		--output . \
		obj/java/$(PACKAGE_PATH)/*.class

	# 5. Packaging (Gunakan alat build Mas biasanya, misal aapt atau manual zip)
	# Pastikan file 'classes.dex' ikut masuk ke dalam root APK.
	@echo "Build selesai. Pastikan classes.dex masuk ke dalam APK!"

clean:
	rm -rf obj/ libs/ bin/ gen/ classes.dex
	rm -f jni/libcakru_core.a

