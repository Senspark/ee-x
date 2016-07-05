LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CFLAGS := -DEE_PLATFORM_ANDROID

LOCAL_MODULE := ee_crashlytics_static

LOCAL_MODULE_FILENAME := lib_ee_crashlytics

LOCAL_SRC_FILES := ${shell find $(LOCAL_PATH)/../../../../../src -name "*.cpp" -print}
                   
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../../../src

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../../../../src

LOCAL_STATIC_LIBRARIES := ee_core_static

include $(BUILD_STATIC_LIBRARY)
