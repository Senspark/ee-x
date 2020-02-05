LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := ee_x_jsb_core_static
LOCAL_ARM_MODE := arm

LOCAL_CPP_FEATURES := exceptions
LOCAL_CPPFLAGS := -std=c++17

LOCAL_SRC_FILES := ${shell find $(LOCAL_PATH) -name "*.cpp" -print}

LOCAL_STATIC_LIBRARIES := ee_x_core_static
LOCAL_STATIC_LIBRARIES += cocos2dx_static

include $(BUILD_STATIC_LIBRARY)