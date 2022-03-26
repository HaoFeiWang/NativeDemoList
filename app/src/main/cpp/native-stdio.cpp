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
    int size1 = fputs("one line START END\n", file);
    int size2 = fputs("two line START\n", file);
    int size3 = fputs("three line", file);
    LOGD("write size1=%d size2=%d size3=%d", size1, size2, size3);
    fclose(file);
}

/**
 * fgets：一次读取一行，保存在 buffer 中
 */
void readLine() {
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

/**
 * fread：从给定流 stream 读取数据到所指向的数组中
 *
 * 参数1 -- 这是指向带有最小尺寸 size*nmemb 字节的内存块的指针。
 * 参数2 -- 这是要读取的每个元素的大小，以字节为单位。
 * 参数3 -- 这是元素的个数，每个元素的大小为 size 字节。
 * 参数4 -- 这是指向 FILE 对象的指针，该 FILE 对象指定了一个输入流。
 *
 * 返回值：成功读取的元素总数会以 size_t 对象返回
 *
 * fseek：设置流 stream 的文件位置为给定的偏移 offset
 * 参数1 -- 这是指向 FILE 对象的指针，该 FILE 对象标识了流。
 * 参数2 -- 这是相对 whence 的偏移量，以字节为单位。
 * 参数3 -- 这是表示开始添加偏移 offset 的位置。
 *
 * 参数3常量
 * SEEK_SET：文件开头
 * SEEK_CUR：文件指针当前位置
 * SEEK_END：文件结尾
 */
void readFile() {
    //打开文件
    FILE *file = fopen("/data/data/com.whf.nativedemolist/cache/readme.txt", "w+");
    if (file == nullptr) {
        LOGD("open error = %s", strerror(errno));
        return;
    }

    fputs("one line START === one line END\n", file);
    fputs("two line START === two line END\n", file);
    fputs("three line START === three line END\n", file);
    fputs("four line START === four line END\n", file);

    char buffer[8];
    fseek(file, 0, SEEK_SET);
    //一次读取一行
    int size;
    while ((size = fread(buffer, 1, 8, file)) != 0) {
        //打印结尾会有乱码，因为buffer中没有\0
        LOGD("read buffer size=%d %s", size, buffer);
    }
    fclose(file);
}

/**
 * fgetc：一次读取一个字符，直接返回
 * feof：判断是否达到文件结束符
 */
void test_feof() {
    //打开文件
    FILE *file = fopen("/data/data/com.whf.nativedemolist/cache/readme.txt", "r");
    if (file == nullptr) {
        LOGD("open error = %s", strerror(errno));
        return;
    }

    char buffer;
    //一次读取一行
    while (1) {
        buffer = fgetc(file);
        if (feof(file)) {
            LOGD("file eof");
            break;
        }
        LOGD("read char = %c", buffer);
    }

    fclose(file);
}

/**
 * ferror：测试给定流 stream 的错误标识符
 */
void test_ferror() {
    //打开文件
    FILE *file = fopen("/data/data/com.whf.nativedemolist/cache/readme.txt", "w");

    //使用只写模式打开，读取会报错
    char buffer = fgetc(file);
    if (ferror(file)) {
        LOGD("read file error1 %s", strerror(errno));
    }

    clearerr(file);
    if (ferror(file)) {
        LOGD("read file error2 %s", strerror(errno));
    }

    fclose(file);
}

/**
 * fgetops：获取流 stream 的当前文件位置
 * ftell: 获取流 stream 的当前文件位置（使用long表示）
 *
 * fsetpos：设置流 stream 的当前文件位置
 * fseek: 设置流 stream 的当前文件位置（使用long表示）
 *
 * fseek参数2：
 * SEEK_SET：从距文件开头 offset 位移量为新的读写位置。
 * SEEK_CUR：以目前的读写位置往后增加 offset 个位移量。
 * SEEK_END：将读写位置指向文件尾后再增加 offset 个位移量。
 *
 * rewind：设置流 stream 的当前文件位置为头部
 */
void test_fgetops() {
    //打开文件
    FILE *file = fopen("/data/data/com.whf.nativedemolist/cache/readme.txt", "w+");

    fpos_t position;
    fgetpos(file, &position);
    //position = 0
    LOGD("test_fgetops fgetpos %ld", position);

    fputs("hello word!", file);
    fgetpos(file, &position);
    //position = 11，已写的字符长度为10
    LOGD("test_fgetops fgetpos %ld", position);

    char buffer[1024];
    memset(buffer, 0, 1024);
    fgets(buffer, 1024, file);
    LOGD("test_fgetops read %s", buffer);

    position = 2;
    fsetpos(file, &position);
    fputs("=Add=", file);

    position = 0;
    fsetpos(file, &position);
    fgets(buffer, 1024, file);
    //输出：he=Add=ord!
    LOGD("test_fgetops read %s", buffer);

    fclose(file);
}

/**
 * 重命名文件
 * 成功返回0，错误返回-1
 */
void test_rename() {
    int result = rename("/data/data/com.whf.nativedemolist/cache/readme.txt",
                        "/data/data/com.whf.nativedemolist/cache/readme2.txt");
    LOGD("rename file result = %d", result);
}

/**
 * 删除文件
 * 成功返回0，错误返回-1
 */
void test_remove() {
    int result = remove("/data/data/com.whf.nativedemolist/cache/readme2.txt");
    LOGD("remove file result = %d", result);
}

/**
 * fscanf：从流中读取格式化字符输入
 * sscanf：从字符串读取格式化字符输入
 * 参数1：源字符串
 * 参数2：格式化模板
 *
 * * 忽略
 * %s：连续字符串，直到遇到一个空格字符（空格字符可以是空白、换行和制表符）
 *
 */
void test_scan() {
    char str[] = "hello word!";
    char value[128];
    sscanf(str, "%*s %s", value);
    //word!
    LOGD("value = %s", value);
}

/**
 * vfprintf：格式化输出到流中
 * sprintf：格式化输入到字符串
 */
void test_sprintf() {
    char value[128];
    sprintf(value, "%s = %d", "Hello", 12);
    LOGD("value = %s", value);
}

extern "C" JNIEXPORT void JNICALL
Java_com_whf_nativedemolist_MainActivity_nativeStdio(
        JNIEnv *env,
        jobject /* this */) {
    writeFile();
    readLine();
    readFile();
    test_feof();
    test_ferror();
    test_fgetops();
    test_rename();
    test_remove();
    test_scan();
    test_sprintf();
}
