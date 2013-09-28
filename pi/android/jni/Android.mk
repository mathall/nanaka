LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := Game
LOCAL_SRC_FILES := $(GAME_LIB)
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := Nanaka
LOCAL_SRC_FILES := $(NANAKA_LIB_DIR)/libnanaka.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := Nanaka_png
LOCAL_SRC_FILES := $(NANAKA_LIB_DIR)/libnanaka_png.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := Nanaka_json_parser
LOCAL_SRC_FILES := $(NANAKA_LIB_DIR)/libnanaka_json_parser.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := Nanaka_zip
LOCAL_SRC_FILES := $(NANAKA_LIB_DIR)/libnanaka_zip.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := Nanaka_ogg
LOCAL_SRC_FILES := $(NANAKA_LIB_DIR)/libnanaka_ogg.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := Nanaka_tremor
LOCAL_SRC_FILES := $(NANAKA_LIB_DIR)/libnanaka_tremor.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := jniNanaka
LOCAL_CFLAGS    := -Werror -Wall
LOCAL_SRC_FILES := Nanaka.cpp GLContextManagerImpl.cpp JNIHelper.cpp FileManagerImpl.cpp Application.cpp
LOCAL_LDLIBS    := -lGLESv2 -lGLESv1_CM -lz -landroid -lEGL -lOpenSLES
LOCAL_WHOLE_STATIC_LIBRARIES := Nanaka Nanaka_png Nanaka_json_parser Nanaka_zip Nanaka_ogg Nanaka_tremor Game
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../../
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../../nanaka
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../../nanaka/external/libzip/src
include $(BUILD_SHARED_LIBRARY)
