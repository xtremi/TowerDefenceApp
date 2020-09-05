# Copyright (C) 2010 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH := $(call my-dir)
AGK_PATH := D:\AGK\AGK_libs2

### copy the AGK lib into the objs directory for linking ###
include $(CLEAR_VARS)
LOCAL_MODULE    := AGKBullet
LOCAL_SRC_FILES := $(AGK_PATH)/platform/android/jni/$(TARGET_ARCH_ABI)/libAGKBullet.a
LOCAL_EXPORT_C_INCLUDES := $(AGK_PATH)/common/include \
    $(AGK_PATH)/bullet \
    $(AGK_PATH)/bullet/BulletCollision/CollisionShapes
include $(PREBUILT_STATIC_LIBRARY) 

include $(CLEAR_VARS)
LOCAL_MODULE    := AGKAssimp
LOCAL_SRC_FILES := $(AGK_PATH)/platform/android/jni/$(TARGET_ARCH_ABI)/libAGKAssimp.a
LOCAL_EXPORT_C_INCLUDES := $(AGK_PATH)/common/include \
    $(AGK_PATH)/bullet \
    $(AGK_PATH)/bullet/BulletCollision/CollisionShapes
include $(PREBUILT_STATIC_LIBRARY) 

include $(CLEAR_VARS)
LOCAL_MODULE    := AGKCurl
LOCAL_SRC_FILES := $(AGK_PATH)/platform/android/jni/$(TARGET_ARCH_ABI)/libAGKCurl.a
LOCAL_EXPORT_C_INCLUDES := $(AGK_PATH)/common/include \
    $(AGK_PATH)/bullet \
    $(AGK_PATH)/bullet/BulletCollision/CollisionShapes
include $(PREBUILT_STATIC_LIBRARY) 

include $(CLEAR_VARS)
LOCAL_MODULE    := AGKAndroid
LOCAL_SRC_FILES := $(AGK_PATH)/platform/android/jni/$(TARGET_ARCH_ABI)/libAGKAndroid.a
LOCAL_EXPORT_C_INCLUDES := $(AGK_PATH)/common/include \
    $(AGK_PATH)/bullet \
    $(AGK_PATH)/bullet/BulletCollision/CollisionShapes
LOCAL_STATIC_LIBRARIES := AGKBullet AGKAssimp
include $(PREBUILT_STATIC_LIBRARY) 

include $(CLEAR_VARS)
LOCAL_MODULE    := analytics
LOCAL_SRC_FILES := $(AGK_PATH)/FirebaseSDK/libs/android/$(TARGET_ARCH_ABI)/c++/libanalytics.a
# LOCAL_STATIC_LIBRARIES := AGKBullet AGKAssimp
include $(PREBUILT_STATIC_LIBRARY) 

include $(CLEAR_VARS)
LOCAL_MODULE    := app
LOCAL_SRC_FILES := $(AGK_PATH)/FirebaseSDK/libs/android/$(TARGET_ARCH_ABI)/c++/libapp.a
# LOCAL_STATIC_LIBRARIES := AGKBullet AGKAssimp
include $(PREBUILT_STATIC_LIBRARY) 

### build the app ###
include $(CLEAR_VARS)

# the name of the library referenced from the AndroidManifest.xml file
LOCAL_MODULE    := android_player

# agk includes folder
LOCAL_C_INCLUDES := $(AGK_PATH)/common/include \
                    $(AGK_PATH)/renderer \
					$(AGK_RELA) \

# app source files, must be relative to the jni folder
LOCAL_SRC_FILES := main.c \
                AnimationJSONReader.cpp \
                Building.cpp \
                Bullet.cpp \
                ChainBullet.cpp \
                AreaSpawnBullet.cpp \
                ChainElement.cpp \
                CircleBullet.cpp \
                FollowBullet.cpp \
                BuildingController.cpp \
                Cell.cpp \
                CellSelectionMenu.cpp \
                Core.cpp \
                colours.cpp \
                GameHUD.cpp \
                GamePlay.cpp \
                global.cpp \
                InfoFrame.cpp \
                InfoFrameList.cpp \
                JSONReader.cpp \
                logger.cpp \
                Map.cpp \
                MediaBank.cpp \
                MediaJSONReader.cpp \
                MenuUI.cpp \
                Mob.cpp \
                MobController.cpp \
                MobEffect.cpp \
                MobEffectController.cpp \
                MobJSONReader.cpp \
                OnScreenLogger.cpp \
                PlayerStatsHUDframe.cpp \
                template.cpp \
                Tower.cpp \
				TowerAim.cpp \
				TowerAOE.cpp \
				TowerAimless.cpp \
				TowerBlock.cpp \
				TowerMultiDir.cpp \
                TowerDirectionMenu.cpp \
                TowerJSONReader.cpp \
                WaveController.cpp \
                WaveJSONReader.cpp \
                WaveHUDframe.cpp \
                NextWaveButton.cpp \
                ../../../../../AGK_RELA/UI/AgkImage.cpp \
                ../../../../../AGK_RELA/UI/AgkSprite.cpp \
                ../../../../../AGK_RELA/UI/AnimatedSprite.cpp \
                ../../../../../AGK_RELA/UI/AnimatedSpriteAndText.cpp \
                ../../../../../AGK_RELA/UI/AnimatedTextSprite.cpp \
                ../../../../../AGK_RELA/UI/Sprite.cpp \
                ../../../../../AGK_RELA/UI/SpriteAndText.cpp \
                ../../../../../AGK_RELA/UI/SpriteAnimation.cpp \
                ../../../../../AGK_RELA/UI/TextSprite.cpp \
                ../../../../../AGK_RELA/UI/NumberTextSprite.cpp \
                ../../../../../AGK_RELA/UI/UIButton.cpp \
                ../../../../../AGK_RELA/UI/UIButtonTypes.cpp \
                ../../../../../AGK_RELA/UI/UIFrame.cpp \
		../../../../../AGK_RELA/ViewController/ViewController.cpp \

# included system libraries
LOCAL_LDLIBS    := -lm -llog -landroid -lEGL -lGLESv2 -lz -lOpenSLES

# included user libraris
LOCAL_STATIC_LIBRARIES := AGKAndroid AGKBullet AGKAssimp AGKCurl analytics app android_native_app_glue

# define IDE_ANDROID (for AGK) and use O3 optimizations
LOCAL_CFLAGS += -DIDE_ANDROID -O3 -fsigned-char
LOCAL_CPPFLAGS += -fexceptions -std=c++11 -frtti

# use arm instead of thumb instructions
LOCAL_ARM_MODE := arm

include $(BUILD_SHARED_LIBRARY)

### build the native support library ###
$(call import-module,android/native_app_glue)