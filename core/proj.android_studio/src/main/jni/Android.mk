LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := ee_core_static

LOCAL_MODULE_FILENAME := lib_ee_core

LOCAL_SRC_FILES := ${shell find $(LOCAL_PATH)/../../../../src -name "*.cpp" -print}
                   
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../../src
LOCAL_C_INCLUDES += ${shell find $(LOCAL_PATH)/../../../../libraries -type d -print}

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../../../src
LOCAL_EXPORT_C_INCLUDES += ${shell find $(LOCAL_PATH)/../../../../libraries -type d -print}

include $(BUILD_STATIC_LIBRARY)
