/*
 * Day 5 —— 字符串 / 字符数组
 * 编译：gcc -Wall -std=c17 day05_string.c -o day05.exe
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "utf8_console.h"

/* ========== 1. 洛谷 P1781 —— 字符串长度 ==========
 * 题目：输入一个字符串，输出其长度（不要用 strlen，自己写）
 * 样例输入：hello
 * 样例输出：5
 */
int my_strlen(const char *s)
{
    int n = 0;
    while (s[n] != '\0')
    {
        n++;
    }
    return n;
}

/* ========== 2. LeetCode L0344 —— 反转字符串 ==========
 * 题目：字符数组原地反转
 * 样例输入：{'h','e','l','l','o'}
 * 样例输出：{'o','l','l','e','h'}
 */
void reverse_string(char *s, int len)
{
    int left = 0, right = len - 1;
    while (left < right)
    {
        char t = s[left];
        s[left] = s[right];
        s[right] = t;
        left++;
        right--;
    }
}

/* ========== 3. 洛谷 P1706 —— 统计数字字符 ==========
 * 题目：输入一个字符串，输出其中数字字符的个数
 * 样例输入：abc123def45
 * 样例输出：5
 */
void solve_p1706(void)
{
    char s[256];
    scanf("%255s", s);
    int cnt = 0;
    for (int i = 0; s[i] != '\0'; i++)
    {
        if (s[i] >= '0' && s[i] <= '9')
        {
            cnt++;
        }
    }
    printf("%d\n", cnt);
}

/* ========== 4. LeetCode L0125 —— 验证回文串 ==========
 * 题目：给定字符串，判断是不是回文串（只考虑字母和数字，忽略大小写）
 * 样例输入："A man, a plan, a canal: Panama"
 * 样例输出：true
 * 样例输入："race a car"
 * 样例输出：false
 */
int is_palindrome(const char *s)
{
    int left = 0, right = my_strlen(s) - 1;
    while (left < right)
    {
        while (left < right && !isalnum((unsigned char)s[left]))
            left++;
        while (left < right && !isalnum((unsigned char)s[right]))
            right--;
        if (tolower((unsigned char)s[left]) != tolower((unsigned char)s[right]))
        {
            return 0;
        }
        left++;
        right--;
    }
    return 1;
}

int main(void)
{
    init_utf8_console();
    printf("=== P1781 字符串长度 ===\n");
    char s1[] = "hello world";
    printf("\"%s\" 长度 = %d\n", s1, my_strlen(s1));

    printf("=== LeetCode 344 反转字符串 ===\n");
    char s2[] = "hello";
    reverse_string(s2, my_strlen(s2));
    printf("反转后：%s\n", s2);

    printf("=== P1706 统计数字字符 ===\n");
    solve_p1706();

    printf("=== LeetCode 125 验证回文串 ===\n");
    printf("'%s' -> %s\n",
           "A man, a plan, a canal: Panama",
           is_palindrome("A man, a plan, a canal: Panama") ? "true" : "false");
    printf("'%s' -> %s\n",
           "race a car",
           is_palindrome("race a car") ? "true" : "false");

    return 0;
}