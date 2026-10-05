/*
 * grammar/04_advanced.c
 * 高级语法：函数（进阶）、结构体、动态内存、文件、预处理器
 * 对应刷题：Day6-Day7 + 后续进阶
 * 编译：gcc -Wall -std=c17 04_advanced.c -o 04.exe
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utf8_console.h"

/* ================================================================
 * 一、函数进阶
 * ================================================================
 *
 * 【函数定义模板】
 *   返回类型 函数名(参数列表) {
 *       函数体;
 *       return 返回值;   ← 如果返回类型不是 void
 *   }
 *
 * 【函数声明（前置声明）】
 *   如果函数定义在 main 之后，需要在 main 之前加声明：
 *   返回类型 函数名(参数类型列表);   ← 参数名可省略
 *
 * 【递归函数】自己调用自己（必须有终止条件！）
 *   经典：阶乘、斐波那契、汉诺塔、树遍历
 *
 * 【函数指针】指向函数的指针（进阶，刷题暂不需要，先知道存在）
 */

/* 函数定义在 main 之前不需要声明 */
int factorial(int n)
{
    /* 阶乘：5! = 5*4*3*2*1 = 120
     * 终止条件：n <= 1 时返回 1
     * 递归关系：n! = n * (n-1)!
     */
    if (n <= 1)
        return 1;
    return n * factorial(n - 1);
}

/* 静态变量 static：只初始化一次，函数调用之间保留值 */
int counter(void)
{
    static int count = 0; // ← 只在第一次调用时初始化为 0
    count++;
    return count;
}

void demo_functions(void)
{
    printf("=== 函数进阶 ===\n");

    /* 递归 */
    printf("5! = %d\n", factorial(5));   // 120
    printf("10! = %d\n", factorial(10)); // 3628800

    /* static 变量演示 */
    printf("counter() 连续调用：");
    for (int i = 0; i < 5; i++)
    {
        printf("%d ", counter()); // 1 2 3 4 5
    }
    printf("\n");
    /* 如果 count 不是 static，每次 counter() 都会输出 1 */
}

/* ================================================================
 * 二、结构体 struct（把多个不同类型的变量打包成一个）
 * ================================================================
 *
 * 【语法模板】
 *   // 定义结构体类型
 *   struct Student {
 *       char name[32];
 *       int age;
 *       double score;
 *   };
 *
 *   // 声明变量
 *   struct Student s1;
 *   struct Student s2 = {"张三", 20, 85.5};   // 初始化
 *
 *   // 访问成员：用 . （点号）
 *   s1.age = 21;
 *   printf("%s 的分数是 %.1f\n", s2.name, s2.score);
 *
 * 【typedef 简化命名】（推荐，刷题必用）
 *   typedef struct Student Student;
 *   之后就能直接写：Student s1;  不用每次写 struct Student
 *
 * 【指针访问结构体成员】
 *   Student *p = &s1;
 *   p->age = 22;   // 用 -> （箭头），等价于 (*p).age
 */

typedef struct
{
    char name[32];
    int age;
    double score;
} Student; // typedef 后，Student 直接是类型名

void demo_struct(void)
{
    printf("\n=== 结构体 ===\n");

    /* 声明 + 初始化 */
    Student s1 = {"张三", 20, 85.5};
    Student s2;
    strcpy(s2.name, "李四"); // 字符串不能用 = 赋值，要用 strcpy
    s2.age = 21;
    s2.score = 92.0;

    printf("学生1：%s, %d岁, 分数 %.1f\n", s1.name, s1.age, s1.score);
    printf("学生2：%s, %d岁, 分数 %.1f\n", s2.name, s2.age, s2.score);

    /* 结构体数组 */
    Student class[] = {
        {"张三", 20, 85.5},
        {"李四", 21, 92.0},
        {"王五", 19, 78.5}};
    int n = sizeof(class) / sizeof(class[0]);
    double total = 0;
    for (int i = 0; i < n; i++)
    {
        total += class[i].score;
    }
    printf("班级平均分：%.2f\n", total / n);

    /* 指针访问 -> */
    Student *p = &s1;
    printf("通过指针访问：%s, %d岁\n", p->name, p->age);
    /* p->name 等价于 (*p).name，箭头更简洁 */
}

/* ================================================================
 * 三、动态内存（malloc / free）
 * ================================================================
 *
 * 【什么时候需要动态内存？】
 *   1. 数组长度运行时才确定（VLA 不够用的时候）
 *   2. 需要手动控制内存的生命周期（不随函数结束释放）
 *   3. 数据量很大（栈内存有限，默认通常 1-8MB）
 *
 * 【语法模板】
 *   int *p = (int *)malloc(n * sizeof(int)); // 申请 n 个 int 的空间
 *   if (p == NULL) {
 *       // 申请失败，必须处理！
 *   }
 *   // 使用 p ...
 *   free(p);  // ← 用完必须释放！否则内存泄漏
 *   p = NULL; // ← 释放后置空，防止野指针
 *
 * 【配套函数】
 *   calloc(n, size)   申请并清零（全 0）
 *   realloc(p, size)  重新调整已申请的内存大小
 *
 * 【坑点】
 *   1. malloc 不会自动清零，calloc 会
 *   2. 用完一定要 free！忘记释放 = 内存泄漏
 *   3. free 之后指针不能再用（变成野指针），要置 NULL
 */

