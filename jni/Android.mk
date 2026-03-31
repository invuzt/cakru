LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := cakru_core
LOCAL_SRC_FILES := libcakru_core.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := hello
LOCAL_SRC_FILES := hello.c sound.c
LOCAL_LDLIBS := -llog -landroid -lGLESv2 -lEGL -lOpenSLES
LOCAL_STATIC_LIBRARIES := android_native_app_glue cakru_core
include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)
