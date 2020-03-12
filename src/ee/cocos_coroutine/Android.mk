LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := ee_x_cocos_coroutine
LOCAL_CPP_FEATURES := exceptions coroutine-ts
LOCAL_CPPFLAGS := -std=c++2a
LOCAL_SRC_FILES := ${shell find $(LOCAL_PATH) -name "*.cpp" -print}

ifeq ($(filter $(modules-get-list), cc_static),)
	# cocos-creator
	LOCAL_STATIC_LIBRARIES := cocos2dx_static
else
	# cocos2d-x
	LOCAL_STATIC_LIBRARIES := cc_static
endif

LOCAL_STATIC_LIBRARIES += ee_x_coroutine

include $(BUILD_STATIC_LIBRARY)