/*
 * grammar/06_c99_c11_features.c
 * C99 / C11 / C17 新特性（你的 GCC 16.2.0 完全支持）
 * 这些是 C 标准的「现代化升级」，解决了旧版的很多痛点
 * 刷题/写项目强烈建议用这些特性
 *
 * ==========================================
 * 重点特性速查
 * ==========================================
 * C99:  VLA变长数组、_Bool、stdint.h定宽整数、%zu、//单行注释、inline、restrict
 * C11:  static_assert、_Atomic原子、_Thread_local、_Generic泛型选择、alignof、_Noreturn
 * C17:  __STDC_NO_ATOMICS__检查、#elifdef / #elifndef
 * C23:  更多自动推导、_BitInt、#embed（资源嵌入）
 */

#include <stdio.h>
#include <stdint.h>  /* C99：定宽整数类型 */
#include <stdbool.h> /* C99：bool 类型 */
#include <assert.h>  /* static_assert */
#include <string.h>
#include <stdlib.h>   /* abs / llabs（_Generic 示例用） */
#include <math.h>     /* fabs（_Generic 示例用） */
#include <stdalign.h> /* C11：alignof 宏（标准关键字是 _Alignof） */
#include "utf8_console.h"

/* ================================================================
 * 一、C99 新特性（最常用，刷题强烈建议用）
 * ================================================================
 */

/* 1. VLA 变长数组（C99，GCC 16.2.0 默认支持）
 * 数组长度可以用运行时变量
 * 语法：int arr[n];  （n 是 int 变量）
 * 限制：VLA 必须在栈上（不能是 static / 全局），不能初始化
 * 刷题关联：当你不知道数组要多大的时候，比手动 malloc 更简洁
 */
void demo_vla(int n)
{
    printf("=== C99 VLA 变长数组 ===\n");
    int arr[n]; /* ← C99 新语法，n 是运行时变量 */
    for (int i = 0; i < n; i++)
    {
        arr[i] = i * 2;
    }
    printf("VLA arr[%d]：", n);
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");
    /* 注意：VLA 在栈上，n 太大（>100万）会栈溢出！动态大数组用 malloc */
}

/* 2. _Bool / bool（C99，#include <stdbool.h>）
 * C 之前没有布尔类型，全靠 int 0/1 代替
 * C99 加了 _Bool（底层是 0/1），stdbool.h 提供更简洁的 bool 别名
 */
void demo_bool(void)
{
    printf("\n=== C99 bool ===\n");
    bool is_valid = true; /* 或者 _Bool is_valid = 1; */
    bool is_empty = false;
    printf("true=%d, false=%d\n", true, false); /* 1 0 */
    printf("is_valid=%d, is_empty=%d\n", is_valid, is_empty);

    /* bool 参与运算会自动转 int：true+true=2 */
    printf("true + true = %d\n", true + true);
}

/* 3. stdint.h 定宽整数（C99）
 * 不同平台 int/long/long long 宽度可能不同（比如 Windows 上 long=4字节，Linux 上 long=8字节）
 * 定宽整数保证跨平台一致性：
 *   int8_t / int16_t / int32_t / int64_t    有符号
 *   uint8_t / uint16_t / uint32_t / uint64_t  无符号
 *   intptr_t / uintptr_t  指针大小的整数（能存任意地址）
 *   intmax_t / uintmax_t  最大整数类型
 * 配套宏（inttypes.h）：PRId32 打印 int32_t，PRIu64 打印 uint64_t 等
 */
void demo_stdint(void)
{
    printf("\n=== C99 stdint.h 定宽整数 ===\n");
    int32_t a = 2147483647;              /* 确定是 32 位有符号，范围 -2^31~2^31-1 */
    int64_t big = 9223372036854775807LL; /* 64 位有符号 */
    uint8_t small = 255;                 /* 8 位无符号，0~255（类似 unsigned char） */
    printf("int32_t 最大值 = %d\n", a);
    printf("int64_t 最大值 = %lld\n", big);
    printf("uint8_t = %u\n", small);

    /* SIZE_MAX：size_t 的最大值（unsigned long long 或 unsigned long） */
    printf("sizeof(int32_t) = %zu, sizeof(int64_t) = %zu\n",
           sizeof(int32_t), sizeof(int64_t));
}

/* 4. %zu（C99）
 * 打印 sizeof 结果必须用 %zu（对应 size_t，无符号整数）
 * C89 没有 %zu，只能 (unsigned long) 强转后用 %lu
 * C99 起直接用 %zu 就行（更简洁、更安全）
 */
void demo_format_spec(void)
{
    printf("\n=== C99 %%zu ===\n"); /* 要打印字面量 % 必须写 %% */
    int arr[10];
    printf("sizeof(arr) = %zu\n", sizeof(arr));                       /* C99：直接用 %zu */
    printf("sizeof(arr) = %lu (强转)\n", (unsigned long)sizeof(arr)); /* C89 写法 */
    /* 用 %d 打印 sizeof 结果会触发未定义行为！编译器会警告 */
}

