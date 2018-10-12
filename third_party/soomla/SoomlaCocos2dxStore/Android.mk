LOCAL_PATH := $(call my-dir)
EE_X_ROOT_DIR = $(LOCAL_PATH)/../../../
COCOS2D_ROOT_DIR = $(LOCAL_PATH)/../../../../../../cocos2d-x

# cocos2dx-store
include $(CLEAR_VARS)

COCOS2D_JAVASCRIPT = $(filter %-DCOCOS2D_JAVASCRIPT=1,$(APP_CPPFLAGS))

LOCAL_MODULE := cocos2dx_store_static
LOCAL_MODULE_FILENAME := libcocos2dxstore

STORE_SRC_LIST := $(wildcard $(LOCAL_PATH)/Soomla/*.cpp)
STORE_SRC_LIST += $(wildcard $(LOCAL_PATH)/Soomla/config/*.cpp)
STORE_SRC_LIST += $(wildcard $(LOCAL_PATH)/Soomla/data/*.cpp)
STORE_SRC_LIST += $(wildcard $(LOCAL_PATH)/Soomla/domain/*.cpp)
STORE_SRC_LIST += $(wildcard $(LOCAL_PATH)/Soomla/domain/virtualCurrencies/*.cpp)
STORE_SRC_LIST += $(wildcard $(LOCAL_PATH)/Soomla/domain/virtualGoods/*.cpp)
STORE_SRC_LIST += $(wildcard $(LOCAL_PATH)/Soomla/NativeImpl/*.cpp)
STORE_SRC_LIST += $(wildcard $(LOCAL_PATH)/Soomla/PurchaseTypes/*.cpp)
STORE_SRC_LIST += $(wildcard $(LOCAL_PATH)/Soomla/rewards/*.cpp)

LOCAL_SRC_FILES := $(STORE_SRC_LIST:$(LOCAL_PATH)/%=%)

LOCAL_C_INCLUDES := $(LOCAL_PATH)/Soomla
LOCAL_C_INCLUDES += $(LOCAL_PATH)/Soomla/config
LOCAL_C_INCLUDES += $(LOCAL_PATH)/Soomla/data
LOCAL_C_INCLUDES += $(LOCAL_PATH)/Soomla/domain
LOCAL_C_INCLUDES += $(LOCAL_PATH)/Soomla/domain/virtualCurrencies
LOCAL_C_INCLUDES += $(LOCAL_PATH)/Soomla/domain/virtualGoods
LOCAL_C_INCLUDES += $(LOCAL_PATH)/Soomla/PurchaseTypes
LOCAL_C_INCLUDES += $(LOCAL_PATH)/Soomla/NativeImpl
LOCAL_C_INCLUDES += $(LOCAL_PATH)/Soomla/rewards
LOCAL_C_INCLUDES += $(COCOS2D_ROOT_DIR)
LOCAL_C_INCLUDES += $(COCOS2D_ROOT_DIR)/cocos

LOCAL_WHOLE_STATIC_LIBRARIES += cocos2dx_soomla_common_static

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)
LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/Soomla
LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/Soomla/config
LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/Soomla/data
LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/Soomla/domain
LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/Soomla/domain/virtualCurrencies
LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/Soomla/domain/virtualGoods
LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/Soomla/PurchaseTypes
LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/Soomla/rewards

include $(BUILD_STATIC_LIBRARY)

##############
# JSB SOOMLA #
##############

include $(CLEAR_VARS)

LOCAL_CPP_FEATURES := exceptions
LOCAL_CPPFLAGS += -std=c++1z
LOCAL_MODULE := ee_x_jsb_soomla_static
LOCAL_C_INCLUDES := $(EE_X_ROOT_DIR)/src/ee/jsb/core
LOCAL_C_INCLUDES += $(COCOS2D_ROOT_DIR)
LOCAL_C_INCLUDES += $(COCOS2D_ROOT_DIR)/cocos
LOCAL_C_INCLUDES += $(COCOS2D_ROOT_DIR)/cocos/editor-support
LOCAL_C_INCLUDES += $(COCOS2D_ROOT_DIR)/external/sources
LOCAL_C_INCLUDES += $(COCOS2D_ROOT_DIR)/external/android/$(TARGET_ARCH_ABI)/include/v8
LOCAL_SRC_FILES := ${shell find $(EE_X_ROOT_DIR)/src/ee/jsb/soomla -name "*.cpp" -print}
LOCAL_STATIC_LIBRARIES := ee_x_jsb_core_static
LOCAL_STATIC_LIBRARIES += cocos2dx_store_static

include $(BUILD_STATIC_LIBRARY)

$(call import-add-path, $(LOCAL_PATH)/../)
$(call import-add-path, $(LOCAL_PATH)/../../../)
$(call import-module,SoomlaCocos2dxCore)