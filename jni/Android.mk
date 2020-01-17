LOCAL_PATH := $(call my-dir)
EE_X_ROOT_DIR = $(LOCAL_PATH)/..
COCOS2D_ROOT_DIR = $(LOCAL_PATH)/../../../../cocos2d-x

########
# CORE #
########

include $(CLEAR_VARS)

LOCAL_MODULE := ee_x_core_static

LOCAL_CPP_FEATURES := exceptions
LOCAL_CPPFLAGS := -std=c++1z

LOCAL_SRC_FILES := ${shell find $(EE_X_ROOT_DIR)/src/ee/core -name "*.cpp" -print}
LOCAL_SRC_FILES += ${shell find $(EE_X_ROOT_DIR)/jni/cpp/ee/core -name "*.cpp" -print}

LOCAL_C_INCLUDES := $(EE_X_ROOT_DIR)/src
LOCAL_C_INCLUDES += $(EE_X_ROOT_DIR)/jni/cpp
LOCAL_C_INCLUDES += $(EE_X_ROOT_DIR)/third_party/nlohmann/include

LOCAL_EXPORT_C_INCLUDES := $(EE_X_ROOT_DIR)/src
LOCAL_EXPORT_C_INCLUDES += $(EE_X_ROOT_DIR)/jni/cpp
LOCAL_EXPORT_C_INCLUDES += $(EE_X_ROOT_DIR)/third_party/nlohmann/include

include $(BUILD_STATIC_LIBRARY)

############
# TWITTER #
############

include $(CLEAR_VARS)

LOCAL_CPP_FEATURES := exceptions
LOCAL_CPPFLAGS += -std=c++1z
LOCAL_MODULE := ee_x_twitter_static
LOCAL_SRC_FILES := ${shell find $(EE_X_ROOT_DIR)/src/ee/twitter -name "*.cpp" -print}
LOCAL_STATIC_LIBRARIES := ee_x_core_static

include $(BUILD_STATIC_LIBRARY)


###############
# CRASHLYTICS #
###############

include $(CLEAR_VARS)

LOCAL_CPP_FEATURES := exceptions
LOCAL_CPPFLAGS += -std=c++1z
LOCAL_MODULE := ee_x_crashlytics_static
LOCAL_SRC_FILES := ${shell find $(EE_X_ROOT_DIR)/src/ee/crashlytics -name "*.cpp" -print}
LOCAL_STATIC_LIBRARIES := ee_x_core_static

include $(BUILD_STATIC_LIBRARY)

################
# NOTIFICATION #
################

include $(CLEAR_VARS)

LOCAL_CPP_FEATURES := exceptions
LOCAL_CPPFLAGS += -std=c++1z
LOCAL_MODULE := ee_x_notification_static
LOCAL_SRC_FILES := ${shell find $(EE_X_ROOT_DIR)/src/ee/notification -name "*.cpp" -print}
LOCAL_STATIC_LIBRARIES := ee_x_core_static

include $(BUILD_STATIC_LIBRARY)

####################
# GOOGLE ANALYTICS #
####################

include $(CLEAR_VARS)

LOCAL_CPP_FEATURES := exceptions
LOCAL_CPPFLAGS += -std=c++1z
LOCAL_MODULE := ee_x_google_analytics_static
LOCAL_SRC_FILES := ${shell find $(EE_X_ROOT_DIR)/src/ee/google -name "*.cpp" -print}
LOCAL_STATIC_LIBRARIES := ee_x_core_static

include $(BUILD_STATIC_LIBRARY)

##############
# APPS FLYER #
##############

include $(CLEAR_VARS)

LOCAL_CPP_FEATURES := exceptions
LOCAL_CPPFLAGS += -std=c++1z
LOCAL_MODULE := ee_x_apps_flyer_static
LOCAL_SRC_FILES := ${shell find $(EE_X_ROOT_DIR)/src/ee/appsflyer -name "*.cpp" -print}
LOCAL_STATIC_LIBRARIES := ee_x_core_static

include $(BUILD_STATIC_LIBRARY)

##########
# TENJIN #
##########

include $(CLEAR_VARS)

