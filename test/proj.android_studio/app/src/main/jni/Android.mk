LOCAL_PATH := $(call my-dir)

PROJECT_DIR := $(LOCAL_PATH)/../../../../..

include $(CLEAR_VARS)

$(call import-add-path, $(PROJECT_DIR))

LOCAL_MODULE := ee_x_test
LOCAL_CPPFLAGS := -std=c++17

LOCAL_SRC_FILES := cpp/main.cpp
LOCAL_SRC_FILES += ${shell find $(PROJECT_DIR)/Classes -name "*.cpp" -print}

LOCAL_C_INCLUDES := ${shell find $(PROJECT_DIR)/Classes -type d -print}

LOCAL_STATIC_LIBRARIES := cc_static
LOCAL_STATIC_LIBRARIES := ccui
LOCAL_STATIC_LIBRARIES += soomla_store_static
LOCAL_STATIC_LIBRARIES += ee_x_admob_static
LOCAL_STATIC_LIBRARIES += ee_x_app_lovin_static
LOCAL_STATIC_LIBRARIES += ee_x_apps_flyer_static
LOCAL_STATIC_LIBRARIES += ee_x_campaign_receiver_static
LOCAL_STATIC_LIBRARIES += ee_x_cocos_static
LOCAL_STATIC_LIBRARIES += ee_x_crashlytics_static
LOCAL_STATIC_LIBRARIES += ee_x_facebook_static
LOCAL_STATIC_LIBRARIES += ee_x_facebook_ads_static
LOCAL_STATIC_LIBRARIES += ee_x_firebase_analytics_static
LOCAL_STATIC_LIBRARIES += ee_x_firebase_dynamic_link_static
LOCAL_STATIC_LIBRARIES += ee_x_firebase_messaging_static
LOCAL_STATIC_LIBRARIES += ee_x_firebase_performance_static
LOCAL_STATIC_LIBRARIES += ee_x_firebase_remote_config_static
LOCAL_STATIC_LIBRARIES += ee_x_firebase_storage_static
LOCAL_STATIC_LIBRARIES += ee_x_google_analytics_static
LOCAL_STATIC_LIBRARIES += ee_x_iron_source_static
LOCAL_STATIC_LIBRARIES += ee_x_notification_static
LOCAL_STATIC_LIBRARIES += ee_x_play_static
LOCAL_STATIC_LIBRARIES += ee_x_recorder_static
LOCAL_STATIC_LIBRARIES += ee_x_tenjin_static
LOCAL_STATIC_LIBRARIES += ee_x_twitter_static
LOCAL_STATIC_LIBRARIES += ee_x_unity_ads_static
LOCAL_STATIC_LIBRARIES += ee_x_vungle_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module, cocos2d/cocos) # Must be placed before ../jni so modules-get-list can work properly.
$(call import-module, ..)
