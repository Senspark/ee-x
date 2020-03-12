LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

COCOS2D_JAVASCRIPT = $(filter %-DCOCOS2D_JAVASCRIPT=1,$(APP_CPPFLAGS))

LOCAL_MODULE := soomla_store

LOCAL_SRC_FILES := ${shell find $(LOCAL_PATH)/src -name "*.cpp" -print}

LOCAL_C_INCLUDES := $(LOCAL_PATH)/src
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/src

LOCAL_STATIC_LIBRARIES := soomla_core

include $(BUILD_STATIC_LIBRARY)