LOCAL_CPP_FEATURES := exceptions
LOCAL_CPPFLAGS := -std=c++17
LOCAL_MODULE := ee_x_tenjin_static
LOCAL_SRC_FILES := ${shell find $(EE_X_ROOT_DIR)/src/ee/tenjin -name "*.cpp" -print}
LOCAL_STATIC_LIBRARIES := ee_x_core_static

include $(BUILD_STATIC_LIBRARY)

##############################
# Google Play Games Services #
##############################
include $(CLEAR_VARS)

LOCAL_CPP_FEATURES := exceptions
LOCAL_CPPFLAGS += -std=c++1z
LOCAL_MODULE := ee_x_play
LOCAL_SRC_FILES := ${shell find $(EE_X_ROOT_DIR)/src/ee/play -name "*.cpp" -print}
LOCAL_STATIC_LIBRARIES := ee_x_core_static

include $(BUILD_STATIC_LIBRARY)

#######
# ADS #
#######

include $(CLEAR_VARS)

LOCAL_CPP_FEATURES := exceptions
LOCAL_CPPFLAGS += -std=c++1z
LOCAL_MODULE := ee_x_ads_static
LOCAL_SRC_FILES := ${shell find $(EE_X_ROOT_DIR)/src/ee/ads -name "*.cpp" -print}
LOCAL_STATIC_LIBRARIES := ee_x_core_static

include $(BUILD_STATIC_LIBRARY)

#########
# ADMOB #
#########

include $(CLEAR_VARS)

LOCAL_CPP_FEATURES := exceptions
LOCAL_CPPFLAGS += -std=c++1z
LOCAL_MODULE := ee_x_admob_static
LOCAL_SRC_FILES := ${shell find $(EE_X_ROOT_DIR)/src/ee/admob -name "*.cpp" -print}
LOCAL_STATIC_LIBRARIES := ee_x_ads_static

include $(BUILD_STATIC_LIBRARY)

############
# APPLOVIN #
############

include $(CLEAR_VARS)

LOCAL_CPP_FEATURES := exceptions
LOCAL_CPPFLAGS += -std=c++1z
LOCAL_MODULE := ee_x_applovin_static
LOCAL_SRC_FILES := ${shell find $(EE_X_ROOT_DIR)/src/ee/applovin -name "*.cpp" -print}
LOCAL_STATIC_LIBRARIES := ee_x_ads_static

include $(BUILD_STATIC_LIBRARY)

################
# FACEBOOK_ADS #
################

include $(CLEAR_VARS)

LOCAL_CPP_FEATURES := exceptions
LOCAL_CPPFLAGS += -std=c++1z
LOCAL_MODULE := ee_x_facebook_ads_static
LOCAL_SRC_FILES := ${shell find $(EE_X_ROOT_DIR)/src/ee/facebookads -name "*.cpp" -print}
LOCAL_STATIC_LIBRARIES := ee_x_ads_static

include $(BUILD_STATIC_LIBRARY)

##################
# IRONSOURCE_ADS #
##################

include $(CLEAR_VARS)

LOCAL_CPP_FEATURES := exceptions
LOCAL_CPPFLAGS += -std=c++1z
LOCAL_MODULE := ee_x_ironsource_static
LOCAL_SRC_FILES := ${shell find $(EE_X_ROOT_DIR)/src/ee/ironsource -name "*.cpp" -print}
LOCAL_STATIC_LIBRARIES := ee_x_ads_static

include $(BUILD_STATIC_LIBRARY)

#############
# UNITY_ADS #
#############

include $(CLEAR_VARS)

LOCAL_CPP_FEATURES := exceptions
LOCAL_CPPFLAGS += -std=c++1z
LOCAL_MODULE := ee_x_unity_ads_static
LOCAL_SRC_FILES := ${shell find $(EE_X_ROOT_DIR)/src/ee/unityads -name "*.cpp" -print}
LOCAL_STATIC_LIBRARIES := ee_x_ads_static

include $(BUILD_STATIC_LIBRARY)

##########
# VUNGLE #
##########

include $(CLEAR_VARS)

