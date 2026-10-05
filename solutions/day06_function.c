/*
 * Day 6 —— 函数 + 指针 + 结构体
 * 编译：gcc -Wall -std=c17 day06_function.c -o day06.exe
 */

#include <stdio.h>
#include "utf8_console.h"

/* ========== 1. 洛谷 P1028 —— 最大值函数 ==========
 * 题目：自己写一个 max 函数，返回两个整数中较大的
 */
int my_max(int a, int b)
{
    return a > b ? a : b;
}

/* ========== 2. 洛谷 P1307 —— 数字反转（指针交换） ==========
 * 题目：输入一个整数 n，输出反转后的数字（考虑负数）
 * 样例输入：-123
 * 样例输出：-321
 */
int reverse_int(int n)
{
    int neg = (n < 0);
    if (neg)
        n = -n;
    int rev = 0;
    while (n > 0)
    {
        rev = rev * 10 + n % 10;
        n /= 10;
    }
    return neg ? -rev : rev;
}

/* ========== 3. LeetCode L0001 —— 两数之和 ==========
 * 题目：在数组中找到和为 target 的两个数，返回它们的下标
 * 样例输入：nums=[2,7,11,15], target=9
 * 样例输出：[0,1]
 */
void two_sum(const int *nums, int n, int target, int *out_i, int *out_j)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            if (nums[i] + nums[j] == target)
            {
                *out_i = i;
                *out_j = j;
                return;
            }
        }
    }
    *out_i = -1;
    *out_j = -1;
}

/* ========== 4. 学生成绩统计（结构体练习） ==========
 * 题目：用 struct 存 3 个学生的姓名和成绩，输出平均分和最高分
 */
typedef struct
{
    char name[32];
    int score;
} Student;

void student_stats(const Student *students, int count)
{
    int total = 0, max_score = students[0].score, max_idx = 0;
    for (int i = 0; i < count; i++)
    {
        total += students[i].score;
        if (students[i].score > max_score)
        {
            max_score = students[i].score;
            max_idx = i;
        }
    }
    printf("平均分：%.2f\n", (double)total / count);
    printf("最高分：%s (%d)\n", students[max_idx].name, max_score);
}

int main(void)
{
    init_utf8_console();
    printf("=== P1028 最大值函数 ===\n");
    printf("max(3, 7) = %d\n", my_max(3, 7));
    printf("max(-5, -2) = %d\n", my_max(-5, -2));

    printf("=== P1307 数字反转 ===\n");
    printf("reverse(123)   = %d\n", reverse_int(123));
    printf("reverse(-123)  = %d\n", reverse_int(-123));
    printf("reverse(120)   = %d\n", reverse_int(120));

    printf("=== LeetCode 1 两数之和 ===\n");
    int nums[] = {2, 7, 11, 15};
    int i, j;
    two_sum(nums, 4, 9, &i, &j);
    printf("下标 [%d, %d]\n", i, j);

    printf("=== 学生成绩统计 ===\n");
    Student stu[] = {
        {"张三", 85},
        {"李四", 92},
        {"王五", 78}};
    student_stats(stu, 3);

    return 0;
}