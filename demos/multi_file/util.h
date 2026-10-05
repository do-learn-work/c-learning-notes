/*
 * demos/multi_file/util.h
 * ============================================================
 * 【知识点】头文件怎么写
 *   - 头文件只放**声明**（函数声明、宏、类型定义）
 *   - 不放**定义**（变量定义、函数实现）
 *   - 用 #ifndef/#define/#endif 防止重复包含
 *
 * 【对应的 .c 文件】util.c（放函数实现）
 * 【怎么编译】和 main.c 一起编译：
 *   gcc -Wall -std=c17 demos/multi_file/main.c demos/multi_file/util.c -o multi.exe
 * ============================================================
 */

#ifndef DEMOS_UTIL_H /* 如果没定义这个宏 */
#define DEMOS_UTIL_H /* 现在定义它 */

/* 头文件守卫的作用：
 * 如果 main.c 和 util.c 都 #include "util.h"
 * 第二次包含时 DEMOS_UTIL_H 已经定义了，#ifndef 块内的内容被跳过
 * 避免重复声明导致编译错误
 */

/* ---------- 宏定义（可以放在头文件里） ---------- */
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

/* ---------- 类型定义（可以放在头文件里） ---------- */
typedef struct
{
    double x;
    double y;
} Point;

/* ---------- 函数声明（放头文件里，实现放 .c 文件里） ---------- */
/* 格式：返回类型 函数名(参数类型列表);  ← 参数名可以省略，但写上更清晰 */

/* 数学工具函数 */
int add(int a, int b);
int gcd(int a, int b);
double distance(Point p1, Point p2);

/* 数组工具函数 */
int array_sum(const int *arr, int n);
void array_print(const int *arr, int n);

/* ---------- 全局变量声明（用 extern！） ---------- */
/* 头文件里只能放 extern 声明，不能放定义！ */
extern int g_demo_counter; /* 声明：这个全局变量在某个 .c 文件里定义了 */
/* 如果写成 int g_demo_counter; ← 定义！多个 .c 包含会导致链接重复定义错误 */

#endif /* DEMOS_UTIL_H  ← #ifndef 的结束 */