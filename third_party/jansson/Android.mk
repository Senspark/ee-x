LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE    := jansson_static

LOCAL_MODULE_FILENAME := libjansson

LOCAL_SRC_FILES := dump.c \
    error.c \
    hashtable.c \
    load.c \
    memory.c \
    pack_unpack.c \
    strbuffer.c \
    strconv.c \
    utf.c \
    value.c


LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)
                    
include $(BUILD_STATIC_LIBRARY)
