LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

define all-c-files-under
$(patsubst ./%,%, \
  $(shell cd $(LOCAL_PATH) ; \
          find $(1) -name "*.c" -and -not -name ".*") \
 )
endef

LOCAL_SRC_FILES:= $(call all-c-files-under, .)

#LOCAL_C_INCLUDES += \


#LOCAL_SHARED_LIBRARIES := \

LOCAL_LDLIBS :=-llog

LOCAL_MODULE := libUsbCam_jni
LOCAL_MODULE_TAGS := optional

include $(BUILD_SHARED_LIBRARY)