LOCAL_CPP_FEATURES := exceptions
LOCAL_CPPFLAGS += -std=c++1z
LOCAL_MODULE := ee_x_vungle_static
LOCAL_SRC_FILES := ${shell find $(EE_X_ROOT_DIR)/src/ee/vungle -name "*.cpp" -print}
LOCAL_STATIC_LIBRARIES := ee_x_ads_static

include $(BUILD_STATIC_LIBRARY)

############
# FACEBOOK #
############

include $(CLEAR_VARS)

LOCAL_CPP_FEATURES := exceptions
LOCAL_CPPFLAGS += -std=c++1z
LOCAL_MODULE := ee_x_facebook_static
LOCAL_SRC_FILES := ${shell find $(EE_X_ROOT_DIR)/src/ee/facebook -name "*.cpp" -print}
LOCAL_STATIC_LIBRARIES := ee_x_core_static

include $(BUILD_STATIC_LIBRARY)

############
# RECORDER #
############

include $(CLEAR_VARS)

LOCAL_CPP_FEATURES := exceptions
LOCAL_CPPFLAGS += -std=c++1z
LOCAL_MODULE := ee_x_recorder_static
LOCAL_SRC_FILES := ${shell find $(EE_X_ROOT_DIR)/src/ee/recorder -name "*.cpp" -print}
LOCAL_STATIC_LIBRARIES := ee_x_core_static

include $(BUILD_STATIC_LIBRARY)

#####################
# CAMPAIGN RECEIVER #
#####################

include $(CLEAR_VARS)

LOCAL_CPP_FEATURES := exceptions
LOCAL_CPPFLAGS += -std=c++1z
LOCAL_MODULE := ee_x_campaign_receiver_static
LOCAL_SRC_FILES := ${shell find $(EE_X_ROOT_DIR)/src/ee/campaignreceiver -name "*.cpp" -print}
LOCAL_STATIC_LIBRARIES := ee_x_core_static

include $(BUILD_STATIC_LIBRARY)

############
# FIREBASE #
############

include $(CLEAR_VARS)
LOCAL_CPP_FEATURES := exceptions
LOCAL_CPPFLAGS += -std=c++1z
LOCAL_MODULE := ee_x_firebase_core_static
LOCAL_SRC_FILES := ${shell find $(EE_X_ROOT_DIR)/src/ee/firebase/core -name "*.cpp" -print}
LOCAL_STATIC_LIBRARIES := ee_x_core_static
LOCAL_STATIC_LIBRARIES += ee_x_firebase_app_prebuilt
include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_CPP_FEATURES := exceptions
LOCAL_CPPFLAGS += -std=c++1z
LOCAL_MODULE := ee_x_firebase_analytics_static
LOCAL_SRC_FILES := ${shell find $(EE_X_ROOT_DIR)/src/ee/firebase/analytics -name "*.cpp" -print}
LOCAL_STATIC_LIBRARIES := ee_x_firebase_core_static
LOCAL_STATIC_LIBRARIES += ee_x_firebase_analytics_prebuilt
include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_CPP_FEATURES := exceptions
LOCAL_CPPFLAGS += -std=c++1z
LOCAL_MODULE := ee_x_firebase_dynamic_link_static
LOCAL_SRC_FILES := ${shell find $(EE_X_ROOT_DIR)/src/ee/firebase/dynamiclink -name "*.cpp" -print}
LOCAL_STATIC_LIBRARIES := ee_x_firebase_core_static
LOCAL_STATIC_LIBRARIES += ee_x_firebase_dynamic_link_prebuilt
include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_CPP_FEATURES := exceptions
LOCAL_CPPFLAGS += -std=c++1z
LOCAL_MODULE := ee_x_firebase_messaging_static
LOCAL_SRC_FILES := ${shell find $(EE_X_ROOT_DIR)/src/ee/firebase/messaging -name "*.cpp" -print}
LOCAL_STATIC_LIBRARIES := ee_x_firebase_core_static
LOCAL_STATIC_LIBRARIES += ee_x_firebase_messaging_prebuilt
include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_CPP_FEATURES := exceptions
LOCAL_CPPFLAGS += -std=c++1z
LOCAL_MODULE := ee_x_firebase_remote_config_static
LOCAL_SRC_FILES := ${shell find $(EE_X_ROOT_DIR)/src/ee/firebase/remoteconfig -name "*.cpp" -print}
LOCAL_STATIC_LIBRARIES := ee_x_firebase_core_static
LOCAL_STATIC_LIBRARIES += ee_x_firebase_remote_config_prebuilt
include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_CPP_FEATURES := exceptions
LOCAL_CPPFLAGS += -std=c++1z
LOCAL_MODULE := ee_x_firebase_storage_static
LOCAL_SRC_FILES := ${shell find $(EE_X_ROOT_DIR)/src/ee/firebase/storage -name "*.cpp" -print}
LOCAL_STATIC_LIBRARIES := ee_x_firebase_core_static
LOCAL_STATIC_LIBRARIES += ee_x_firebase_storage_prebuilt
include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_CPP_FEATURES := exceptions
LOCAL_CPPFLAGS += -std=c++1z
LOCAL_MODULE := ee_x_firebase_performance_static
LOCAL_SRC_FILES := ${shell find $(EE_X_ROOT_DIR)/src/ee/firebase/performance -name "*.cpp" -print}
LOCAL_STATIC_LIBRARIES := ee_x_firebase_core_static
include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := ee_x_firebase_static
LOCAL_STATIC_LIBRARIES := ee_x_firebase_analytics_static
LOCAL_STATIC_LIBRARIES += ee_x_firebase_messaging_static
LOCAL_STATIC_LIBRARIES += ee_x_firebase_remote_config_static
LOCAL_STATIC_LIBRARIES += ee_x_firebase_storage_static
include $(BUILD_STATIC_LIBRARY)

