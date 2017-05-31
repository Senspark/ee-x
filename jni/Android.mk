LOCAL_PATH := $(call my-dir)
EE_X_ROOT_DIR = $(LOCAL_PATH)/..

########
# CORE #
########

include $(CLEAR_VARS)

LOCAL_CPP_FEATURES += exceptions

LOCAL_MODULE := ee_x_core_static

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

LOCAL_MODULE := ee_x_crashlytics_static
LOCAL_SRC_FILES := ${shell find $(EE_X_ROOT_DIR)/src/ee/crashlytics -name "*.cpp" -print}
LOCAL_STATIC_LIBRARIES := ee_x_core_static

include $(BUILD_STATIC_LIBRARY)

################
# NOTIFICATION #
################

include $(CLEAR_VARS)

LOCAL_MODULE := ee_x_notification_static
LOCAL_SRC_FILES := ${shell find $(EE_X_ROOT_DIR)/src/ee/notification -name "*.cpp" -print}
LOCAL_STATIC_LIBRARIES := ee_x_core_static

include $(BUILD_STATIC_LIBRARY)

################
# FACEBOOK_ADS #
################

include $(CLEAR_VARS)

LOCAL_MODULE := ee_x_facebook_ads_static
LOCAL_SRC_FILES := ${shell find $(EE_X_ROOT_DIR)/src/ee/facebook -name "*.cpp" -print}
LOCAL_STATIC_LIBRARIES := ee_x_core_static

include $(BUILD_STATIC_LIBRARY)

############
# FIREBASE #
############

include $(CLEAR_VARS)

LOCAL_MODULE := ee_x_firebase_static

LOCAL_CPPFLAGS := -std=c++14

LOCAL_SRC_FILES := ${shell find $(EE_X_ROOT_DIR)/src/ee/firebase -name "*.cpp" -print}
LOCAL_SRC_FILES += ${shell find $(EE_X_ROOT_DIR)/jni/cpp/ee/firebase -name "*.cpp" -print}

LOCAL_STATIC_LIBRARIES := ee_x_core_static
LOCAL_STATIC_LIBRARIES += firebase_analytics_prebuilt
LOCAL_STATIC_LIBRARIES += firebase_remote_config_prebuilt
LOCAL_STATIC_LIBRARIES += firebase_storage_prebuilt
LOCAL_STATIC_LIBRARIES += firebase_app_prebuilt

include $(BUILD_STATIC_LIBRARY)

include $(EE_X_ROOT_DIR)/third_party/firebase_cpp_sdk/Android.mk