/*
 * demos/multi_file/main.c
 * ============================================================
 * 【知识点】主程序文件
 *   #include "util.h" 引入自己写的头文件
 *   链接时需要把 util.c 也编译进去
 *
 * 【编译命令】
 *   # 方式 1：一步搞定（最简单）
 *   gcc -Wall -std=c17 demos/multi_file/main.c demos/multi_file/util.c -o multi.exe
 *
 *   # 方式 2：分别编译 .o 再链接（大项目这么做，改一个文件不用全重编）
 *   gcc -c demos/multi_file/main.c -o demos/multi_file/main.o
 *   gcc -c demos/multi_file/util.c -o demos/multi_file/util.o
 *   gcc demos/multi_file/main.o demos/multi_file/util.o -o multi.exe
 *
 * 【运行】
 *   ./multi.exe
 *
 * 【常见链接错误】
 *   undefined reference to `add'
 *   → 忘了把 util.c 加进编译命令！加上就好
 *
 *   multiple definition of `g_demo_counter'
 *   → 在头文件里写了变量**定义**（不是 extern 声明）
 *   → 解决：头文件里用 extern 声明，在某个 .c 文件里写一次定义
 *
 *   编译带 sqrt 的代码可能需要加 -lm 链接数学库（有些平台）
 *   gcc main.c util.c -o multi.exe -lm
 * ============================================================
 */

#include "util.h" /* 引入自己的头文件（用双引号） */
#include <stdio.h>
#include "utf8_console.h"

int main(void)
{
    init_utf8_console();
    printf("============================================================\n");
    printf(" 多文件编译演示\n");
    printf(" main.c + util.c + util.h 三个文件一起编译\n");
    printf("============================================================\n\n");

    /* 使用 util.h 里声明的宏 */
    printf("MAX(10, 20) = %d\n", MAX(10, 20));
    printf("MIN(10, 20) = %d\n", MIN(10, 20));

    /* 使用 util.h 里声明的函数 */
    printf("\nadd(3, 5) = %d\n", add(3, 5));
    printf("gcd(48, 18) = %d (调用了 %d 次)\n", gcd(48, 18), g_demo_counter);
    printf("gcd(100, 35) = %d (累计调用 %d 次)\n", gcd(100, 35), g_demo_counter);
    /* g_demo_counter 是 util.c 里定义的全局变量，main.c 通过 extern 声明访问它 */

    /* 使用 util.h 里定义的结构体 */
    Point a = {0, 0};
    Point b = {3, 4};
    printf("\n点 (%.1f,%.1f) 到 (%.1f,%.1f) 的距离 = %.2f\n",
           a.x, a.y, b.x, b.y, distance(a, b));

    /* 数组工具 */
    int nums[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int n = ARRAY_SIZE(nums); /* 宏：算数组长度 */
    printf("\n数组：");
    array_print(nums, n);
    printf("sum = %d\n", array_sum(nums, n));

    printf("\n============================================================\n");
    printf(" 编译命令回顾：\n");
    printf("   gcc -Wall main.c util.c -o multi.exe\n");
    printf(" 或分开编译（大项目常用）：\n");
    printf("   gcc -c main.c -o main.o\n");
    printf("   gcc -c util.c -o util.o\n");
    printf("   gcc main.o util.o -o multi.exe\n");
    printf("============================================================\n");

    return 0;
}