LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := ee_x
LOCAL_ARM_MODE := arm
LOCAL_CPP_FEATURES := exceptions coroutine-ts
LOCAL_CPPFLAGS := -std=c++2a
LOCAL_SRC_FILES := \
	${shell find $(LOCAL_PATH)/ad_colony -print} \
	${shell find $(LOCAL_PATH)/ad_mob -print} \
	${shell find $(LOCAL_PATH)/adjust -print} \
	${shell find $(LOCAL_PATH)/ads -print} \
	${shell find $(LOCAL_PATH)/app_lovin -print} \
	${shell find $(LOCAL_PATH)/apps_flyer -print} \
	${shell find $(LOCAL_PATH)/cocos -print} \
	${shell find $(LOCAL_PATH)/core -print} \
	${shell find $(LOCAL_PATH)/facebook -print} \
	${shell find $(LOCAL_PATH)/facebook_ads -print} \
	${shell find $(LOCAL_PATH)/firebase_analytics -print} \
	${shell find $(LOCAL_PATH)/firebase_crashlytics -print} \
	${shell find $(LOCAL_PATH)/firebase_performance -print} \
	${shell find $(LOCAL_PATH)/iron_source -print} \
	${shell find $(LOCAL_PATH)/notification -print} \
	${shell find $(LOCAL_PATH)/play -print} \
	${shell find $(LOCAL_PATH)/recorder -print} \
	${shell find $(LOCAL_PATH)/services -print} \
	${shell find $(LOCAL_PATH)/store -print} \
	${shell find $(LOCAL_PATH)/unity_ads -print} \
	${shell find $(LOCAL_PATH)/vungle -print}

EE_X_ROOT_DIR = $(LOCAL_PATH)/../../..

LOCAL_C_INCLUDES := \
	$(EE_X_ROOT_DIR)/src/cpp \
	$(EE_X_ROOT_DIR)/third_party/nlohmann/include

LOCAL_EXPORT_C_INCLUDES := \
	$(EE_X_ROOT_DIR)/src/cpp \
	$(EE_X_ROOT_DIR)/third_party/nlohmann/include

ifeq ($(filter $(modules-get-list), cc_static),)
	# cocos-creator
	LOCAL_STATIC_LIBRARIES := cocos2dx_static
else
	# cocos2d-x
	LOCAL_STATIC_LIBRARIES := cc_static
endif

include $(BUILD_STATIC_LIBRARY)