/*
 * Day 1 —— 变量、运算符、输入输出
 * 平台：洛谷 / LeetCode
 * 编译：gcc -Wall -std=c17 day01_basics.c -o day01.exe
 * 运行：./day01.exe
 */

#include <stdio.h>
#include "utf8_console.h"

/* ========== 1. 洛谷 P1001 —— A+B Problem ==========
 * 题目：输入两个整数 a, b，输出它们的和
 * 样例输入：1 2
 * 样例输出：3
 */
void solve_p1001(void)
{
    int a, b;
    scanf("%d %d", &a, &b);
    printf("%d\n", a + b);
}

/* ========== 2. 洛谷 P1909 —— 计算(a+b)×c ==========
 * 题目：给定三个整数 a, b, c，求 (a+b)*c
 * 样例输入：2 3 5
 * 样例输出：25
 */
void solve_p1909(void)
{
    int a, b, c;
    scanf("%d %d %d", &a, &b, &c);
    printf("%d\n", (a + b) * c);
}

/* ========== 3. LeetCode L0000 —— Hello World ==========
 * 题目：输出 Hello World
 * （这里展示 printf 的格式化输出）
 */
void solve_leetcode_hello(void)
{
    printf("Hello, World!\n");
    printf("整数：%d 浮点：%.2f 字符：%c\n", 42, 3.14159, 'A');
}

/* ========== 4. 洛谷 P5714 —— 温度转换 ==========
 * 题目：输入一个华氏温度 F，转换为摄氏温度 C，保留 3 位小数
 * 公式：C = 5(F-32)/9
 * 样例输入：100
 * 样例输出：37.778
 */
void solve_p5714(void)
{
    double f, c;
    scanf("%lf", &f);
    c = 5.0 * (f - 32.0) / 9.0;
    printf("%.3f\n", c);
}

/* ========== main：逐个测试 ========== */
int main(void)
{
    init_utf8_console();
    printf("=== P1001 A+B ===\n");
    solve_p1001();

    printf("=== P1909 (a+b)*c ===\n");
    solve_p1909();

    printf("=== LeetCode Hello World ===\n");
    solve_leetcode_hello();

    printf("=== P5714 温度转换 ===\n");
    solve_p5714();

    return 0;
}