/* 5. // 单行注释（C99）
 * C89 只有块注释（「斜杠+星号」开头，「星号+斜杠」结尾），而且不能嵌套
 * C99 新增了 // 单行注释（和 C++、Java 一样），刷题时写临时注释更方便
 */
// 这就是 C99 单行注释（从 // 一直到行尾都算注释）

/* 块注释依然可以用，而且可以跨多行
 * 但注意块注释不能嵌套！
 */

    /* 6. inline 内联函数（C99）
     * 告诉编译器把函数体直接展开到调用处，省掉函数调用开销
     * 适合非常短、频繁调用的小函数（比如 max/min、getter/setter）
     * 编译器可以选择忽略 inline（只是建议，不是强制）
     */
    static inline int imax(int a, int b)
{
    return a > b ? a : b; /* 编译器会把 imax(3,5) 直接替换成 a > b ? a : b */
}

void demo_inline(void)
{
    printf("\n=== C99 inline ===\n");
    printf("imax(10, 20) = %d（编译器可能直接展开了）\n", imax(10, 20));
}

/* 7. restrict 限定符（C99）
 * 告诉编译器：指针指向的内存区域**不会和其他指针对同一块内存**重叠
 * 编译器能做更激进的优化
 * 标准库 memcpy 的签名就是：void *memcpy(void *restrict dst, const void *restrict src, size_t n);
 * （dst 和 src 不能重叠，重叠要用 memmove）
 * 刷题场景：很少显式用，但知道就行
 */

/* ================================================================
 * 二、C11 新特性（更现代，GCC 16.2.0 默认支持）
 * ================================================================
 */

/* 1. static_assert 编译期断言
 * C89 的 assert 是运行时断言，static_assert 是**编译期**检查
 * 编译时条件为假就直接报错，不会生成可执行文件
 * 刷题场景：检查数组大小、检查类型范围
 */
void demo_static_assert(void)
{
    printf("\n=== C11 static_assert ===\n");

    /* 编译期就检查：int 至少 32 位（在你机器上当然成立） */
    _Static_assert(sizeof(int) >= 4, "int must be at least 32 bits");
    /* C11 还提供了 static_assert 作为别名，需要 #include <assert.h> */

    /* 检查常量表达式 */
    _Static_assert(2 + 2 == 4, "math is broken"); /* 永远成立 */
    printf("static_assert 编译期检查通过\n");
}

/* 2. _Atomic 原子类型（C11 多线程）
 * 保证多线程环境下对该变量的读写是原子的（不会被中断）
 * 刷题暂不需要，但知道 C 语言有多线程支持就行
 */
/* _Atomic int counter = 0;  ← 原子计数器 */

/* 3. _Thread_local 线程局部存储
 * 每个线程有自己独立的变量副本，互不干扰
 */
/* _Thread_local int thread_id = 0; */

/* 4. _Generic 泛型选择（C11）
 * 根据参数类型选择不同的处理函数（类似 C++ 的函数重载，但 C 没有重载）
 * 语法：_Generic((值), 类型1: 表达式1, 类型2: 表达式2, default: 默认表达式)
 */
void demo_generic(void)
{
    printf("\n=== C11 _Generic ===\n");
    double d = 3.14;
    int i = 42;

    /* 根据 d 的类型选择不同的格式化输出 */
    _Generic((d),
        double: printf("double: %f\n", d),
        int: printf("int: %d\n", i),
        default: printf("unknown type\n"));
    /* d 是 double，所以走 double 分支，输出 "double: 3.14" */

    /* 用宏封装一个安全的 abs（处理不同类型） */
#define GEN_ABS(x) _Generic((x), \
    int: abs,                    \
    long long: llabs,            \
    double: fabs)(x)
    printf("GEN_ABS(-5) = %d\n", GEN_ABS(-5));         /* int → abs */
    printf("GEN_ABS(-3.14) = %.2f\n", GEN_ABS(-3.14)); /* double → fabs */
}

/* 5. alignof / _Alignas（C11）
 * alignof 获取类型的对齐要求
 * _Alignas 指定变量的对齐要求
 * 刷题目录：内存对齐、SIMD 指令优化
 */
void demo_alignof(void)
{
    printf("\n=== C11 alignof ===\n");
    printf("alignof(char)    = %zu\n", alignof(char));
    printf("alignof(int)     = %zu\n", alignof(int));
    printf("alignof(double)  = %zu\n", alignof(double));
    printf("alignof(int64_t) = %zu\n", alignof(int64_t));

    /* 手动指定对齐到 16 字节（SIMD 常用） */
    _Alignas(16) float simd_buf[4];
    printf("simd_buf 地址 %p 应该是 16 的倍数\n", (void *)simd_buf);
}

