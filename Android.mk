LOCAL_PATH := $(call my-dir)

$(call import-add-path, $(LOCAL_PATH))

$(call import-module, src/cpp/ee)
$(call import-module, src/cpp/ee/soomla)
$(call import-module, third_party/firebase_cpp_sdk)
$(call import-module, third_party/jansson)
$(call import-module, third_party/soomla/core)
$(call import-module, third_party/soomla/store)
