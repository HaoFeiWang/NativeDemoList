#include <jni.h>
#include <string>
#include <unistd.h>
#include <sys/inotify.h>

#include "utils/log.h"

extern "C" JNIEXPORT void JNICALL
Java_com_whf_nativedemolist_MainActivity_inotify(
        JNIEnv *env,
        jobject /* this */) {
    int inotifyFd = inotify_init();
    LOGD("inotify fd = %d", inotifyFd);

    int wd = inotify_add_watch(inotifyFd, "/sdcard/", IN_CREATE | IN_DELETE);
    LOGD("inotify add watch = %d", wd);

    char buffer[512];
    int readLength;
    while ((readLength = read(inotifyFd, buffer, 512)) > 0) {
        LOGD("read length = %d", readLength);
        //每次读取到可能为多个事件，需要进行拆分
        int offset = 0;
        int eventSize;
        do {
            struct inotify_event *event = (struct inotify_event *) (buffer + offset);
            if (event->mask & IN_CREATE) {
                LOGD("read create file = %s", event->name);
            } else {
                LOGD("read delete file = %s", event->name);
            }

            //inotify_event的大小是不确定的，因为name大小是不确定的
            eventSize = sizeof(inotify_event) + event->len;

            readLength -= eventSize;
            offset += eventSize;
        } while (readLength > eventSize);
    }
}