NDK_PATH ?= $(ANDROID_NDK_HOME)

build:
	# 1. Build Rust Logic
	cd rust_logic && cargo build --target aarch64-linux-android --release
	# 2. Copy hasil build Rust ke folder jni agar ditemukan NDK
	cp rust_logic/target/aarch64-linux-android/release/librust_logic.a jni/
	# 3. Jalankan NDK Build
	$(NDK_PATH)/ndk-build NDK_PROJECT_PATH=. APP_BUILD_SCRIPT=jni/Android.mk NDK_APPLICATION_MK=jni/Application.mk
