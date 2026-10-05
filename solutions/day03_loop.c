/*
 * Day 3 —— 循环结构 for / while / do-while
 * 编译：gcc -Wall -std=c17 day03_loop.c -o day03.exe
 */

#include <stdio.h>
#include "utf8_console.h"

/* ========== 1. 洛谷 P5718 —— 求和 ==========
 * 题目：输入一个整数 n，求 1 + 2 + ... + n
 * 样例输入：10
 * 样例输出：55
 */
void solve_p5718(void)
{
    int n, sum = 0;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++)
    {
        sum += i;
    }
    printf("%d\n", sum);
}

/* ========== 2. 洛谷 P5719 —— 找最大数 ==========
 * 题目：输入 n 和 n 个整数，输出其中最大值
 * 样例输入：5 3 1 4 1 5
 * 样例输出：5
 */
void solve_p5719(void)
{
    int n, x, max;
    scanf("%d", &n);
    scanf("%d", &max);
    for (int i = 1; i < n; i++)
    {
        scanf("%d", &x);
        if (x > max)
            max = x;
    }
    printf("%d\n", max);
}

/* ========== 3. 洛谷 P1035 —— 奇数求和 ==========
 * 题目：计算 1 + 3 + 5 + ... + n（n 为奇数）
 * 样例输入：9
 * 样例输出：25
 */
void solve_p1035(void)
{
    int n, sum = 0;
    scanf("%d", &n);
    for (int i = 1; i <= n; i += 2)
    {
        sum += i;
    }
    printf("%d\n", sum);
}

/* ========== 4. LeetCode L0509 —— 斐波那契 ==========
 * 题目：F(0)=0, F(1)=1, F(n)=F(n-1)+F(n-2)，求 F(n)
 * 样例输入：10
 * 样例输出：55
 */
int fib(int n)
{
    if (n <= 1)
        return n;
    int a = 0, b = 1;
    for (int i = 2; i <= n; i++)
    {
        int t = a + b;
        a = b;
        b = t;
    }
    return b;
}

int main(void)
{
    init_utf8_console();
    printf("=== P5718 求和 ===\n");
    solve_p5718();

    printf("=== P5719 找最大数 ===\n");
    solve_p5719();

    printf("=== P1035 奇数求和 ===\n");
    solve_p1035();

    printf("=== LeetCode 509 斐波那契 ===\n");
    for (int i = 0; i <= 10; i++)
    {
        printf("F(%d) = %d\n", i, fib(i));
    }

    return 0;
}