/* 6. _Noreturn 函数（C11）
 * 告诉编译器这个函数**永远不会返回**（比如 exit、abort）
 * 编译器可以优化，也会对调用者的缺失返回警告
 */
/* _Noreturn void fatal_error(const char *msg);  ← 声明一个不返回的函数 */

/* ================================================================
 * 三、输入输出进阶（刷题非常有用的"武器库"）
 * ================================================================
 */

/* 1. sprintf / sscanf（字符串和数字互转的利器）
 * sprintf：把格式化结果写到字符串里（而不是终端）
 * sscanf：从字符串里按格式读数据（而不是终端）
 * 刷题场景：
 *   - 把整数转字符串：char buf[32]; sprintf(buf, "%d", n);
 *   - 把字符串拆成数字：sscanf(line, "%d,%d,%d", &a, &b, &c);
 *   - 数字和字符拼接：sprintf(buf, "%d-%02d-%02d", y, m, d);
 * 安全版：snprintf / sscanf（snprintf 限制写入长度，防溢出）
 */
void demo_sprintf_sscanf(void)
{
    printf("\n=== sprintf / sscanf ===\n");

    /* sprintf：数字转字符串 */
    int year = 2026, month = 10, day = 3;
    char date[32];
    sprintf(date, "%d-%02d-%02d", year, month, day); /* %02d 补零 */
    printf("格式化日期：%s\n", date);                /* 2026-10-03 */

    /* sscanf：从字符串读数据 */
    char input[] = "x=123, y=456, z=789";
    int x, y, z;
    sscanf(input, "x=%d, y=%d, z=%d", &x, &y, &z);
    printf("解析结果：x=%d, y=%d, z=%d\n", x, y, z);

    /* snprintf 安全版（推荐，限制写入长度防溢出） */
    char buf[8];
    int written = snprintf(buf, sizeof(buf), "Hello, C!");
    printf("snprintf 写入 %d 字节（buf 大小 8，实际内容截断了）\n", written);
    printf("buf = \"%s\"\n", buf);
}

/* 2. getchar / putchar（单字符 I/O，刷字符串题高效）
 * getchar() 从标准输入读一个字符（返回 int，因为 EOF 是 -1）
 * putchar(c) 输出一个字符
 * 刷题场景：
 *   - 逐字符读入字符串直到 '\n' 或 EOF
 *   - 统计字符频率
 */
void demo_getchar(void)
{
    printf("\n=== getchar ===\n");
    printf("下面演示逐字符读入（遇到换行停止）：");
    /*
    int c;
    int count = 0;
    while ((c = getchar()) != '\n' && c != EOF) {
        count++;
        putchar(c);
    }
    printf("\n总共读入 %d 个字符\n", count);
    */
    /* 比 scanf("%s", buf) 更灵活：能读空格，能精确控制终止条件 */
}

/* 3. feof / ferror / clearerr
 * feof(fp)：检查是否到达文件末尾
 * ferror(fp)：检查是否有 I/O 错误
 * clearerr(fp)：清除 EOF 和错误标志
 * 刷题场景：读文件时正确判断结束（不要用 !feof(fp) 当循环条件！）
 */

/* ================================================================
 * 四、柔性数组（C99/C11 结构体最后一个成员）
 * ================================================================
 *
 * 【语法】struct { int size; int data[]; };  ← data[] 没有长度！
 * 【用途】在结构体末尾加一个可变长度的数组，配合 malloc 一次性申请内存
 * 【刷题场景】需要存数据量可变的结构体（比如动态字符串、动态数组结构体）
 */
typedef struct
{
    int count;
    int data[]; /* 柔性数组，没有长度，必须是最后一个成员 */
} DynArray;

void demo_flexible_array(void)
{
    printf("\n=== 柔性数组 ===\n");

    /* 一次性申请：结构体头部 + count 个 int */
    int count = 5;
    DynArray *da = (DynArray *)malloc(sizeof(DynArray) + count * sizeof(int));
    if (da == NULL)
        return;
    da->count = count;
    for (int i = 0; i < count; i++)
    {
        da->data[i] = i * 3; /* ← 直接像普通数组一样访问！ */
    }
    printf("DynArray 元素：");
    for (int i = 0; i < da->count; i++)
    {
        printf("%d ", da->data[i]);
    }
    printf("\n");
    printf("malloc 只调用了一次，free 也只需要一次！\n");
    free(da);
    /* 对比：如果用指针成员 int *data;，需要两次 malloc 两次 free */
}

int main(void)
{
    init_utf8_console(); /* 让中文正常显示（详见 utf8_console.h） */
    demo_vla(6);
    demo_bool();
    demo_stdint();
    demo_format_spec();
    demo_inline();
    demo_static_assert();
    demo_generic();
    demo_alignof();
    demo_sprintf_sscanf();
    demo_getchar();
    demo_flexible_array();
    return 0;
}