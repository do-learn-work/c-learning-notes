/*
 * demos/stdlib_deep.c
 * ============================================================
 * 【知识点】标准库的深层用法（刷题高效武器）
 *   - 缓冲机制（printf 为什么不立即输出）
 *   - errno 错误处理（C 语言没有异常，靠 errno）
 *   - sprintf / sscanf（字符串和数字互转）
 *   - fgets / getchar（安全读输入）
 *   - memset / memcpy / memmove（内存操作）
 *   - qsort 到底怎么实现的（不是纯快排！）
 *
 * 【编译 + 运行】
 *   gcc -Wall -std=c17 stdlib_deep.c -o stdlib.exe
 *   ./stdlib.exe
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include "utf8_console.h"

/* ============================================================
 * 1. 缓冲机制：printf 什么时候真正输出？
 * ============================================================
 *
 * C 语言的 I/O 是**带缓冲**的（为了性能，减少系统调用次数）：
 *   - stdout 指向终端：**行缓冲**（遇到 \n 才刷新到终端）
 *   - stdout 指向文件：**全缓冲**（缓冲区满才刷新）
 *   - stderr：**无缓冲**（立即输出，因为错误信息不能等）
 *
 * 后果：printf("Hello") 后面没 \n，程序卡住或崩溃时你看不到输出！
 * 解决：fflush(stdout); 或 printf("Hello\n");
 */
void demo_buffer(void)
{
    printf("--- 1. 缓冲机制 ---\n");

    printf("这行末尾有 \\n，会立即输出\n");
    printf("这行末尾没有 \\n..."); /* ← 运行时你可能看不到这行！ */
    fflush(stdout);                /* ← 强制刷新，现在能看到了 */
    printf(" (fflush 后才出现)\n");

    /* 设置 stdin 为无缓冲（不推荐，但可以知道） */
    /* setvbuf(stdout, NULL, _IONBF, 0);  ← 设为无缓冲 */

    /* 对比 stderr（无缓冲，立即输出） */
    fprintf(stderr, "这行用 stderr，无缓冲，立即出现\n\n");
}

/* ============================================================
 * 2. errno + perror：C 语言的"异常处理"
 * ============================================================
 *
 * C 语言没有 try-catch，库函数失败时会：
 *   - 返回错误值（通常是 NULL、-1、EOF）
 *   - 设置全局变量 errno 为具体的错误码
 *
 * 用法：
 *   1. 调用库函数
 *   2. 检查返回值是否为错误
 *   3. 用 perror() 根据 errno 打印错误原因
 *      或 strerror(errno) 拿到错误描述字符串
 */
void demo_errno(void)
{
    printf("--- 2. errno 错误处理 ---\n");

    /* 尝试打开不存在的文件 */
    FILE *fp = fopen("不存在的文件_xyz123.txt", "r");
    if (fp == NULL)
    {
        /* perror 会自动读取 errno 并打印：
         * "打开文件失败: No such file or directory" */
        perror("打开文件失败");

        /* strerror 拿到描述字符串，可以自己拼格式 */
        printf("错误码: %d, 描述: %s\n", errno, strerror(errno));

        /* 常见 errno 值：
         *   ENOENT (2)  - 文件不存在
         *   EACCES (13) - 权限不够
         *   ENOMEM (12) - 内存不足（malloc 返回 NULL 时）
         *   EINVAL (22) - 参数无效
         */
    }
    printf("\n");
}

/* ============================================================
 * 3. sprintf / snprintf / sscanf：字符串处理利器
 * ============================================================
 *
 * sprintf：格式化输出到字符串（不是终端）
 *   危险！不检查目标缓冲区大小，可能溢出
 *   安全版：snprintf(buf, sizeof(buf), fmt, ...)
 *
 * sscanf：从字符串里按格式读数据（不是从终端）
 *   刷题场景：把一行文本拆成多个字段
 */
void demo_sprintf_sscanf(void)
{
    printf("--- 3. sprintf / sscanf ---\n");

    /* sprintf：数字转字符串（刷题常用！） */
    int n = 12345;
    char buf[32];
    sprintf(buf, "%d", n);
    printf("数字 %d 转字符串 \"%s\" (长度 %zu)\n", n, buf, strlen(buf));

    /* sprintf：格式化拼接 */
    int year = 2026, month = 10, day = 3;
    sprintf(buf, "%04d-%02d-%02d", year, month, day);
    printf("日期拼接: %s\n", buf); /* 2026-10-03 */

    /* snprintf：安全版，限制写入长度 */
    char small[16];
    int written = snprintf(small, sizeof(small), "Hello, C!");
    printf("snprintf 写入 %d 字节 (small 有 16 字节)\n", written);
    printf("small = \"%s\"\n", small);

    /* sscanf：从字符串解析数据（刷题高频！） */
    char csv_line[] = "Alice,85,92,78"; /* CSV 格式 */
    char name[32];
    int score1, score2, score3;
    sscanf(csv_line, "%31[^,],%d,%d,%d", name, &score1, &score2, &score3);
    /* %31[^,] 意思：最多读 31 个字符，遇到逗号停止 */
    printf("\n解析 CSV: name=\"%s\", 分数=%d/%d/%d\n",
           name, score1, score2, score3);

    /* sscanf：解析结构化文本 */
    char log[] = "[2026-10-03 14:30:00] ERROR code=404 user=bob";
    int error_code;
    char user[16];
    sscanf(log, "%*[^ ] %*[^ ] ERROR code=%d user=%15s", &error_code, user);
    /* %*[^ ] 意思：读但跳过（* = suppress），直到遇到空格 */
    printf("解析日志: error_code=%d, user=%s\n\n", error_code, user);
}