void demo_dynamic(void)
{
    printf("\n=== 动态内存 ===\n");

    /* 申请一个动态数组，长度由用户输入（或变量） */
    int n = 5;
    int *arr = (int *)malloc(n * sizeof(int));
    if (arr == NULL)
    {
        printf("内存申请失败！\n");
        return;
    }

    /* 使用 */
    for (int i = 0; i < n; i++)
    {
        arr[i] = i * i;
    }
    printf("动态数组：");
    for (int i = 0; i < n; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");

    /* 用完释放 */
    free(arr);
    arr = NULL;
    /* 如果忘了 free，内存会一直被占用直到程序结束 */
}

/* ================================================================
 * 四、文件操作（<stdio.h>）
 * ================================================================
 *
 * 【语法模板】
 *   FILE *fp = fopen("文件名", "模式");
 *   if (fp == NULL) {
 *       // 打开失败，必须处理！
 *   }
 *   // 读写（和 printf/scanf 几乎一样，只是多了 fp 参数）
 *   fprintf(fp, "%d\n", a); // 写入文件
 *   fscanf(fp, "%d", &a);   // 从文件读
 *   fgets(buf, size, fp);   // 读一行字符串
 *   fputs(buf, fp);         // 写字符串
 *   fread / fwrite          // 二进制读写
 *   fclose(fp);             // 关闭文件（写完必须关，否则数据可能没刷到磁盘）
 *
 * 【常用模式】
 *   "r"  只读（文本）     "w"  只写（清空再写）
 *   "a"  追加（在末尾加） "rb" "wb" "ab" 二进制模式
 */

void demo_file(void)
{
    printf("\n=== 文件操作 ===\n");

    /* 写文件 */
    FILE *fp = fopen("test_output.txt", "w");
    if (fp == NULL)
    {
        printf("文件打开失败！\n");
        return;
    }
    fprintf(fp, "Hello from C!\n");
    fprintf(fp, "分数：%d\n", 95);
    fclose(fp);
    printf("已写入 test_output.txt\n");

    /* 读文件 */
    fp = fopen("test_output.txt", "r");
    if (fp == NULL)
    {
        printf("文件打开失败！\n");
        return;
    }
    char buf[128];
    printf("文件内容：\n");
    while (fgets(buf, sizeof(buf), fp) != NULL)
    {
        printf("  %s", buf); // fgets 会保留换行符
    }
    fclose(fp);
}

/* ================================================================
 * 五、预处理器指令（#开头的，在编译前处理）
 * ================================================================
 *
 * 【常用指令】
 *   #include <header.h>    引入系统头文件
 *   #include "header.h"    引入自己项目的头文件
 *   #define PI 3.1415926   宏定义（常量）
 *   #define MAX(a,b) ((a)>(b)?(a):(b))   带参数的宏（函数式宏）
 *   #ifdef / #endif        条件编译
 *
 * 【宏 vs 常量】
 *   - 宏：纯文本替换，没有类型，不占内存，可能有副作用
 *     #define SQUARE(x) ((x)*(x))   SQUARE(i++) 会让 i 加两次！
 *   - const 常量：有类型，占内存，安全
 *     const double pi = 3.1415926;  ← 推荐
 *   - enum 枚举常量：也常用作常量
 *
 * 【坑点】
 *   函数式宏的参数一定要加括号！
 *   #define SQUARE(x) x*x     ← 错误：SQUARE(a+b) = a+b*a+b = a+(b*a)+b
 *   #define SQUARE(x) ((x)*(x)) ← 正确
 */

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define ARR_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
/* ARR_SIZE 是常用宏，用来算数组长度（仅限当前作用域） */

void demo_preprocessor(void)
{
    printf("\n=== 预处理器 ===\n");

    /* 函数式宏 */
    printf("MAX(3, 7) = %d\n", MAX(3, 7));
    printf("MAX(10, 5) = %d\n", MAX(10, 5));

    /* ARR_SIZE 宏 */
    int arr[] = {1, 2, 3, 4, 5, 6};
    printf("数组长度 = %zu\n", ARR_SIZE(arr));
}

/* ================================================================
 * 六、作用域和存储期
 * ================================================================
 *
 * 【作用域】变量/函数在哪些地方可见
 *   块作用域：{ } 内部定义的变量，出了 {} 就看不见
 *   文件作用域：函数外定义的变量，整个文件可见
 *   函数作用域：只有 goto 标签（整个函数内可见）
 *
 * 【存储期】变量的内存什么时候分配、什么时候释放
 *   自动存储期：局部变量（默认），进入块分配，离开块释放（栈上）
 *   静态存储期：全局变量 + static 变量，程序开始分配，结束释放
 *   动态存储期：malloc 申请的，手动分配手动释放（堆上）
 *
 * 【static 的两种含义】
 *   1. 函数内 static int count = 0;  → 静态局部变量，保留值
 *   2. 文件外 static void helper();  → 内部链接，只在当前文件可见
 */

int main(void)
{
    init_utf8_console(); /* 让中文正常显示（详见 utf8_console.h） */
    demo_functions();
    demo_struct();
    demo_dynamic();
    demo_file();
    demo_preprocessor();
    return 0;
}