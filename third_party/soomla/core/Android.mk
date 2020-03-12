LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

COCOS2D_JAVASCRIPT = $(filter %-DCOCOS2D_JAVASCRIPT=1,$(APP_CPPFLAGS))

LOCAL_MODULE := soomla_core

LOCAL_SRC_FILES := ${shell find $(LOCAL_PATH)/src -name "*.cpp" -print}

LOCAL_C_INCLUDES := $(LOCAL_PATH)/src
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/src

ifeq ($(filter $(modules-get-list), cc_static),)
	# cocos-creator
	LOCAL_STATIC_LIBRARIES := cocos2dx_static
else
	# cocos2d-x
	LOCAL_STATIC_LIBRARIES := cc_static
endif

LOCAL_STATIC_LIBRARIES += jansson

include $(BUILD_STATIC_LIBRARY)