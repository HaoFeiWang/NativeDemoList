//
// Created by WHF on 2022/3/13.
//
#include <stdlib.h>
#include <jni.h>
#include "utils/log.h"
#include <string.h>

/**
 * 字符串转换为 float 类型，忽略非数值类型的字符
 */
void test_atof() {
    char *str = "12.3abc";
    float value = atof(str);
    LOGD("test_atof = %f", value);
}

/**
 * 字符串转换为 int 类型，忽略非数值类型的字符
 */
void test_atoi() {
    char *str = "123abc";
    int value = atoi(str);
    //value = 123
    LOGD("test_atoi = %d", value);
}

/**
 * 字符串转换为 long 类型，忽略非数值类型的字符
 */
void test_atol() {
    char *str = "123";
    long int value = atol(str);
    LOGD("test_atoi = %ld", value);
}

/**
 * 字符串转换为 double 类型，并将非数值类型字符串存储在参数2中
 */
void test_strtod() {
    const char *str1 = "123abc";
    char *str2;
    double value = strtod(str1, &str2);
    //str2=abc value= 123.000000
    LOGD("test_strtod str2=%s value=%lf", str2, value);
}

/**
 * 分配、释放内存
 */
void test_malloc() {
    //分配内存，但不会初始化为0
    void *buffer = malloc(128);
    LOGD("buffer addr = %p", buffer);

    //分配内存，会初始化为0，item_count为分配多少个单元，item_size为每个单元的字节大小
    void *buffer2 = calloc(128, 1);
    LOGD("buffer2 addr = %p", buffer2);

    //重新分配内存大小，原有内存中的内容不变，但是位置可能发生变化（如果数据后面的字节不够，会拷贝到新位置）
    void *buffer3 = realloc(buffer, 129);
    LOGD("buffer3 addr = %p", buffer3);

    free(buffer2);
    free(buffer3);
}

/**
 * 获取环境变量
 */
void test_getenv() {
    char *path = getenv("PATH");
    LOGD("path env = %s", path);
}

/**
 * 执行 shell 命令
 */
void test_system() {
    int result = system("ls /sdcard/ -l");
    LOGD("test_system result = %d", result);
}

/**
 * 取绝对值
 * abs：int类型
 * labs：long类型
 */
void test_abs() {
    int num = -5;
    int result = abs(num);
    LOGD("test_abs result = %d", result);
}

/**
 * 获取 [0,RAND_MAX] 之间的伪随机数
 */
void test_random() {
    LOGD("test_random max = %d", RAND_MAX);
    int value1 = random();
    int value2 = random();
    int value3 = random();
    int value4 = random();
    int value5 = random();
    LOGD("test_random value1=%d value2=%d value3=%d value4=%d value5=%d", value1, value2, value3,
         value4, value5);
}

extern "C" JNIEXPORT void JNICALL
Java_com_whf_nativedemolist_MainActivity_nativeStdlib(
        JNIEnv *env,
        jobject /* this */) {
    test_atof();
    test_atoi();
    test_atol();
    test_strtod();
    test_malloc();
    test_getenv();
    test_system();
    test_abs();
    test_random();
}