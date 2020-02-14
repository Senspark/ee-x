LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := ee_x_coroutine_static
LOCAL_CPP_FEATURES := exceptions coroutine-ts
LOCAL_CPPFLAGS := -std=c++2a
LOCAL_SRC_FILES := ${shell find $(LOCAL_PATH) -name "*.cpp" -print}

LOCAL_STATIC_LIBRARIES := ee_x_core_static

include $(BUILD_STATIC_LIBRARY)