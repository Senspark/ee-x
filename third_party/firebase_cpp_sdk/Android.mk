LOCAL_PATH := $(call my-dir)
FIREBASE_LIBS_DIR := $(LOCAL_PATH)/libs/android

include $(CLEAR_VARS)

LOCAL_MODULE := ee_x_firebase_app_prebuilt
LOCAL_SRC_FILES := $(FIREBASE_LIBS_DIR)/$(TARGET_ARCH_ABI)/libfirebase_app.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
include $(PREBUILT_STATIC_LIBRARY)

LOCAL_MODULE := ee_x_firebase_analytics_prebuilt
LOCAL_SRC_FILES := $(FIREBASE_LIBS_DIR)/$(TARGET_ARCH_ABI)/libfirebase_analytics.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
include $(PREBUILT_STATIC_LIBRARY)

LOCAL_MODULE := ee_x_firebase_dynamic_link_prebuilt
LOCAL_SRC_FILES := $(FIREBASE_LIBS_DIR)/$(TARGET_ARCH_ABI)/libfirebase_dynamic_links.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
include $(PREBUILT_STATIC_LIBRARY)

LOCAL_MODULE := ee_x_firebase_messaging_prebuilt
LOCAL_SRC_FILES := $(FIREBASE_LIBS_DIR)/$(TARGET_ARCH_ABI)/libfirebase_messaging.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
include $(PREBUILT_STATIC_LIBRARY)

LOCAL_MODULE := ee_x_firebase_remote_config_prebuilt
LOCAL_SRC_FILES := $(FIREBASE_LIBS_DIR)/$(TARGET_ARCH_ABI)/libfirebase_remote_config.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
include $(PREBUILT_STATIC_LIBRARY)

LOCAL_MODULE := ee_x_firebase_storage_prebuilt
LOCAL_SRC_FILES := $(FIREBASE_LIBS_DIR)/$(TARGET_ARCH_ABI)/libfirebase_storage.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
include $(PREBUILT_STATIC_LIBRARY)