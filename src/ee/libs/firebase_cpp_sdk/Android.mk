LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := firebase_app_prebuilt
LOCAL_SRC_FILES := libs/$(TARGET_ARCH_ABI)/libapp.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
include $(PREBUILT_STATIC_LIBRARY)

LOCAL_MODULE := firebase_analytics_prebuilt
LOCAL_SRC_FILES := libs/$(TARGET_ARCH_ABI)/libanalytics.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
include $(PREBUILT_STATIC_LIBRARY)

LOCAL_MODULE := firebase_messaging_prebuilt
LOCAL_SRC_FILES := libs/$(TARGET_ARCH_ABI)/libmessaging.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
include $(PREBUILT_STATIC_LIBRARY)

LOCAL_MODULE := firebase_remote_config_prebuilt
LOCAL_SRC_FILES := libs/$(TARGET_ARCH_ABI)/libremote_config.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
include $(PREBUILT_STATIC_LIBRARY)

LOCAL_MODULE := firebase_storage_prebuilt
LOCAL_SRC_FILES := libs/$(TARGET_ARCH_ABI)/libstorage.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
include $(PREBUILT_STATIC_LIBRARY)