#include <jni.h>
#include <string>
#include <unistd.h>
#include <sys/inotify.h>
#include <sys/epoll.h>

#include "utils/log.h"

int addWatchInotify(const char *path) {
    int inotifyFd = inotify_init();
    LOGD("inotify fd = %d", inotifyFd);

    int wd = inotify_add_watch(inotifyFd, path, IN_CREATE | IN_DELETE);
    LOGD("inotify add watch = %d", wd);

    return inotifyFd;
}

void readInotify(int readLength, char *buffer) {
    //每次读取到可能为多个事件，需要进行拆分
    int length = readLength;
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

        length -= eventSize;
        offset += eventSize;
    } while (length > eventSize);
}

void readInotifyLoop(int inotifyFd) {
    char buffer[512];
    int readLength;
    while ((readLength = read(inotifyFd, buffer, 512)) > 0) {
        LOGD("read length = %d", readLength);
        readInotify(readLength, buffer);
    }
}

void readInotifyOnce(int inotifyFd) {
    char buffer[512];
    int readLength = read(inotifyFd, buffer, 512);
    LOGD("read length = %d", readLength);
    readInotify(readLength, buffer);
}

extern "C" JNIEXPORT void JNICALL
Java_com_whf_nativedemolist_MainActivity_inotify(
        JNIEnv *env,
        jobject /* this */) {
    int inotifyFd = addWatchInotify("/sdcard/");
    readInotifyLoop(inotifyFd);
}

void addInotifyToEpoll(int epollFd, const char *path) {
    int inotifyFd = addWatchInotify(path);

    struct epoll_event event;
    event.events = EPOLLIN | EPOLLET;
    event.data.fd = inotifyFd;

    int addResult = epoll_ctl(epollFd, EPOLL_CTL_ADD, inotifyFd, &event);
    LOGD("epoll add result = %d", addResult);
}

extern "C" JNIEXPORT void JNICALL
Java_com_whf_nativedemolist_MainActivity_epoll(
        JNIEnv *env,
        jobject /* this */) {
    int epollFd = epoll_create1(0);
    LOGD("epoll fd = %d", epollFd);

    addInotifyToEpoll(epollFd, "/sdcard/");
    addInotifyToEpoll(epollFd, "/sdcard/Music/");
    addInotifyToEpoll(epollFd, "/sdcard/Download/");

    struct epoll_event events[1024];
    while (true) {
        //epoll监听到的个数
        int count = epoll_wait(epollFd, events, 1024, -1);
        LOGD("epoll_wait count = %d", count);
        if (count == -1) {
            return;
        }

        for (int i = 0; i < count; ++i) {
            int listenFd = events[i].data.fd;
            LOGD("listen Fd = %d", listenFd);
            readInotifyOnce(listenFd);
        }
    }
}