/* ============================================================
 * 4. fgets / getchar：安全读输入
 * ============================================================
 *
 * scanf("%s", buf) 有两个问题：
 *   - 遇到空格就停止（读 "Hello World" 只会读到 "Hello"）
 *   - 不检查缓冲区大小（输入太长就溢出）
 *
 * fgets：读一整行（包括空格），自动限制长度（安全！）
 *   char buf[64];
 *   fgets(buf, sizeof(buf), stdin);   ← 最多读 63 字符，自动加 '\0'
 *   注意：fgets 会把末尾的 '\n' 也读进去，有时需要手动去掉
 *
 * getchar：逐字符读（刷字符串题高效）
 *   返回 int（因为 EOF 是 -1，需要用 int 存）
 */
void demo_fgets(void)
{
    printf("--- 4. fgets / getchar ---\n");

    /* fgets 读整行 */
    printf("请输入一行文字（可以包含空格）: ");
    /* fgets(buf, sizeof(buf), stdin); */
    /* printf("你输入的是: \"%s\" (长度 %zu)\n", buf, strlen(buf)); */

    /* fgets 会把 '\n' 也读进去，手动去掉 */
    /* if (buf[strlen(buf) - 1] == '\n') {
        buf[strlen(buf) - 1] = '\0';
    } */

    /* getchar 逐字符读，统计输入的字符类型 */
    printf("\n用 getchar 演示逐字符处理（输入一行，统计数字/字母/其他）:\n");
    /*
    int c;
    int digits = 0, letters = 0, others = 0;
    while ((c = getchar()) != '\n' && c != EOF) {
        if (isdigit(c)) digits++;
        else if (isalpha(c)) letters++;
        else others++;
    }
    printf("  数字: %d, 字母: %d, 其他: %d\n\n", digits, letters, others);
    */
    printf("（需要你在终端输入才能演示，取消上面注释试试）\n\n");
}

/* ============================================================
 * 5. memset / memcpy / memmove：内存操作三件套
 * ============================================================
 *
 * memset：把一块内存全部填成某个字节（常用于清零）
 *   memset(arr, 0, sizeof(arr));   ← 清零数组
 *   memset(s, '\0', sizeof(s));   ← 清零字符串
 *   注意：memset 按字节填！memset(arr, 1, sizeof(arr)) 不会把 int 设成 1！
 *
 * memcpy：拷贝内存块（目标和源**不能重叠**！）
 *   memcpy(dst, src, n);   ← 安全版 strcpy，指定拷贝长度
 *
 * memmove：拷贝内存块（目标和源**可以重叠**）
 *   和 memcpy 功能一样，但处理重叠时更慢但更安全
 */
void demo_memops(void)
{
    printf("--- 5. 内存操作 ---\n");

    /* memset 清零数组 */
    int arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    printf("清零前 arr[0..4]: ");
    for (int i = 0; i < 5; i++)
        printf("%d ", arr[i]);
    memset(arr, 0, sizeof(arr)); /* 按字节清零整个数组 */
    printf("\n清零后 arr[0..4]: ");
    for (int i = 0; i < 5; i++)
        printf("%d ", arr[i]);
    printf("\n");

    /* memset 的坑：按字节填，不是按元素填！ */
    int arr2[5];
    memset(arr2, 0xFF, sizeof(arr2)); /* 把每个字节都填 0xFF */
    printf("\nmemset(arr, 0xFF) 后 arr2[0] = %d (不是 255！是 -1)\n", arr2[0]);
    /* 因为 int 是 4 字节：0xFFFFFFFF = -1（补码） */

    /* memcpy vs memmove */
    char dst[20] = "Hello, World!";
    printf("\n拷贝前 dst = \"%s\"\n", dst);
    /* 把 "World" 移到前面（源和目标重叠！） */
    memmove(dst, dst + 7, 5); /* 安全处理重叠 */
    dst[5] = '\0';
    printf("memmove 重叠拷贝后 dst = \"%s\"\n", dst);
    /* 如果用 memcpy 处理重叠，结果是未定义的（可能错） */
    printf("\n");
}

/* ============================================================
 * 6. qsort 的内部实现（不是纯快排！）
 * ============================================================
 *
 * 你写的 qsort(arr, n, sizeof(int), cmp_int) 内部是什么？
 *   - 大多数实现是**快排 + 插入排序的混合**
 *   - 当分区小于某个阈值（通常 16）时，切换到插入排序
 *     （小数组用插入排序比快排更快，因为快排有递归开销）
 *   - 有的实现（如 glibc）还加了 **堆排** 做最坏情况兜底
 *     （如果快排退化到 O(n²)，自动切换堆排保证 O(n log n)）
 *
 * 比较函数的坑：
 *   int cmp(const void *a, const void *b)
 *   返回值规则：a<b 返回负数，a==b 返回 0，a>b 返回正数
 *   错误写法：return *(int*)a - *(int*)b;  ← 两个大数相减可能溢出！
 *   正确写法：return (*(int*)a > *(int*)b) - (*(int*)a < *(int*)b);
 */
int cmp_int_safe(const void *a, const void *b)
{
    int x = *(const int *)a;
    int y = *(const int *)b;
    return (x > y) - (x < y); /* 避免整数溢出 */
}

void demo_qsort_deep(void)
{
    printf("--- 6. qsort 深层 ---\n");

    int arr[] = {38, 27, 43, 3, 9, 82, 10};
    int n = sizeof(arr) / sizeof(arr[0]);

    printf("排序前: ");
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");

    qsort(arr, n, sizeof(int), cmp_int_safe);

    printf("排序后: ");
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n\n");
}

int main(void)
{
    init_utf8_console();
    demo_buffer();
    demo_errno();
    demo_sprintf_sscanf();
    demo_fgets();
    demo_memops();
    demo_qsort_deep();
    return 0;
}