############
# JSB_CORE #
############

include $(CLEAR_VARS)

LOCAL_CPP_FEATURES := exceptions
LOCAL_CPPFLAGS += -std=c++1z
LOCAL_MODULE := ee_x_jsb_core_static
LOCAL_C_INCLUDES := $(COCOS2D_ROOT_DIR)
LOCAL_C_INCLUDES += $(COCOS2D_ROOT_DIR)/cocos
LOCAL_C_INCLUDES += $(COCOS2D_ROOT_DIR)/cocos/editor-support
LOCAL_C_INCLUDES += $(COCOS2D_ROOT_DIR)/external/sources
LOCAL_C_INCLUDES += $(COCOS2D_ROOT_DIR)/external/android/$(TARGET_ARCH_ABI)/include/v8
LOCAL_EXPORT_C_INCLUDES := $(COCOS2D_ROOT_DIR)
LOCAL_EXPORT_C_INCLUDES += $(COCOS2D_ROOT_DIR)/cocos
LOCAL_EXPORT_C_INCLUDES += $(COCOS2D_ROOT_DIR)/cocos/editor-support
LOCAL_EXPORT_C_INCLUDES += $(COCOS2D_ROOT_DIR)/external/sources
LOCAL_EXPORT_C_INCLUDES += $(COCOS2D_ROOT_DIR)/external/android/$(TARGET_ARCH_ABI)/include/v8
LOCAL_SRC_FILES := $(EE_X_ROOT_DIR)/src/ee/jsb/jsb_core.cpp
LOCAL_SRC_FILES += ${shell find $(EE_X_ROOT_DIR)/src/ee/jsb/core -name "*.cpp" -print}
LOCAL_STATIC_LIBRARIES := ee_x_core_static
include $(BUILD_STATIC_LIBRARY)

##################
# JSB_APPS_FLYER #
##################

include $(CLEAR_VARS)

LOCAL_CPP_FEATURES := exceptions
LOCAL_CPPFLAGS += -std=c++1z
LOCAL_MODULE := ee_x_jsb_apps_flyer_static
LOCAL_SRC_FILES := $(EE_X_ROOT_DIR)/src/ee/jsb/JsbAppsFlyer.cpp
LOCAL_SRC_FILES += ${shell find $(EE_X_ROOT_DIR)/src/ee/jsb/appsflyer -name "*.cpp" -print}
LOCAL_STATIC_LIBRARIES := ee_x_jsb_core_static
LOCAL_STATIC_LIBRARIES += ee_x_apps_flyer_static

include $(BUILD_STATIC_LIBRARY)

##############
# JSB_TENJIN #
##############

include $(CLEAR_VARS)

