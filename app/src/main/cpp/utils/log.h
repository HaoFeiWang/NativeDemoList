//
// Created by 王浩飞 on 2021/12/18.
//

#include <android/log.h>

#ifndef NATIVEDEMOLIST_LOG_H
#define NATIVEDEMOLIST_LOG_H

#define LOG_TAG "NativeDemo_Native"

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

#endif //NATIVEDEMOLIST_LOG_H
