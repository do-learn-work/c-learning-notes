/*
 * demos/memory_model.c
 * ============================================================
 * 【知识点】C 进程的内存布局（栈 / 堆 / 全局 / BSS / 代码段）
 *   理解这个才能写出「不崩」的代码
 *
 * 【怎么编译 + 运行】
 *   gcc -Wall -std=c17 memory_model.c -o memory.exe
 *   ./memory.exe
 *
 * 【额外实验】
 *   开启 ASan 检测内存错误：
 *   gcc -fsanitize=address -g memory_model.c -o memory_asan.exe
 *
 *   用 GDB 单步调试（看变量在内存里的值）：
 *   gcc -g memory_model.c -o memory_gdb.exe
 *   gdb ./memory_gdb.exe
 *
 * 【预期输出】
 *   你会看到 5 类变量的内存地址，从高到低排列：
 *     代码段(main)  → 最高
 *     全局/静态变量  → 次高
 *     BSS未初始化    → 中
 *     栈(局部变量)   → 较低
 *     堆(malloc)     → 最低
 *   栈向下增长（每次函数调用地址变小）
 *   堆向上增长（每次 malloc 地址变大）
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "utf8_console.h"

/* ---------- 全局变量（数据段 + BSS 段） ---------- */
int g_initialized = 42;       /* 数据段：有初值的全局变量 */
int g_uninitialized;          /* BSS 段：未初始化的全局变量（自动清零） */
static int s_static_var = 99; /* 数据段 + internal linkage（只在当前文件可见） */

/* ---------- 代码段（函数本身在代码段） ---------- */
int main(void); /* 前向声明，下面 demo_address_map 会打印 main 的地址 */

/* 嵌套调用加深栈深度，观察栈向下增长 */
void stack_level_3(int *prev_addr)
{
    int local = 300;
    printf("  Level 3: &local=%p  (比上一层地址低，栈向下增长)\n", (void *)&local);
    (void)prev_addr;
}
void stack_level_2(void)
{
    int local = 200;
    printf("  Level 2: &local=%p\n", (void *)&local);
    stack_level_3(&local);
}
void stack_level_1(void)
{
    int local = 100;
    printf("  Level 1: &local=%p\n", (void *)&local);
    stack_level_2();
}

/* 演示：函数返回局部变量的地址（经典错误！） */
/* 取消注释下面的 bad_func，用 ASan 编译运行就能看到 use-after-return 错误 */
/*
int *bad_func(void) {
    int x = 999;
    return &x;   // 危险！x 在栈上，函数返回后栈帧释放
}
*/

/* ============================================================
 * 实验 1：打印各类变量的地址，建立内存直觉
 * ============================================================ */
void demo_address_map(void)
{
    printf("============================================================\n");
    printf(" 实验 1：内存地址分布图（你的机器上地址顺序可能略有不同）\n");
    printf("============================================================\n");

    /* 代码段：函数入口地址 */
    printf("\n【代码段】函数入口（只读）\n");
    printf("  main          = %p\n", (void *)main);
    printf("  demo_address  = %p\n", (void *)demo_address_map);

    /* 全局/静态变量 */
    printf("\n【数据段 / BSS 段】全局变量（程序整个生命周期存在）\n");
    printf("  g_initialized(带初值)  = %p  值=%d\n", (void *)&g_initialized, g_initialized);
    printf("  g_uninitialized(BSS)    = %p  值=%d  (自动清零)\n", (void *)&g_uninitialized, g_uninitialized);
    printf("  s_static_var(static)   = %p  值=%d\n", (void *)&s_static_var, s_static_var);

    /* 局部变量（栈上） */
    int local_a = 111;
    int local_b = 222;
    printf("\n【栈 Stack】局部变量（函数返回就释放）\n");
    printf("  local_a = %p  值=%d\n", (void *)&local_a, local_a);
    printf("  local_b = %p  值=%d\n", (void *)&local_b, local_b);
    printf("  ⚠ 栈地址通常比堆地址高，且每次函数调用地址递减\n");

    /* 堆变量 */
    int *heap_a = (int *)malloc(sizeof(int));
    int *heap_b = (int *)malloc(sizeof(int));
    *heap_a = 333;
    *heap_b = 444;
    printf("\n【堆 Heap】malloc 申请（手动管理）\n");
    printf("  heap_a = %p  值=%d\n", (void *)heap_a, *heap_a);
    printf("  heap_b = %p  值=%d\n", (void *)heap_b, *heap_b);
    printf("  ⚠ 堆地址通常比栈低，每次 malloc 地址递增\n");
    printf("  ⚠ 忘记 free = 内存泄漏！\n");
    free(heap_a);
    free(heap_b);

    /* 字符串常量（只读段，有些平台合并到代码段） */
    char *str_lit = "Hello, C!";
    printf("\n【只读段】字符串字面量（不能修改！）\n");
    printf("  \"Hello, C!\" = %p\n", (void *)str_lit);
    /* str_lit[0] = 'X'; ← 未定义行为！字符串常量只读 */

    printf("\n【类型对比】不同类型的地址差异\n");
    char char_arr[8];
    int int_arr[8];
    double dbl_arr[8];
    printf("  char[8]   = %p  每个元素占 1 字节\n", (void *)char_arr);
    printf("  int[8]    = %p  每个元素占 4 字节\n", (void *)int_arr);
    printf("  double[8] = %p  每个元素占 8 字节\n", (void *)dbl_arr);
}

