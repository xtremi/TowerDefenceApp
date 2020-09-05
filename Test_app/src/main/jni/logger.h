#pragma once
#include "agk.h"
#ifndef WIN_64_BUILD
#include <android/log.h>

#define  LOG_TAG    "TestAppLogger" 

#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,    LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,     LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,     LOG_TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,    LOG_TAG, __VA_ARGS__)


#else

#define LOGE(...)  
#define LOGW(...) 
#define LOGI(...) 
#define LOGD(...) 


#endif