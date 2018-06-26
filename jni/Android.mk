LOCAL_PATH := $(call my-dir)
EE_X_ROOT_DIR = $(LOCAL_PATH)/..

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

############
# ADMOB #
############

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

############
# CAMPAIGN RECEIVER #
############

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
LOCAL_SRC_FILES := $(EE_X_ROOT_DIR)/jni/cpp/ee/firebase/FirebaseApp.cpp
LOCAL_STATIC_LIBRARIES := ee_x_core_static
LOCAL_STATIC_LIBRARIES += ee_x_firebase_app_prebuilt
include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_CPP_FEATURES := exceptions
LOCAL_CPPFLAGS += -std=c++1z
LOCAL_MODULE := ee_x_firebase_analytics_static
LOCAL_SRC_FILES := $(EE_X_ROOT_DIR)/src/ee/firebase/FirebaseAnalytics.cpp
LOCAL_STATIC_LIBRARIES := ee_x_firebase_core_static
LOCAL_STATIC_LIBRARIES += ee_x_firebase_analytics_prebuilt
include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_CPP_FEATURES := exceptions
LOCAL_CPPFLAGS += -std=c++1z
LOCAL_MODULE := ee_x_firebase_dynamic_link_static
LOCAL_SRC_FILES := $(EE_X_ROOT_DIR)/src/ee/firebase/FirebaseDynamicLink.cpp
LOCAL_STATIC_LIBRARIES := ee_x_firebase_core_static
LOCAL_STATIC_LIBRARIES += ee_x_firebase_dynamic_link_prebuilt
include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_CPP_FEATURES := exceptions
LOCAL_CPPFLAGS += -std=c++1z
LOCAL_MODULE := ee_x_firebase_messaging_static
LOCAL_SRC_FILES := $(EE_X_ROOT_DIR)/src/ee/firebase/FirebaseMessaging.cpp
LOCAL_STATIC_LIBRARIES := ee_x_firebase_core_static
LOCAL_STATIC_LIBRARIES += ee_x_firebase_messaging_prebuilt
include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_CPP_FEATURES := exceptions
LOCAL_CPPFLAGS += -std=c++1z
LOCAL_MODULE := ee_x_firebase_remote_config_static
LOCAL_SRC_FILES := $(EE_X_ROOT_DIR)/src/ee/firebase/FirebaseRemoteConfig.cpp
LOCAL_STATIC_LIBRARIES := ee_x_firebase_core_static
LOCAL_STATIC_LIBRARIES += ee_x_firebase_remote_config_prebuilt
include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_CPP_FEATURES := exceptions
LOCAL_CPPFLAGS += -std=c++1z
LOCAL_MODULE := ee_x_firebase_storage_static
LOCAL_SRC_FILES := $(EE_X_ROOT_DIR)/src/ee/firebase/FirebaseStorage.cpp
LOCAL_STATIC_LIBRARIES := ee_x_firebase_core_static
LOCAL_STATIC_LIBRARIES += ee_x_firebase_storage_prebuilt
include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := ee_x_firebase_static
LOCAL_STATIC_LIBRARIES := ee_x_firebase_analytics_static
LOCAL_STATIC_LIBRARIES += ee_x_firebase_messaging_static
LOCAL_STATIC_LIBRARIES += ee_x_firebase_remote_config_static
LOCAL_STATIC_LIBRARIES += ee_x_firebase_storage_static
include $(BUILD_STATIC_LIBRARY)

include $(EE_X_ROOT_DIR)/third_party/firebase_cpp_sdk/Android.mk