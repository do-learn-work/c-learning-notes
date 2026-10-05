/*
 * demos/bug_demo.c
 * ============================================================
 * 【知识点】C 语言经典 bug 模式 + 怎么用 ASan / GDB 定位
 *   故意写了几个有问题的函数，注释里告诉你怎么检测
 *
 * 【怎么编译 + 运行（检测 bug）】
 *
 *   1. 开启所有警告（编译器帮你找）：
 *      gcc -Wall -Wextra -Wpedantic bug_demo.c -o bug.exe
 *      ← 编译器会对你的代码给警告（比如 unused variable）
 *
 *   2. AddressSanitizer（内存错误检测器，巨强！）：
 *      gcc -fsanitize=address -g bug_demo.c -o bug_asan.exe
 *      ./bug_asan.exe
 *      ← 遇到内存错误会打印详细的错误位置 + 调用栈
 *
 *   3. GDB 单步调试：
 *      gcc -g bug_demo.c -o bug_gdb.exe
 *      gdb ./bug_gdb.exe
 *      (gdb) break bug_use_after_free
 *      (gdb) run
 *      (gdb) next       ← 单步
 *      (gdb) print p    ← 打印变量
 *      (gdb) backtrace  ← 看调用栈
 *
 * 【每个 bug 函数的标题告诉你是什么类型的 bug】
 *  取消函数调用的注释来触发 bug，然后用上面的工具检测
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utf8_console.h"

/* ============================================================
 * Bug 1：use-after-free（释放后继续使用）
 * 触发方式：运行 bug_use_after_free()
 * 检测方式：ASan 会报 "heap-use-after-free"
 * ============================================================ */
void bug_use_after_free(void)
{
    printf("--- Bug 1: use-after-free ---\n");
    int *p = (int *)malloc(sizeof(int));
    *p = 42;
    printf("malloc 后 *p = %d\n", *p);
    free(p);
    printf("free 后，下面这行访问是未定义行为！\n");
    /* printf("*p = %d (崩！或输出随机值)\n", *p); */ /* 取消注释触发 bug */
    p = NULL;                                         /* 正确做法：free 后置空 */
    printf("p 已置 NULL\n\n");
}

/* ============================================================
 * Bug 2：缓冲区溢出（数组越界写）
 * 触发方式：运行 bug_buffer_overflow()
 * 检测方式：ASan 会报 "stack-buffer-overflow" 或 "heap-buffer-overflow"
 * ============================================================ */
void bug_buffer_overflow(void)
{
    printf("--- Bug 2: 缓冲区溢出 ---\n");

    /* 栈上溢出 */
    volatile char buf[8]; /* volatile 防止编译器优化掉，也消除 unused 警告 */
    (void)buf;
    printf("buf 只有 8 字节，下面 memcpy 16 字节会溢出！\n");
    /* memcpy(buf, "0123456789ABCDEF", 16);   ← 取消注释触发栈溢出 */
    /* printf("buf = %s\n", buf); */

    /* 堆上溢出 */
    int *heap = (int *)malloc(5 * sizeof(int)); /* 5 个 int */
    for (int i = 0; i < 10; i++)
    {
        /* heap[i] = i;   ← 取消注释：写了 10 个，越界 5 个 */
    }
    free(heap);

    /* scanf 溢出（经典坑！） */
    char small[16];
    /* scanf("%s", small);  ← 取消注释：如果输入超过 15 字符就溢出 */
    scanf("%15s", small); /* 正确：%15s 限制最多 15 字符（留 1 位给 '\0'） */
    printf("安全读入: %s\n\n", small);
}

/* ============================================================
 * Bug 3：野指针（未初始化指针）
 * 触发方式：运行 bug_dangling_pointer()
 * 检测方式：ASan 会报 "segfault" 或直接崩溃
 * ============================================================ */
