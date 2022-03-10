LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := soomla_store
LOCAL_CPP_FEATURES := exceptions
LOCAL_CPPFLAGS := -std=c++20
LOCAL_SRC_FILES := ${shell find $(LOCAL_PATH) -print}
LOCAL_STATIC_LIBRARIES := soomla_store_internal

include $(BUILD_STATIC_LIBRARY)