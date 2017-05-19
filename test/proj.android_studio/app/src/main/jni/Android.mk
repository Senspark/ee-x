LOCAL_PATH := $(call my-dir)
EE_X_PROJECT_DIR = $(LOCAL_PATH)/../../../../../..

include $(CLEAR_VARS)

$(call import-add-path, $(EE_X_PROJECT_DIR))

LOCAL_MODULE := ee_x_test_android_shared

LOCAL_SRC_FILES := cpp/main.cpp
LOCAL_SRC_FILES += ${shell find $(EE_X_PROJECT_DIR)/test/Classes -name "*.cpp" -print}

LOCAL_C_INCLUDES := ${shell find $(EE_X_PROJECT_DIR)/test/Classes -type d -print}

LOCAL_STATIC_LIBRARIES := ee_x_crashlytics_static
LOCAL_STATIC_LIBRARIES += ee_x_notification_static
LOCAL_STATIC_LIBRARIES += ee_x_firebase_static
LOCAL_STATIC_LIBRARIES += ee_x_facebook_ads_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module, jni)