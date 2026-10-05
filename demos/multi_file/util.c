/*
 * demos/multi_file/util.c
 * ============================================================
 * 【知识点】函数实现放在 .c 文件里
 *   每个 .c 文件是一个独立的编译单元
 *   编译时先分别编译 .c → .o，最后链接 .o → 可执行文件
 *
 * 【怎么单独编译这个文件】
 *   gcc -c demos/multi_file/util.c -o demos/multi_file/util.o
 * （然后可以用 objdump -d util.o 看生成的机器码）
 *
 * 【内部链接函数】
 *   static void helper(void); ← 只在当前 .c 文件可见
 *   别的 .c 文件不能调用它（不需要在头文件声明）
 * ============================================================
 */

#include "util.h" /* 自己的头文件用 "xxx.h"，系统头文件用 <xxx.h> */
#include <stdio.h>
#include <math.h>

/* ---------- 全局变量的**定义**（只能在一个 .c 文件里定义一次！） ---------- */
int g_demo_counter = 0; /* ← 定义！头文件里的 extern 声明对应这里 */
/* 如果头文件里写了 int g_demo_counter;（没 extern），
 * 而 main.c 和 util.c 都包含了 util.h，
 * 链接时会报 "multiple definition of g_demo_counter" 错误！ */

/* ---------- 内部链接函数（static，只在本文件可见） ---------- */
static int abs_int(int x)
{
    /* 这个 helper 函数只在 util.c 内部使用，不需要暴露给外部 */
    /* 用 static 修饰 = internal linkage，别的 .c 看不到 */
    return x >= 0 ? x : -x;
}

/* ---------- 头文件里声明的函数，在这里实现 ---------- */

int add(int a, int b)
{
    return a + b;
}

/* 辗转相除法求最大公约数 */
int gcd(int a, int b)
{
    a = abs_int(a); /* 调用内部 helper（abs_int 是 static，只能本文件调用） */
    b = abs_int(b);
    while (b != 0)
    {
        int t = b;
        b = a % b;
        a = t;
    }
    g_demo_counter++; /* 全局计数器，每次调用 gcd 加 1 */
    return a;
}

double distance(Point p1, Point p2)
{
    double dx = p1.x - p2.x;
    double dy = p1.y - p2.y;
    return sqrt(dx * dx + dy * dy);
}

int array_sum(const int *arr, int n)
{
    int sum = 0;
    for (int i = 0; i < n; i++)
    {
        sum += arr[i];
    }
    return sum;
}

void array_print(const int *arr, int n)
{
    printf("[");
    for (int i = 0; i < n; i++)
    {
        if (i > 0)
            printf(", ");
        printf("%d", arr[i]);
    }
    printf("]\n");
}