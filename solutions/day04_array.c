/*
 * Day 4 —— 一维数组 + 排序 + 二分
 * 编译：gcc -Wall -std=c17 day04_array.c -o day04.exe
 */

#include <stdio.h>
#include <stdlib.h>
#include "utf8_console.h"

/* ========== 1. 洛谷 P1601 —— 数组求和 ==========
 * 题目：输入 n 和 n 个整数，输出它们的和
 * 样例输入：5 1 2 3 4 5
 * 样例输出：15
 */
void solve_p1601(void)
{
    int n;
    scanf("%d", &n);
    int arr[n];
    long long sum = 0;
    for (int i = 0; i < n; i++)
    {
        scanf("%d", &arr[i]);
        sum += arr[i];
    }
    printf("%lld\n", sum);
}

/* ========== 2. 洛谷 P1428 —— 查找最大元素 ==========
 * 题目：输入 n 和数组，输出最大元素及其下标
 * 样例输入：5 3 7 1 4 9
 * 样例输出：max=9, index=4
 */
void solve_p1428(void)
{
    int n;
    scanf("%d", &n);
    int arr[n];
    int max_idx = 0;
    for (int i = 0; i < n; i++)
    {
        scanf("%d", &arr[i]);
        if (arr[i] > arr[max_idx])
        {
            max_idx = i;
        }
    }
    printf("max=%d, index=%d\n", arr[max_idx], max_idx);
}

/* ========== 3. LeetCode L0912 —— 数组排序（qsort） ==========
 * 题目：给你一个整数数组，将它按升序排序后返回
 * C 语言用系统自带的 qsort，需要写一个 compare 函数
 */
int cmp_int(const void *a, const void *b)
{
    return *(const int *)a - *(const int *)b;
}

void solve_leetcode_912(void)
{
    int arr[] = {5, 2, 8, 1, 9, 3, 7};
    int n = sizeof(arr) / sizeof(arr[0]);

    qsort(arr, n, sizeof(int), cmp_int);

    printf("排序后：");
    for (int i = 0; i < n; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

/* ========== 4. LeetCode L0704 —— 二分查找 ==========
 * 题目：在有序数组中找目标值，返回下标，找不到返回 -1
 * 样例输入：nums = [-1,0,3,5,9,12], target = 9
 * 样例输出：4
 */
int binary_search(int *nums, int nums_size, int target)
{
    int left = 0, right = nums_size - 1;
    while (left <= right)
    {
        int mid = left + (right - left) / 2;
        if (nums[mid] == target)
            return mid;
        if (nums[mid] < target)
            left = mid + 1;
        else
            right = mid - 1;
    }
    return -1;
}

int main(void)
{
    init_utf8_console();
    printf("=== P1601 数组求和 ===\n");
    solve_p1601();

    printf("=== P1428 查找最大元素 ===\n");
    solve_p1428();

    printf("=== LeetCode 912 数组排序 ===\n");
    solve_leetcode_912();

    printf("=== LeetCode 704 二分查找 ===\n");
    int nums[] = {-1, 0, 3, 5, 9, 12};
    int idx = binary_search(nums, 6, 9);
    printf("target 9 下标 = %d\n", idx);

    return 0;
}