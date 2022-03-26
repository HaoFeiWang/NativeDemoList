//
// Created by WHF on 2022/3/12.
//
#include <string.h>
#include <jni.h>
#include <errno.h>
#include "../utils/log.h"

/**
 * 搜索某个字符，返回该字符之后的字符串，如果没有找到返回NULL
 * 有 memchr、strchr 两个函数
 * memchr：从内存中搜索，遇到 \0 不会终止
 * strchr：从字符串中搜索，遇到 \0 会终止，因为需要不断判断\0，所以性能也没有 memchr 高
 * strrchr: 从后往前搜索
 */
void test_chr() {
    const char *value = "HelloWord!";
    const char target = 'o';
    char *result;
    LOGD("test_memchr result = %s", result);

    //参数3代表在前N个字节中搜索
    result = (char *) memchr(value, target, 10);
    //result = oWord!
    LOGD("test_memchr result = %s", result);

    result = (char *) memchr(value, target, 2);
    //result = (null)
    LOGD("test_memchr result = %s", result);

    const char *strResult = strchr(value, target);
    //oWord!
    LOGD("test_chr strResult = %s", strResult);

    //从后往前搜索
    const char *strResult2 = strrchr(value, target);
    //ord!
    LOGD("test_chr strResult2 = %s", strResult2);
}

/**
 * strcspn（搜寻的是字符）: 检索字符串 str1 开头连续有几个字符都不含字符串 str2 中的字符
 * strspn（搜寻的是字符）: 检索字符串 str1 中第一个不在字符串 str2 中出现的字符下标
 * strpbrk（搜寻的是字符）: 依次检验字符串 str1 中的字符，当被检验字符在字符串 str2 中也包含时，则停止检验，并返回该字符位置
 * strstr（搜寻的是字符串）: 在字符串 str1 中查找第一次出现字符串 str2（不包含空结束字符）的位置。
 */
void test_strcspn() {
    char *str1 = "abcdef";
    char *str2 = "dc";
    int result = strcspn(str1, str2);
    //result = 2
    LOGD("test_strcspn reslut = %d", result);

    int result2 = strspn(str1, str2);
    //result2 = 0
    LOGD("test_strcspn result2 = %d", result2);

    char *result3 = strpbrk(str1, str2);
    //result3 = cdef
    LOGD("test_strcspn strpbrk = %s", result3);

    char *result4 = strstr(str1, str2);
    LOGD("test_strcspn result4 = %s", result4);
}

/**
 * 比较两个字符串大小
 *
 * str1 -- 指向内存块的指针。
 * str2 -- 指向内存块的指针。
 * n -- 要被比较的字节数。
 *
 * 原理：从首字符开始，只要不相等就直接返回结果
 *
 * 返回值：
 * 大于0：str1 大于 str2
 * 等于0：str1 等于 str2
 * 小于0：str1 小于 str2
 *
 * 两种操作
 * memcmp
 * strcmp
 */
void test_cmp() {
    // \0的ANSI码为0，0个ANSI码为48
    LOGD("test_cmp a=%d b=%d c=%d A=%d B=%d C=%d 0=%d 0=%d", 'a', 'b', 'c', 'A', 'B', 'C', '\0',
         '0');
    const char *str1 = "A0B";
    const char *str2 = "A\0cC";
    int result = memcmp(str1, str2, 3);
    LOGD("test_cmp memcmp = %d ", result);

    int result2 = strcmp(str1, str2);
    LOGD("test_cmp strcmp = %d ", result2);

    int result3 = strncmp(str1, str2, 3);
}

/**
 * 从 str2 复制 N 个字节到 str1
 *
 * 参数1(det)：目标字符数组
 * 参数2(src)：源字符指针起始地址
 * 参数3(copy_amount)：复制的字节数
 *
 * 返回值：str1的指针
 *
 * memmove 比 memcpy 更安全，如果目标区域和源区域有重叠的话
 * memmove 能够保证源串在被覆盖之前将重叠区域的字节拷贝到目标区域中
 * 如果目标区域与源区域没有重叠，则和 memcpy 函数功能相同
 *
 * memcpy：内存拷贝
 * strcpy：字符串拷贝
 */
void test_cpy() {
    char str1[100] = "abcdef";
    char *str2 = "g\0hijkl";
    memcpy(str1, str2, 3);
    //str1 为 ghcdef
    LOGD("test_cpy memcpy str1 = %s", str1);

    char str3[100] = "abcdef";
    //从str2的第2个字符开始复制，复制2个字节
    memcpy(str3, str2 + 2, 3);
    //str3 为 ijcdef
    LOGD("test_cpy memcpy str3 = %s", str3);

    char str4[100] = "abcdef";
    strcpy(str4, str2);
    LOGD("test_cpy strcpy str4 = %s", str4);

    char str5[100] = "abcdef";
    strncpy(str5, str2, 3);
    LOGD("test_cpy strcpy str5 = %s", str5);
}

/**
 * 复制字符 c（一个无符号字符）到参数 str 所指向的字符串的前 n 个字符
 * 参数1：要填充的内存块。
 * 参数2：要被设置的值。该值以 int 形式传递，但是函数在填充内存块时是使用该值的无符号字符形式。
 * 参数3：要被设置为该值的字符数
 */
void test_set() {
    char str1[100] = "abcdef";
    memset(str1, 'h', 2);
    //str1 = hhcdef
    LOGD("testMemset str1 = %s", str1);
}

/**
 * 拼接字符串：将 str2 拼接到 str1 末尾
 */
void test_strcat() {
    char str1[100] = "abcdef";
    char *str2 = "ghijkl";
    strcat(str1, str2);
    //str1 = abcdefghijkl
    LOGD("test_strcat str1 = %s", str1);

    char str3[100] = "abcdef";
    char *str4 = "ghijkl";
    //参数3为拼接3个字符
    strncat(str3, str4, 3);
    //str3 = abcdefghi
    LOGD("test_strcat str3 = %s", str3);
}

/**
 * 从内部数组中搜索错误号 errno，并返回一个指向错误消息字符串的指针
 * strerror 生成的错误字符串取决于开发平台和编译器。
 */
void test_strerror() {
    //Success
    errno = 0;
    char *error = strerror(errno);
    LOGD("test_strerror = %s", error);

    //Operation not permitted
    errno = 1;
    char *error1 = strerror(errno);
    LOGD("test_strerror = %s", error1);

    //No such file or directory
    errno = 2;
    char *error2 = strerror(errno);
    LOGD("test_strerror = %s", error2);

    //No such process
    errno = 3;
    char *error3 = strerror(errno);
    LOGD("test_strerror = %s", error3);
}

/**
 * 计算字符串 str 的长度
 */
void test_strlen() {
    char *str = "abcdef";
    int length = strlen(str);
    LOGD("length = %d", length);
}

/**
 * 分割字符串
 */
void test_strtok() {
    char str1[100] = "www.baidu.com";
    const char str2[2] = ".";
    char *result = strtok(str1, str2);
    LOGD("test_strtok result = %s", result);

    while (result != NULL) {
        result = strtok(NULL, str2);
        LOGD("test_strtok result = %s", result);
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_whf_nativedemolist_MainActivity_nativeString(
        JNIEnv *env,
        jobject /* this */) {
    test_chr();
    test_cmp();
    test_cpy();
    test_set();
    test_strcat();
    test_strcspn();
    test_strerror();
    test_strlen();
    test_strtok();
}