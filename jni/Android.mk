LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE    := hello
LOCAL_SRC_FILES := hello.c
LOCAL_LDLIBS    := -llog -landroid
LOCAL_STATIC_LIBRARIES := cakru_core_static

include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := cakru_core_static
LOCAL_SRC_FILES := libcakru_core.a
include $(PREBUILT_STATIC_LIBRARY)