/* ============================================================
 * 实验 2：观察栈向下增长（递归嵌套越深，地址越小）
 * ============================================================ */
void demo_stack_growth(void)
{
    printf("\n============================================================\n");
    printf(" 实验 2：栈向下增长演示（函数嵌套调用）\n");
    printf("============================================================\n");
    printf("每一层函数调用，局部变量地址应该比上一层小\n");
    printf("（栈从高地址往低地址生长）\n\n");
    stack_level_1();
    printf("\n如果反过来（地址变大），那你的栈可能是向上增长的（罕见但存在）\n");
}

/* ============================================================
 * 实验 3：观察堆向上增长（连续 malloc，地址递增）
 * ============================================================ */
void demo_heap_growth(void)
{
    printf("\n============================================================\n");
    printf(" 实验 3：堆向上增长演示（连续 malloc 10 块）\n");
    printf("============================================================\n");

    const int N = 10;
    int *ptrs[N];
    for (int i = 0; i < N; i++)
    {
        ptrs[i] = (int *)malloc(64); /* 每次申请 64 字节 */
        printf("  malloc #%2d: %p", i + 1, (void *)ptrs[i]);
        if (i > 0 && ptrs[i] > ptrs[i - 1])
        {
            printf("  ← 比上一个大 ✓ (堆向上增长)");
        }
        printf("\n");
    }
    printf("\n依次 free（先申请的后释放，和栈相反）：");
    for (int i = 0; i < N; i++)
    {
        free(ptrs[i]);
    }
    printf("全部释放完成\n");
}

/* ============================================================
 * 实验 4：指针到底是什么（地址的地址）
 * ============================================================ */
void demo_pointer_intuition(void)
{
    printf("\n============================================================\n");
    printf(" 实验 4：指针的本质（地址变量）\n");
    printf("============================================================\n");

    int a = 42;
    int *p = &a;   /* p 存了 a 的地址 */
    int **pp = &p; /* pp 存了 p 的地址 */

    printf("  a     = %d      &a     = %p\n", a, (void *)&a);
    printf("  p     = %p     &p     = %p      *p  = %d\n",
           (void *)p, (void *)&p, *p);
    printf("  pp    = %p     &pp    = %p      *pp = %p   **pp = %d\n",
           (void *)pp, (void *)&pp, (void *)*pp, **pp);
    printf("\n");
    printf("  p == &a  → %s\n", (void *)p == (void *)&a ? "true ✓" : "false");
    printf("  *p == a  → %s\n", *p == a ? "true ✓" : "false");
    printf("  **pp == a → %s\n", **pp == a ? "true ✓" : "false");

    /* 通过指针修改变量 */
    *p = 100;
    printf("\n  *p = 100 后 → a = %d （指针能修改它指向的变量）\n", a);
}

int main(void)
{
    init_utf8_console();
    demo_address_map();
    demo_stack_growth();
    demo_heap_growth();
    demo_pointer_intuition();

    printf("\n============================================================\n");
    printf(" 全部实验完成！\n");
    printf(" 试试开启 ASan：gcc -fsanitize=address memory_model.c -o m.exe\n");
    printf(" 试试 GDB 调试：gcc -g memory_model.c -o m.exe && gdb ./m.exe\n");
    printf("============================================================\n");
    return 0;
}