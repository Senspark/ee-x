LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := ee_x_core
LOCAL_ARM_MODE := arm

LOCAL_CPP_FEATURES := exceptions
LOCAL_CPPFLAGS := -std=c++2a

LOCAL_SRC_FILES := ${shell find $(LOCAL_PATH) -name "*.cpp" -print}

EE_X_ROOT_DIR = $(LOCAL_PATH)/../../..

LOCAL_C_INCLUDES := $(EE_X_ROOT_DIR)/src
LOCAL_C_INCLUDES += $(EE_X_ROOT_DIR)/third_party/nlohmann/include

LOCAL_EXPORT_C_INCLUDES := $(EE_X_ROOT_DIR)/src
LOCAL_EXPORT_C_INCLUDES += $(EE_X_ROOT_DIR)/third_party/nlohmann/include

include $(BUILD_STATIC_LIBRARY)