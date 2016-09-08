LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := ee_crashlytics_static

LOCAL_MODULE_FILENAME := lib_ee_crashlytics

PROJ_DIR = $(LOCAL_PATH)/../../../..

LOCAL_SRC_FILES := ${shell find $(PROJ_DIR)/src/ee -name "*.cpp" -print}
                   
LOCAL_C_INCLUDES := $(PROJ_DIR)/include

LOCAL_EXPORT_C_INCLUDES := $(PROJ_DIR)/include

LOCAL_STATIC_LIBRARIES := ee_core_static

include $(BUILD_STATIC_LIBRARY)
