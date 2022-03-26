//
// Created by WHF on 2022/3/26.
//

#include <jni.h>
#include <time.h>

#include "../utils/log.h"

void getCurrentTime() {
    timeval currentTime;
    gettimeofday(&currentTime, NULL);

    //秒
    long sec = currentTime.tv_sec;
    //微秒
    long usec = currentTime.tv_usec;
    //毫秒
    long msec = sec * 1000 + usec / 1000;

    LOGD("getCurrentTime = %ld", msec);
}

/**
 *
 * int tm_sec;        秒，范围从 0 到 59
 * int tm_min;        分，范围从 0 到 59
 * int tm_hour;       小时，范围从 0 到 23
 * int tm_mday;       一月中的第几天，范围从 1 到 31
 * int tm_mon;        月，范围从 0 到 11
 * int tm_year;       自 1900 年起的年数
 * int tm_wday;       一周中的第几天，范围从 0 到 6
 * int tm_yday;       一年中的第几天，范围从 0 到 365
 * int tm_isdst;      夏令时
 */
void getCurrentTime2() {
    time_t curTimer;
    //获取当前时间戳（秒）
    time(&curTimer);
    LOGD("current timer = %ld", curTimer);

    tm *timer = localtime(&curTimer);
    //从1900年至今的时间
    LOGD("getCurrentTime2 year = %d mon = %d day = %d hour = %d min = %d sec = %d", timer->tm_year,
         timer->tm_mon, timer->tm_mday, timer->tm_hour, timer->tm_min, timer->tm_sec);
}

/**
 * 格式化时间
 * %Y 年份 2012
 * %m 十进制数表示的月份（01-12）
 * %d 一月中的第几天（01-31）
 * %H 24 小时格式的小时（00-23）
 * %M 分（00-59）
 * %S 秒（00-61）
 */
void formatTime() {
    time_t curTimer;
    time(&curTimer);
    tm *timer = localtime(&curTimer);

    char buffer[128];
    strftime(buffer, 128, "%Y-%m-%d %H:%M:%S", timer);
    LOGD("current time = %s", buffer);
}

extern "C" JNIEXPORT void JNICALL
Java_com_whf_nativedemolist_MainActivity_nativeTime(
        JNIEnv *env, jobject /* this */) {
    getCurrentTime();
    getCurrentTime2();
    formatTime();
}