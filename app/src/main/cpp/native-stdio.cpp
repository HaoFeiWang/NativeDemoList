//
// Created by WHF on 2022/3/13.
//
#include <stdio.h>
#include <jni.h>
#include <errno.h>
#include <string.h>
#include "utils/log.h"


//函数索引
//fopen
//flose
//fgets
//fputs


/**
 * 打开文件参数
 * 读写权限：
 * r：只读，文件必须存在
 * w：只写，如果文件不存在则自动创建，如果文件存在则清空文件内容
 * a：追加，如果文件不存在则自动创建，如果文件存在则追加
 * r+：读写，文件必须存在
 * w+：读写，相当于w和r+，文件不存在则创建，文件存在则清空
 * a+：读写，相当于a和r+，文件不存在则创建，文件存在则追加
 *
 * 打开方式：
 * t: 文本文件，默认方式
 * b：二进制文件
 *
 * 读写权限和读写方式可以组合使用，但是必须将读写方式放在读写权限的中间或者尾部
 */
void writeFile() {
    //打开文件
    FILE *file = fopen("/data/data/com.whf.nativedemolist/cache/readme.txt", "w+");
    if (file == nullptr) {
        LOGD("open error = %s", strerror(errno));
        return;
    }

    //写入buffer
    int size1 = fputs("第一行START 第一行END\n", file);
    int size2 = fputs("第二行START\n", file);
    int size3 = fputs("第三行", file);
    LOGD("write size1=%d size2=%d size3=%d", size1, size2, size3);
    fclose(file);
}

void readFile() {
    //打开文件
    FILE *file = fopen("/data/data/com.whf.nativedemolist/cache/readme.txt", "r");
    if (file == nullptr) {
        LOGD("open error = %s", strerror(errno));
        return;
    }

    char buffer[1024];
    //一次读取一行
    while (fgets(buffer, 1024, file) != NULL) {
        LOGD("read line = %s", buffer);
    }
    fclose(file);
}

extern "C" JNIEXPORT void JNICALL
Java_com_whf_nativedemolist_MainActivity_nativeStdio(
        JNIEnv *env,
        jobject /* this */) {
    writeFile();
    readFile();
}
