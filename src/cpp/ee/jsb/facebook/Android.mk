LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := ee_x_jsb_facebook
LOCAL_CPP_FEATURES := exceptions
LOCAL_CPPFLAGS := -std=c++2a
LOCAL_SRC_FILES := ${shell find $(LOCAL_PATH) -name "*.cpp" -print}
LOCAL_STATIC_LIBRARIES := ee_x_jsb_core
LOCAL_STATIC_LIBRARIES += ee_x_facebook

include $(BUILD_STATIC_LIBRARY)