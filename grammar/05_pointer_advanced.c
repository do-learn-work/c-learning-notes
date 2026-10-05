/*
 * grammar/05_pointer_advanced.c
 * 指针进阶：const、二级指针、指针数组、函数指针、字符串指针数组
 * 刷题关联：Day6-Day7 进阶、LeetCode 中等题
 *
 * ==========================================
 * 语法核心
 * ==========================================
 * 1. const 修饰指针的 4 种写法（区分「const 在前还是 * 在前」）
 * 2. 二级指针 int **p  → 指向指针的指针
 * 3. 指针数组 int *arr[n]  → 存指针的数组
 * 4. 数组指针 int (*p)[n]  → 指向数组的指针（和指针数组长得很像！）
 * 5. 函数指针 int (*fp)(int, int)  → 指向函数的指针
 * 6. 字符串指针数组 char *strs[]  → 存多个字符串，刷题处理多组输入常用
 */

#include <stdio.h>
#include <stdlib.h> /* malloc / free / qsort */
#include <string.h>
#include "utf8_console.h"

/* ================================================================
 * 一、const 修饰指针的 4 种写法（必掌握，刷题只读参数必加！）
 * ================================================================
 *
 * 【口诀】const 在 * 左边 → 指针指向的内容不可改
 *        const 在 * 右边 → 指针本身不可改
 *        都有 → 都不可改
 *
 * ① const int *p;       ← 常量指针（指向常量的指针）
 *    *p 不能改，但 p 本身能改（可以换个地址指）
 *    例：p 指向某个只读数据，保证函数不会误改
 *
 * ② int * const p;      ← 指针常量（指针本身是常量）
 *    p 不能改（不能换指向），但 *p 能改
 *    例：p 绑定到某个固定地址，保证永远指向它
 *
 * ③ const int * const p; ← 双重常量
 *    p 不能改，*p 也不能改
 *
 * ④ const int * const p = NULL;
 *    同上，但初始化为 NULL
 *
 * 【刷题高频】函数参数加 const 保证只读：
 *   int sum(const int *arr, int n);  ← 承诺函数不会修改 arr 的内容
 *   char *strcmp(const char *a, const char *b);  ← 标准库就是这么写的
 */

void demo_const_pointer(void)
{
    printf("=== const 修饰指针 ===\n");
    int a = 10, b = 20;

    /* ① 常量指针（const 在 * 左边） */
    const int *p1 = &a;
    /* *p1 = 100;  ← 错！不能改 *p1 指向的值 */
    p1 = &b; /* 对！可以改 p1 本身的指向 */
    printf("p1 现在指向 b=%d\n", *p1);

    /* ② 指针常量（const 在 * 右边） */
    int *const p2 = &a; /* 必须初始化！ */
    *p2 = 100;          /* 对！可以改 *p2 指向的值（a 变成 100） */
    /* p2 = &b;     ← 错！不能改 p2 本身的指向 */
    printf("a=%d（通过 *p2=100 修改的）\n", a);

    /* ③ 双重常量 */
    const int *const p3 = &a;
    /* *p3 = 200;  ← 错 */
    /* p3 = &b;    ← 错 */
}

/* ================================================================
 * 二、二级指针（指向指针的指针）
 * ================================================================
 *
 * 【语法】int **pp;
 *  pp 是指向 int* 类型的指针
 *  *pp 得到一个 int*（指向某个 int）
 *  **pp 得到那个 int 的值
 *
 * 【内存模型】
 *    a (int)  →  p (int*)  →  pp (int**)
 *    10         &a            &p
 *
 * 【刷题场景】
 *   1. 函数里要修改调用者的指针本身（不是指针指向的值）
 *      比如：函数里 malloc 了空间，想让外面的指针也指向这块空间
 *   2. 处理字符串指针数组（char **argv）
 */

/* 前置声明：C 语言必须「先声明后使用」，定义都在调用者之后 */
void alloc_buffer(int **out);
int cmp_int(const void *a, const void *b);

void demo_double_pointer(void)
{
    printf("\n=== 二级指针 ===\n");

    int a = 42;
    int *p = &a;
    int **pp = &p;

    printf("a = %d\n", a);
    printf("p = %p, *p = %d\n", (void *)p, *p);
    printf("pp = %p, *pp = %p, **pp = %d\n",
           (void *)pp, (void *)*pp, **pp);

    /* 二级指针作为函数参数：让函数能修改外面的指针 */
    int *buf = NULL;
    alloc_buffer(&buf); /* 传 &buf，函数内能修改 buf 的指向 */
    printf("buf = %p, buf[0] = %d\n", (void *)buf, buf[0]);
    free(buf);
    buf = NULL;
}

/* 二级指针参数的典型用法：函数内 malloc，让外面的指针也指向 */
void alloc_buffer(int **out)
{
    int *p = (int *)malloc(5 * sizeof(int));
    for (int i = 0; i < 5; i++)
    {
        p[i] = i * i;
    }
    *out = p; /* 通过二级指针修改调用者的指针变量 */
}

