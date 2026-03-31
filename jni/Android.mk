LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := rust_logic
# Kita asumsikan file ada di folder jni/ saat build
LOCAL_SRC_FILES := librust_logic.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := hello
LOCAL_SRC_FILES := hello.c sound.c
LOCAL_LDLIBS := -llog -landroid -lGLESv2 -lEGL -lOpenSLES
LOCAL_STATIC_LIBRARIES := android_native_app_glue rust_logic
include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)