void bug_dangling_pointer(void)
{
    printf("--- Bug 3: 野指针 ---\n");
    volatile int *p; /* volatile 消除 uninitialized 警告（故意演示 bug） */
    printf("p = %p (随机地址，未定义！)\n", (void *)p);
    /* *p = 100;   ← 取消注释：解引用野指针 = 段错误崩溃 */

    int *safe = NULL; /* 正确：初始化为 NULL */
    if (safe == NULL)
    {
        printf("safe 是 NULL，不能解引用 ✓\n");
    }
    safe = (int *)malloc(sizeof(int));
    *safe = 100;
    printf("赋值后 *safe = %d\n\n", *safe);
    free(safe);
    safe = NULL;
}

/* ============================================================
 * Bug 4：内存泄漏（malloc 忘记 free）
 * 触发方式：运行 bug_memory_leak()
 * 检测方式：ASan 退出时会报 "memory leak detected"
 * ============================================================ */
void bug_memory_leak(void)
{
    printf("--- Bug 4: 内存泄漏 ---\n");
    int *leak = (int *)malloc(100 * sizeof(int));
    /* 做了一些计算... */
    leak[0] = 99;
    /* 忘记 free(leak);   ← 取消注释这行的缺失来触发泄漏 */
    /* ASan 检测：程序退出时会报 "1 block (400 bytes) leaked" */
    printf("leak[0] = %d (但忘记 free 了！)\n\n", leak[0]);

    /* 正确写法：不管中间有没有 return，都要保证 free */
    int *correct = (int *)malloc(100 * sizeof(int));
    correct[0] = 88;
    free(correct);
    correct = NULL;
}

/* ============================================================
 * Bug 5：函数返回局部变量地址（经典错误）
 * 触发方式：运行 bug_return_local_addr()
 * 检测方式：ASan 可能检测到 use-after-return
 * 或者用 GDB 单步看 &local 在函数返回后变成什么
 * ============================================================ */
int *bug_make_local(void)
{
    volatile int local = 999;
    (void)local;
    /* return &local;   ← 取消注释触发 bug！local 在栈上，函数返回就释放了 */
    static int still_alive = 999; /* 正确：用 static 变量，函数返回后还活着 */
    return &still_alive;
}

void bug_return_local_addr(void)
{
    printf("--- Bug 5: 返回局部变量地址 ---\n");
    int *p = bug_make_local();
    printf("p = %p, *p = %d\n", (void *)p, *p);
    printf("(如果用了 static，这里能正确输出 999)\n\n");
}

/* ============================================================
 * Bug 6：整数除法截断 / 溢出
 * 触发方式：运行 bug_integer_math()
 * 检测方式：编译器可能警告，或者值明显不对
 * ============================================================ */
void bug_integer_math(void)
{
    printf("--- Bug 6: 整数截断 / 溢出 ---\n");

    /* 整数除法截断 */
    int a = 5, b = 2;
    int result1 = a / b;            /* 2（不是 2.5！） */
    double result2 = a / b;         /* 2.0（先整数除法得 2，再转 double） */
    double result3 = (double)a / b; /* 2.5（先转 double，再浮点除法） */
    printf("5/2 = %d (整数除法，砍掉小数)\n", result1);
    printf("5/2 = %.1f (int/int 后转 double，已经截断了)\n", result2);
    printf("(double)5/2 = %.1f (正确的浮点除法)\n", result3);

    /* 整数溢出 */
    int big = 2000000000;
    int overflow = big + big; /* 超出 int 范围！变成负数（环绕） */
    printf("\nbig = %d, big+big = %d (溢出！变成负数了)\n", big, overflow);
    printf("用 long long 修复：big + big = %lld\n", (long long)big + big);
    printf("\n");
}

int main(void)
{
    init_utf8_console();
    printf("============================================================\n");
    printf(" C 语言经典 bug 演示\n");
    printf(" 编译: gcc -fsanitize=address -g bug_demo.c -o bug.exe\n");
    printf(" 运行: ./bug.exe\n");
    printf(" 然后取消每个 bug 函数里被注释的代码来触发错误\n");
    printf("============================================================\n\n");

    bug_use_after_free();
    bug_buffer_overflow();
    bug_dangling_pointer();
    bug_memory_leak();
    bug_return_local_addr();
    bug_integer_math();

    return 0;
}