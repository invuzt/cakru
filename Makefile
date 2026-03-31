NDK_PATH ?= $(ANDROID_NDK_HOME)

build:
	# 1. Build Rust Logic
	cd rust_logic && cargo build --target aarch64-linux-android --release
	# 2. Copy hasil build (Sesuaikan dengan nama di Cargo.toml)
	cp rust_logic/target/aarch64-linux-android/release/libcakru_core.a jni/
	# 3. Jalankan NDK Build
	$(NDK_PATH)/ndk-build NDK_PROJECT_PATH=. APP_BUILD_SCRIPT=jni/Android.mk NDK_APPLICATION_MK=jni/Application.mk