LOCAL_CPP_FEATURES := exceptions
LOCAL_CPPFLAGS := -std=c++17
LOCAL_MODULE := ee_x_jsb_tenjin_static
LOCAL_SRC_FILES := $(EE_X_ROOT_DIR)/src/ee/jsb/JsbTenjin.cpp
LOCAL_SRC_FILES += ${shell find $(EE_X_ROOT_DIR)/src/ee/jsb/tenjin -name "*.cpp" -print}
LOCAL_STATIC_LIBRARIES := ee_x_jsb_core_static
LOCAL_STATIC_LIBRARIES += ee_x_tenjin_static

include $(BUILD_STATIC_LIBRARY)

###########
# JSB_ADS #
###########

include $(CLEAR_VARS)

LOCAL_CPP_FEATURES := exceptions
LOCAL_CPPFLAGS += -std=c++1z
LOCAL_MODULE := ee_x_jsb_ads_static
LOCAL_SRC_FILES := $(EE_X_ROOT_DIR)/src/ee/jsb/jsb_ads.cpp
LOCAL_SRC_FILES += ${shell find $(EE_X_ROOT_DIR)/src/ee/jsb/ads -name "*.cpp" -print}
LOCAL_STATIC_LIBRARIES := ee_x_jsb_core_static

include $(BUILD_STATIC_LIBRARY)

#############
# JSB_ADMOB #
#############

include $(CLEAR_VARS)

LOCAL_CPP_FEATURES := exceptions
LOCAL_CPPFLAGS += -std=c++1z
LOCAL_MODULE := ee_x_jsb_admob_static
LOCAL_SRC_FILES := $(EE_X_ROOT_DIR)/src/ee/jsb/jsb_admob.cpp
LOCAL_SRC_FILES += ${shell find $(EE_X_ROOT_DIR)/src/ee/jsb/admob -name "*.cpp" -print}
LOCAL_STATIC_LIBRARIES := ee_x_admob_static
LOCAL_STATIC_LIBRARIES += ee_x_jsb_ads_static

include $(BUILD_STATIC_LIBRARY)

################
# JSB_FACEBOOK #
################

include $(CLEAR_VARS)

LOCAL_CPP_FEATURES := exceptions
LOCAL_CPPFLAGS += -std=c++1z
LOCAL_MODULE := ee_x_jsb_facebook_ads_static
LOCAL_SRC_FILES := $(EE_X_ROOT_DIR)/src/ee/jsb/jsb_facebook_ads.cpp
LOCAL_SRC_FILES += ${shell find $(EE_X_ROOT_DIR)/src/ee/jsb/facebookads -name "*.cpp" -print}
LOCAL_STATIC_LIBRARIES := ee_x_facebook_ads_static
LOCAL_STATIC_LIBRARIES += ee_x_jsb_ads_static

include $(BUILD_STATIC_LIBRARY)

######################
# JSB_IRONSOURCE_ADS #
######################

include $(CLEAR_VARS)

LOCAL_CPP_FEATURES := exceptions
LOCAL_CPPFLAGS += -std=c++1z
LOCAL_MODULE := ee_x_jsb_ironsource_static
LOCAL_SRC_FILES := $(EE_X_ROOT_DIR)/src/ee/jsb/jsb_ironsource.cpp
LOCAL_SRC_FILES += ${shell find $(EE_X_ROOT_DIR)/src/ee/jsb/ironsource -name "*.cpp" -print}
LOCAL_STATIC_LIBRARIES := ee_x_ironsource_static
LOCAL_STATIC_LIBRARIES += ee_x_jsb_ads_static

include $(BUILD_STATIC_LIBRARY)

#################
# JSB_UNITY_ADS #
#################

include $(CLEAR_VARS)

LOCAL_CPP_FEATURES := exceptions
LOCAL_CPPFLAGS += -std=c++1z
LOCAL_MODULE := ee_x_jsb_unity_ads_static
LOCAL_SRC_FILES := $(EE_X_ROOT_DIR)/src/ee/jsb/jsb_unity_ads.cpp
LOCAL_SRC_FILES += ${shell find $(EE_X_ROOT_DIR)/src/ee/jsb/unityads -name "*.cpp" -print}
LOCAL_STATIC_LIBRARIES := ee_x_unity_ads_static
LOCAL_STATIC_LIBRARIES += ee_x_jsb_ads_static