/* ================================================================
 * 三、指针数组 vs 数组指针（长得像，但本质完全不同！）
 * ================================================================
 *
 * 【指针数组】int *arr[n];
 *   → 数组里存的是 n 个指针
 *   → 内存：arr[0], arr[1], ..., arr[n-1] 都是地址
 *   → 刷题目录：存多个字符串（char *strs[100]）
 *              存动态申请的多行数据（int *rows[m]，每行长度可不同）
 *
 * 【数组指针】int (*p)[n];
 *   → 指向数组的指针（p 是一个指针，指向长度为 n 的 int 数组）
 *   → 内存：p 本身是一个地址，指向一个数组的首地址
 *   → 刷题目录：把二维数组当参数传递
 *
 * 【区分口诀】看 * 和 [] 谁先和变量名结合
 *   int *arr[10];   → arr 先和 [10] 结合，是数组，存 int* → 指针数组
 *   int (*arr)[10]; → * 和 arr 先结合，是指针，指向 int[10] → 数组指针
 */

void demo_pointer_array_vs_array_pointer(void)
{
    printf("\n=== 指针数组 vs 数组指针 ===\n");

    /* 指针数组：存多个字符串（LeetCode 处理多组字符串输入常用） */
    char *names[] = {"张三", "李四", "王五"};
    int count = sizeof(names) / sizeof(names[0]);
    printf("指针数组 names[%d]：", count);
    for (int i = 0; i < count; i++)
    {
        printf("%s ", names[i]); /* names[i] 是 char*，直接打印 */
    }
    printf("\n");

    /* 指针数组：存不同长度的动态数组（二维数组每行长度不同） */
    int *rows[3];
    rows[0] = (int *)malloc(2 * sizeof(int)); /* 第 0 行 2 个元素 */
    rows[1] = (int *)malloc(5 * sizeof(int)); /* 第 1 行 5 个元素 */
    rows[2] = (int *)malloc(3 * sizeof(int)); /* 第 2 行 3 个元素 */
    for (int i = 0; i < 2; i++)
        rows[0][i] = i;
    for (int i = 0; i < 5; i++)
        rows[1][i] = i * 10;
    for (int i = 0; i < 3; i++)
        rows[2][i] = i * 100;
    printf("锯齿数组（每行长度不同）：\n");
    printf("  rows[0]: ");
    for (int i = 0; i < 2; i++)
        printf("%d ", rows[0][i]);
    printf("\n  rows[1]: ");
    for (int i = 0; i < 5; i++)
        printf("%d ", rows[1][i]);
    printf("\n  rows[2]: ");
    for (int i = 0; i < 3; i++)
        printf("%d ", rows[2][i]);
    printf("\n");
    for (int i = 0; i < 3; i++)
        free(rows[i]);

    /* 数组指针：指向二维数组的某一行 */
    int matrix[2][3] = {{1, 2, 3}, {4, 5, 6}};
    int (*row_ptr)[3] = &matrix[0]; /* row_ptr 指向 matrix 的第 0 行 */
    printf("数组指针指向的行：");
    for (int j = 0; j < 3; j++)
    {
        printf("%d ", (*row_ptr)[j]); /* (*row_ptr) 是一行，再按下标访问 */
    }
    printf("\n");
    row_ptr++; /* 跳过一行！row_ptr 原来指向 matrix[0]，现在指向 matrix[1] */
    printf("row_ptr++ 后指向：");
    for (int j = 0; j < 3; j++)
    {
        printf("%d ", (*row_ptr)[j]);
    }
    printf("\n");
}

/* ================================================================
 * 四、函数指针（指向函数的指针）
 * ================================================================
 *
 * 【语法模板】
 *   返回类型 (*函数指针名)(参数类型列表);
 *   int (*fp)(int, int);  ← fp 是指向「两个 int 参数、返回 int」的函数指针
 *
 * 【赋值】fp = 函数名;  （函数名本身就是函数的入口地址）
 * 【调用】fp(实参...);  （和调用普通函数一模一样）
 *
 * 【刷题场景】
 *   1. qsort 的比较函数指针（Day4 已用，但可能没注意是函数指针）
 *   2. 回调：把某个函数当参数传给另一个函数
 *   3. 策略模式：同一个逻辑，用不同函数指针实现不同策略
 */

int add(int a, int b) { return a + b; }
int mul(int a, int b) { return a * b; }

void demo_function_pointer(void)
{
    printf("\n=== 函数指针 ===\n");

    /* 声明 + 赋值 */
    int (*calc)(int, int) = add;
    printf("calc(3, 5) = %d（用的是 add）\n", calc(3, 5));

    calc = mul; /* 可以换指向另一个函数 */
    printf("calc(3, 5) = %d（换成 mul 了）\n", calc(3, 5));

    /* qsort 的比较函数就是函数指针的典型应用（Day4） */
    int arr[] = {5, 2, 8, 1, 9};
    int n = 5;
    qsort(arr, n, sizeof(int), cmp_int); /* cmp_int 是函数指针 */
    printf("qsort 后：");
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

/* qsort 要求的比较函数原型 */
int cmp_int(const void *a, const void *b)
{
    return *(const int *)a - *(const int *)b;
}

int main(void)
{
    init_utf8_console(); /* 让中文正常显示（详见 utf8_console.h） */
    demo_const_pointer();
    demo_double_pointer();
    demo_pointer_array_vs_array_pointer();
    demo_function_pointer();
    return 0;
}