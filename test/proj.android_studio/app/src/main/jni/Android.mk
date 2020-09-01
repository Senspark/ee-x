LOCAL_PATH := $(call my-dir)

PROJECT_DIR := $(LOCAL_PATH)/../../../../..

include $(CLEAR_VARS)

$(call import-add-path, $(PROJECT_DIR))

LOCAL_MODULE := ee_x_test
LOCAL_CPPFLAGS := -std=c++2a

LOCAL_SRC_FILES := cpp/main.cpp
LOCAL_SRC_FILES += ${shell find $(PROJECT_DIR)/Classes -name "*.cpp" -print}

LOCAL_C_INCLUDES := ${shell find $(PROJECT_DIR)/Classes -type d -print}

LOCAL_STATIC_LIBRARIES := cc_static
LOCAL_STATIC_LIBRARIES := ccui
LOCAL_STATIC_LIBRARIES += soomla_store
LOCAL_STATIC_LIBRARIES += ee_x_adjust
LOCAL_STATIC_LIBRARIES += ee_x_admob
LOCAL_STATIC_LIBRARIES += ee_x_app_lovin
LOCAL_STATIC_LIBRARIES += ee_x_apps_flyer
LOCAL_STATIC_LIBRARIES += ee_x_campaign_receiver
LOCAL_STATIC_LIBRARIES += ee_x_cocos
LOCAL_STATIC_LIBRARIES += ee_x_facebook
LOCAL_STATIC_LIBRARIES += ee_x_facebook_ads
LOCAL_STATIC_LIBRARIES += ee_x_firebase_analytics
LOCAL_STATIC_LIBRARIES += ee_x_firebase_crashlytics
LOCAL_STATIC_LIBRARIES += ee_x_firebase_dynamic_link
LOCAL_STATIC_LIBRARIES += ee_x_firebase_messaging
LOCAL_STATIC_LIBRARIES += ee_x_firebase_performance
LOCAL_STATIC_LIBRARIES += ee_x_firebase_remote_config
LOCAL_STATIC_LIBRARIES += ee_x_firebase_storage
LOCAL_STATIC_LIBRARIES += ee_x_google_analytics
LOCAL_STATIC_LIBRARIES += ee_x_iron_source
LOCAL_STATIC_LIBRARIES += ee_x_notification
LOCAL_STATIC_LIBRARIES += ee_x_play
LOCAL_STATIC_LIBRARIES += ee_x_recorder
LOCAL_STATIC_LIBRARIES += ee_x_unity_ads
LOCAL_STATIC_LIBRARIES += ee_x_vungle

include $(BUILD_SHARED_LIBRARY)

$(call import-module, cocos2d/cocos) # Must be placed before ../jni so modules-get-list can work properly.
$(call import-module, ..)