include $(BUILD_STATIC_LIBRARY)

##################
# JSB_VUNGLE_ADS #
##################

include $(CLEAR_VARS)

LOCAL_CPP_FEATURES := exceptions
LOCAL_CPPFLAGS += -std=c++1z
LOCAL_MODULE := ee_x_jsb_vungle_static
LOCAL_SRC_FILES := $(EE_X_ROOT_DIR)/src/ee/jsb/jsb_vungle.cpp
LOCAL_SRC_FILES += ${shell find $(EE_X_ROOT_DIR)/src/ee/jsb/vungle -name "*.cpp" -print}
LOCAL_STATIC_LIBRARIES := ee_x_vungle_static
LOCAL_STATIC_LIBRARIES += ee_x_jsb_ads_static

include $(BUILD_STATIC_LIBRARY)

###################
# JSB CRASHLYTICS #
###################

include $(CLEAR_VARS)

LOCAL_CPP_FEATURES := exceptions
LOCAL_CPPFLAGS += -std=c++1z
LOCAL_MODULE := ee_x_jsb_crashlytics_static
LOCAL_SRC_FILES := $(EE_X_ROOT_DIR)/src/ee/jsb/jsb_crashlytics.cpp
LOCAL_SRC_FILES += ${shell find $(EE_X_ROOT_DIR)/src/ee/jsb/crashlytics -name "*.cpp" -print}
LOCAL_STATIC_LIBRARIES := ee_x_jsb_core_static
LOCAL_STATIC_LIBRARIES += ee_x_crashlytics_static

include $(BUILD_STATIC_LIBRARY)

########################
# JSB GOOGLE ANALYTICS #
########################

include $(CLEAR_VARS)

LOCAL_CPP_FEATURES := exceptions
LOCAL_CPPFLAGS += -std=c++1z
LOCAL_MODULE := ee_x_jsb_google_analytics_static
LOCAL_SRC_FILES := $(EE_X_ROOT_DIR)/src/ee/jsb/jsb_google_analytics.cpp
LOCAL_SRC_FILES += ${shell find $(EE_X_ROOT_DIR)/src/ee/jsb/google -name "*.cpp" -print}
LOCAL_STATIC_LIBRARIES := ee_x_jsb_core_static
LOCAL_STATIC_LIBRARIES += ee_x_google_analytics_static

include $(BUILD_STATIC_LIBRARY)

####################
# JSB NOTIFICATION #
####################

include $(CLEAR_VARS)

LOCAL_CPP_FEATURES := exceptions
LOCAL_CPPFLAGS += -std=c++1z
LOCAL_MODULE := ee_x_jsb_notification_static
LOCAL_SRC_FILES := $(EE_X_ROOT_DIR)/src/ee/jsb/jsb_notification.cpp
LOCAL_SRC_FILES += ${shell find $(EE_X_ROOT_DIR)/src/ee/jsb/notification -name "*.cpp" -print}
LOCAL_STATIC_LIBRARIES := ee_x_jsb_core_static
LOCAL_STATIC_LIBRARIES += ee_x_notification_static

include $(BUILD_STATIC_LIBRARY)

################
# JSB FIREBASE #
################

include $(CLEAR_VARS)
LOCAL_CPP_FEATURES := exceptions
LOCAL_CPPFLAGS += -std=c++1z
LOCAL_MODULE := ee_x_jsb_firebase_analytics_static
LOCAL_SRC_FILES := $(EE_X_ROOT_DIR)/src/ee/jsb/jsb_firebase.cpp
LOCAL_SRC_FILES += $(EE_X_ROOT_DIR)/src/ee/jsb/firebase/jsb_firebase_analytics.cpp
LOCAL_STATIC_LIBRARIES := ee_x_jsb_core_static
LOCAL_STATIC_LIBRARIES += ee_x_firebase_analytics_static
include $(BUILD_STATIC_LIBRARY)

##############################
# JSB FIREBASE REMOTE CONFIG #
##############################

