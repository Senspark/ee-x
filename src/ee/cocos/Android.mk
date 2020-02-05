LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := ee_x_cocos_static
LOCAL_CPP_FEATURES := exceptions
LOCAL_CPPFLAGS := -std=c++17
LOCAL_SRC_FILES := ${shell find $(LOCAL_PATH) -name "*.cpp" -print}

ifeq ($(filter $(modules-get-list), cc_static),)
	# cocos-creator
	LOCAL_STATIC_LIBRARIES := cocos2dx_static
else
	# cocos2d-x
	LOCAL_STATIC_LIBRARIES := cc_static
endif

EE_X_ROOT_DIR = $(LOCAL_PATH)/../../..
LOCAL_C_INCLUDES := $(EE_X_ROOT_DIR)/src
LOCAL_EXPORT_C_INCLUDES := $(EE_X_ROOT_DIR)/src

include $(BUILD_STATIC_LIBRARY)