LOCAL_PATH := $(call my-dir)
FIREBASE_LIBS_DIR := $(LOCAL_PATH)/libs/android

include $(CLEAR_VARS)

LOCAL_MODULE := ee_x_firebase_app_prebuilt
LOCAL_SRC_FILES := $(FIREBASE_LIBS_DIR)/$(TARGET_ARCH_ABI)/libapp.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
include $(PREBUILT_STATIC_LIBRARY)

LOCAL_MODULE := ee_x_firebase_analytics_prebuilt
LOCAL_SRC_FILES := $(FIREBASE_LIBS_DIR)/$(TARGET_ARCH_ABI)/libanalytics.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
include $(PREBUILT_STATIC_LIBRARY)

LOCAL_MODULE := ee_x_firebase_dynamic_link_prebuilt
LOCAL_SRC_FILES := $(FIREBASE_LIBS_DIR)/$(TARGET_ARCH_ABI)/libdynamic_links.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
include $(PREBUILT_STATIC_LIBRARY)

LOCAL_MODULE := ee_x_firebase_messaging_prebuilt
LOCAL_SRC_FILES := $(FIREBASE_LIBS_DIR)/$(TARGET_ARCH_ABI)/libmessaging.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
include $(PREBUILT_STATIC_LIBRARY)

LOCAL_MODULE := ee_x_firebase_remote_config_prebuilt
LOCAL_SRC_FILES := $(FIREBASE_LIBS_DIR)/$(TARGET_ARCH_ABI)/libremote_config.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
include $(PREBUILT_STATIC_LIBRARY)

LOCAL_MODULE := ee_x_firebase_storage_prebuilt
LOCAL_SRC_FILES := $(FIREBASE_LIBS_DIR)/$(TARGET_ARCH_ABI)/libstorage.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
include $(PREBUILT_STATIC_LIBRARY)