LOCAL_PATH := $(call my-dir)
FIREBASE_LIBS_DIR := $(LOCAL_PATH)/libs/android

include $(CLEAR_VARS)

LOCAL_MODULE := firebase_app
LOCAL_SRC_FILES := $(FIREBASE_LIBS_DIR)/$(TARGET_ARCH_ABI)/libfirebase_app.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
include $(PREBUILT_STATIC_LIBRARY)

LOCAL_MODULE := firebase_analytics
LOCAL_SRC_FILES := $(FIREBASE_LIBS_DIR)/$(TARGET_ARCH_ABI)/libfirebase_analytics.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
include $(PREBUILT_STATIC_LIBRARY)

LOCAL_MODULE := firebase_dynamic_links
LOCAL_SRC_FILES := $(FIREBASE_LIBS_DIR)/$(TARGET_ARCH_ABI)/libfirebase_dynamic_links.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
include $(PREBUILT_STATIC_LIBRARY)

LOCAL_MODULE := firebase_messaging
LOCAL_SRC_FILES := $(FIREBASE_LIBS_DIR)/$(TARGET_ARCH_ABI)/libfirebase_messaging.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
include $(PREBUILT_STATIC_LIBRARY)

LOCAL_MODULE := firebase_remote_config
LOCAL_SRC_FILES := $(FIREBASE_LIBS_DIR)/$(TARGET_ARCH_ABI)/libfirebase_remote_config.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
include $(PREBUILT_STATIC_LIBRARY)

LOCAL_MODULE := firebase_storage
LOCAL_SRC_FILES := $(FIREBASE_LIBS_DIR)/$(TARGET_ARCH_ABI)/libfirebase_storage.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
include $(PREBUILT_STATIC_LIBRARY)