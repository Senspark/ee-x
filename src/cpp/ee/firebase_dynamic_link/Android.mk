LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := ee_x_firebase_dynamic_link
LOCAL_CPP_FEATURES := exceptions
LOCAL_CPPFLAGS := -std=c++2a
LOCAL_SRC_FILES := ${shell find $(LOCAL_PATH) -print}
LOCAL_STATIC_LIBRARIES := ee_x_firebase_core firebase_dynamic_links

include $(BUILD_STATIC_LIBRARY)