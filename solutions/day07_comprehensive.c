/*
 * Day 7 —— 综合练习 + 复盘
 * 编译：gcc -Wall -std=c17 day07_comprehensive.c -o day07.exe
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utf8_console.h"

/* ========== 1. LeetCode L0118 —— 杨辉三角 ==========
 * 题目：给定 n，打印 n 行杨辉三角
 * 样例输入：5
 * 样例输出：
 *   1
 *   1 1
 *   1 2 1
 *   1 3 3 1
 *   1 4 6 4 1
 */
void pascal_triangle(int n)
{
    int tri[n][n];
    for (int i = 0; i < n; i++)
    {
        tri[i][0] = 1;
        tri[i][i] = 1;
        for (int j = 1; j < i; j++)
        {
            tri[i][j] = tri[i - 1][j - 1] + tri[i - 1][j];
        }
    }
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j <= i; j++)
        {
            printf("%d ", tri[i][j]);
        }
        printf("\n");
    }
}

/* ========== 2. LeetCode L0003 —— 无重复字符的最长子串 ==========
 * 题目：给定字符串，求不含重复字符的最长子串长度（滑动窗口）
 * 样例输入：abcabcbb
 * 样例输出：3 ("abc")
 * 样例输入：bbbbb
 * 样例输出：1 ("b")
 */
int length_of_longest_substring(const char *s)
{
    int n = (int)strlen(s);
    int last[128];
    for (int i = 0; i < 128; i++)
        last[i] = -1;

    int max_len = 0, left = 0;
    for (int right = 0; right < n; right++)
    {
        unsigned char c = (unsigned char)s[right];
        if (last[c] >= left)
        {
            left = last[c] + 1;
        }
        last[c] = right;
        int cur = right - left + 1;
        if (cur > max_len)
            max_len = cur;
    }
    return max_len;
}

/* ========== 3. LeetCode L0088 —— 合并两个有序数组 ==========
 * 题目：nums1 长度 m+n，nums2 长度 n，把 nums2 合并到 nums1 里保持有序
 * 思路：从后往前合并，避免覆盖
 * 样例：nums1=[1,2,3,0,0,0], m=3; nums2=[2,5,6], n=3
 * 结果：nums1=[1,2,2,3,5,6]
 */
void merge_sorted(int *nums1, int m, const int *nums2, int n)
{
    int i = m - 1;
    int j = n - 1;
    int k = m + n - 1;
    while (i >= 0 && j >= 0)
    {
        if (nums1[i] > nums2[j])
        {
            nums1[k--] = nums1[i--];
        }
        else
        {
            nums1[k--] = nums2[j--];
        }
    }
    while (j >= 0)
    {
        nums1[k--] = nums2[j--];
    }
}

int main(void)
{
    init_utf8_console();
    printf("=== LeetCode 118 杨辉三角 (n=6) ===\n");
    pascal_triangle(6);

    printf("\n=== LeetCode 3 最长无重复子串 ===\n");
    printf("abcabcbb -> %d\n", length_of_longest_substring("abcabcbb"));
    printf("bbbbb    -> %d\n", length_of_longest_substring("bbbbb"));
    printf("pwwkew   -> %d\n", length_of_longest_substring("pwwkew"));

    printf("\n=== LeetCode 88 合并有序数组 ===\n");
    int nums1[] = {1, 2, 3, 0, 0, 0};
    int nums2[] = {2, 5, 6};
    merge_sorted(nums1, 3, nums2, 3);
    printf("合并后：");
    for (int i = 0; i < 6; i++)
        printf("%d ", nums1[i]);
    printf("\n");

    return 0;
}