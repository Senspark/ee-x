LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := ee_core_static

LOCAL_MODULE_FILENAME := lib_ee_core

PROJ_DIR = $(LOCAL_PATH)/../../../..

LOCAL_SRC_FILES := ${shell find $(PROJ_DIR)/src/ee -name "*.cpp" -print}
LOCAL_SRC_FILES += ${shell find $(PROJ_DIR)/proj.android_studio/cpp/ee -name "*.cpp" -print}
                   
LOCAL_C_INCLUDES := $(PROJ_DIR)/src
LOCAL_C_INCLUDES += $(PROJ_DIR)/proj.android_studio/cpp

LOCAL_EXPORT_C_INCLUDES := $(PROJ_DIR)/src
LOCAL_EXPORT_C_INCLUDES += $(PROJ_DIR)/proj.android_studio/cpp

include $(BUILD_STATIC_LIBRARY)
