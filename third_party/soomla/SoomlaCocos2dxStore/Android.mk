LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

COCOS2D_JAVASCRIPT = $(filter %-DCOCOS2D_JAVASCRIPT=1,$(APP_CPPFLAGS))

LOCAL_MODULE := soomla_cocos2dx_store_static

LOCAL_SRC_FILES := ${shell find $(LOCAL_PATH)/Soomla -name "*.cpp" -print}

LOCAL_C_INCLUDES := $(LOCAL_PATH)
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)

LOCAL_STATIC_LIBRARIES := soomla_cocos2dx_core_static

include $(BUILD_STATIC_LIBRARY)