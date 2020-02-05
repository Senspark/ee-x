LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := ee_x_jsb_iron_source_static
LOCAL_CPP_FEATURES := exceptions
LOCAL_CPPFLAGS := -std=c++17
LOCAL_SRC_FILES := ${shell find $(LOCAL_PATH) -name "*.cpp" -print}
LOCAL_STATIC_LIBRARIES := ee_x_jsb_ads_static
LOCAL_STATIC_LIBRARIES += ee_x_iron_source_static

include $(BUILD_STATIC_LIBRARY)