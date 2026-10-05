/*
 * Day 2 —— 分支结构 if / switch
 * 编译：gcc -Wall -std=c17 day02_branch.c -o day02.exe
 */

#include <stdio.h>
#include "utf8_console.h"

/* ========== 1. 洛谷 P5715 —— 正数判断 ==========
 * 题目：输入一个整数，判断是否为正数，正数输出 "positive"，
 *       负数输出 "negative"，零输出 "zero"
 * 样例输入：5
 * 样例输出：positive
 */
void solve_p5715(void)
{
    int n;
    scanf("%d", &n);
    if (n > 0)
    {
        printf("positive\n");
    }
    else if (n < 0)
    {
        printf("negative\n");
    }
    else
    {
        printf("zero\n");
    }
}

/* ========== 2. LeetCode —— 绝对值 ==========
 * 题目：不用 abs 函数，返回整数的绝对值
 * 样例输入：-42
 * 样例输出：42
 */
int my_abs(int n)
{
    return n >= 0 ? n : -n;
}

/* ========== 3. 洛谷 P5716 —— 星期几 ==========
 * 题目：输入一个整数 1-7，输出对应的星期（Mon-Sun）
 * 样例输入：3
 * 样例输出：Wed
 */
void solve_p5716(void)
{
    int n;
    scanf("%d", &n);
    switch (n)
    {
    case 1:
        printf("Mon\n");
        break;
    case 2:
        printf("Tue\n");
        break;
    case 3:
        printf("Wed\n");
        break;
    case 4:
        printf("Thu\n");
        break;
    case 5:
        printf("Fri\n");
        break;
    case 6:
        printf("Sat\n");
        break;
    case 7:
        printf("Sun\n");
        break;
    default:
        printf("invalid\n");
        break;
    }
}

/* ========== 4. 洛谷 P5717 —— 三角形分类 ==========
 * 题目：输入三边长度（整数），判断能否构成三角形；
 *       能则判断类型（等边/等腰/普通），不能则输出 "no"
 * 样例输入：3 4 5
 * 样例输出：yes, ordinary
 * 样例输入：2 2 2
 * 样例输出：yes, equilateral
 * 样例输入：1 2 3
 * 样例输出：no
 */
void solve_p5717(void)
{
    int a, b, c;
    scanf("%d %d %d", &a, &b, &c);

    if (a + b <= c || a + c <= b || b + c <= a)
    {
        printf("no\n");
        return;
    }

    if (a == b && b == c)
    {
        printf("yes, equilateral\n");
    }
    else if (a == b || b == c || a == c)
    {
        printf("yes, isosceles\n");
    }
    else
    {
        printf("yes, ordinary\n");
    }
}

int main(void)
{
    init_utf8_console();
    printf("=== P5715 正数判断 ===\n");
    solve_p5715();

    printf("=== LeetCode 绝对值 ===\n");
    printf("|-42| = %d\n", my_abs(-42));
    printf("|10|  = %d\n", my_abs(10));

    printf("=== P5716 星期几 ===\n");
    solve_p5716();

    printf("=== P5717 三角形分类 ===\n");
    solve_p5717();

    return 0;
}