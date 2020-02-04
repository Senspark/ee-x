LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

EE_X_ROOT_DIR = $(LOCAL_PATH)/../../../
COCOS2D_ROOT_DIR = $(LOCAL_PATH)/../../../../../cocos2d

COCOS2D_JAVASCRIPT = $(filter %-DCOCOS2D_JAVASCRIPT=1,$(APP_CPPFLAGS))

LOCAL_MODULE := cocos2dx_store_static
LOCAL_MODULE_FILENAME := libcocos2dxstore

LOCAL_SRC_FILES := ${shell find $(LOCAL_PATH)/Soomla -name "*.cpp" -print}

LOCAL_C_INCLUDES := $(LOCAL_PATH)
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)

LOCAL_STATIC_LIBRARIES := cocos2dx_soomla_common_static

include $(BUILD_STATIC_LIBRARY)