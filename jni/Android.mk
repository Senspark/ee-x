LOCAL_PATH := $(call my-dir)
EE_X_ROOT_DIR = $(LOCAL_PATH)/..

########
# CORE #
########

include $(CLEAR_VARS)

LOCAL_CPP_FEATURES += exceptions

LOCAL_MODULE := ee_x_core_static

LOCAL_SRC_FILES := ${shell find $(EE_X_ROOT_DIR)/src/ee/core -name "*.cpp" -print}
LOCAL_SRC_FILES += ${shell find $(EE_X_ROOT_DIR)/proj.android_studio/cpp/ee/core -name "*.cpp" -print}
                   
LOCAL_C_INCLUDES := $(EE_X_ROOT_DIR)/src
LOCAL_C_INCLUDES += $(EE_X_ROOT_DIR)/proj.android_studio/cpp

LOCAL_EXPORT_C_INCLUDES := $(EE_X_ROOT_DIR)/src
LOCAL_EXPORT_C_INCLUDES += $(EE_X_ROOT_DIR)/proj.android_studio/cpp

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