include $(CLEAR_VARS)
LOCAL_CPP_FEATURES := exceptions
LOCAL_CPPFLAGS += -std=c++1z
LOCAL_MODULE := ee_x_jsb_firebase_remote_config_static
LOCAL_SRC_FILES := $(EE_X_ROOT_DIR)/src/ee/jsb/firebase/jsb_firebase_remote_config.cpp
LOCAL_SRC_FILES += $(EE_X_ROOT_DIR)/src/ee/jsb/firebase/JsbFirebaseConfigInfo.cpp
LOCAL_SRC_FILES += $(EE_X_ROOT_DIR)/src/ee/jsb/firebase/JsbFirebaseFetchFailureReason.cpp
LOCAL_SRC_FILES += $(EE_X_ROOT_DIR)/src/ee/jsb/firebase/JsbFirebaseLastFetchStatus.cpp
LOCAL_STATIC_LIBRARIES := ee_x_jsb_core_static
LOCAL_STATIC_LIBRARIES += ee_x_firebase_remote_config_static
include $(BUILD_STATIC_LIBRARY)

############################
# JSB FIREBASE PERFORMANCE #
############################

include $(CLEAR_VARS)
LOCAL_CPP_FEATURES := exceptions
LOCAL_CPPFLAGS += -std=c++1z
LOCAL_MODULE := ee_x_jsb_firebase_performance_static
LOCAL_SRC_FILES := $(EE_X_ROOT_DIR)/src/ee/jsb/firebase/JsbFirebasePerformance.cpp
LOCAL_SRC_FILES += $(EE_X_ROOT_DIR)/src/ee/jsb/firebase/JsbFirebasePerformanceTrace.cpp
LOCAL_STATIC_LIBRARIES := ee_x_jsb_core_static
LOCAL_STATIC_LIBRARIES += ee_x_firebase_performance_static
include $(BUILD_STATIC_LIBRARY)

################
#  JSB SOOMLA  #
################

include $(CLEAR_VARS)
LOCAL_CPP_FEATURES := exceptions
LOCAL_CPPFLAGS += -std=c++1z
LOCAL_MODULE := ee_x_jsb_soomla_store_static

LOCAL_SRC_FILES := $(EE_X_ROOT_DIR)/src/ee/jsb/jsb_soomla.cpp
LOCAL_SRC_FILES += ${shell find $(EE_X_ROOT_DIR)/src/ee/jsb/soomla -name "*.cpp" -print}

LOCAL_STATIC_LIBRARIES := ee_x_jsb_core_static
LOCAL_STATIC_LIBRARIES += cocos2dx_store_static
include $(BUILD_STATIC_LIBRARY)

##################
#  JSB FACEBOOK  #
##################

include $(CLEAR_VARS)
LOCAL_CPP_FEATURES := exceptions
LOCAL_CPPFLAGS += -std=c++1z
LOCAL_MODULE := ee_x_jsb_facebook_static

LOCAL_SRC_FILES := $(EE_X_ROOT_DIR)/src/ee/jsb/jsb_facebook.cpp
LOCAL_SRC_FILES += ${shell find $(EE_X_ROOT_DIR)/src/ee/jsb/facebook -name "*.cpp" -print}

LOCAL_STATIC_LIBRARIES := ee_x_jsb_core_static
LOCAL_STATIC_LIBRARIES += ee_x_facebook_static
include $(BUILD_STATIC_LIBRARY)

################
# JSB RECORDER #
################

include $(CLEAR_VARS)
LOCAL_CPP_FEATURES := exceptions
LOCAL_CPPFLAGS += -std=c++1z
LOCAL_MODULE := ee_x_jsb_recorder_static

LOCAL_SRC_FILES := $(EE_X_ROOT_DIR)/src/ee/jsb/jsb_recorder.cpp
LOCAL_SRC_FILES += ${shell find $(EE_X_ROOT_DIR)/src/ee/jsb/recorder -name "*.cpp" -print}
LOCAL_STATIC_LIBRARIES := ee_x_jsb_core_static
LOCAL_STATIC_LIBRARIES += ee_x_recorder_static
include $(BUILD_STATIC_LIBRARY)

#################
include $(EE_X_ROOT_DIR)/third_party/firebase_cpp_sdk/Android.mk
