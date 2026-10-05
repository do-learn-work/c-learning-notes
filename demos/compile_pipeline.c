/*
 * demos/compile_pipeline.c
 * ============================================================
 * 【知识点】C 编译链接的 4 个阶段
 *   预处理 → 编译 → 汇编 → 链接
 *   每个阶段产出什么、gcc 参数怎么控制
 *
 * 【不要运行这个 .c 文件！它是给你看「编译流程」的材料】
 *   用命令行工具执行下面的编译命令，观察每一步的产物
 *
 * 【一步一步来，跟着做】
 * ============================================================
 *
 * === 第 1 步：预处理 ===
 * 作用：展开 #include、替换 #define、处理 #ifdef 条件编译
 * 命令：gcc -E compile_pipeline.c -o step1_preprocessed.i
 * 然后：打开 step1_preprocessed.i，你会看到：
 *   - #include <stdio.h> 被展开成几千行代码
 *   - #define PI 3.14 被直接替换成 3.14
 *   - 注释被全部删除
 *
 * === 第 2 步：编译（C → 汇编） ===
 * 作用：C 代码翻译成汇编指令（人类可读的 CPU 指令）
 * 命令：gcc -S step1_preprocessed.i -o step2_assembly.s
 * 然后：打开 step2_assembly.s，你会看到：
 *   - .intel_syntax noprefix（汇编语法风格）
 *   - main 函数的汇编实现
 *   - 每一行 C 代码对应几条汇编指令
 *   - 变量在栈上的相对偏移（ebp-0x4 这种）
 *
 * === 第 3 步：汇编（汇编 → 目标文件） ===
 * 作用：汇编代码翻译成机器码（二进制），但还不可执行
 * 命令：gcc -c step2_assembly.s -o step3_object.o
 * 然后：用 hexdump 或 objdump 看 .o 文件（可选）
 *   objdump -d step3_object.o   （反汇编目标文件）
 *   .o 文件里已经有机器码，但还没链接到系统库（printf 在哪里？）
 *
 * === 第 4 步：链接（.o + 系统库 → 可执行文件） ===
 * 作用：把你的 .o 文件和 C 标准库、系统启动代码拼在一起
 * 命令：gcc step3_object.o -o step4_final.exe
 * 然后：运行 step4_final.exe 就看到输出了！
 *
 * === 一键查看完整流程（带详细日志） ===
 * 命令：gcc -v compile_pipeline.c -o final.exe
 * 你会看到 gcc 内部调用了 cpp(预处理)、cc1(编译)、as(汇编)、ld(链接)
 * 每个工具的完整命令行参数都打印出来了
 *
 * === 多文件编译（核心概念） ===
 * 假设有 main.c + util.c + helper.c
 * 命令1：gcc -c main.c -o main.o    ← 单独编译每个 .c
 * 命令2：gcc -c util.c -o util.o
 * 命令3：gcc -c helper.c -o helper.o
 * 命令4：gcc main.o util.o helper.o -o app.exe   ← 链接在一起
 * 或者一步到位：gcc main.c util.c helper.c -o app.exe
 * （但大项目都是先分别编译 .o，最后链接，改一个文件不用全重编）
 */

#include <stdio.h>
#include "utf8_console.h"

/* 这个宏会在预处理阶段被直接替换成 3.14159 */
#define PI 3.14159
#define CIRCLE_AREA(r) (PI * (r) * (r))

int main(void)
{
    init_utf8_console();
    /* 这些代码经过 4 个阶段变成最终的机器码 */
    double radius = 5.0;
    double area = CIRCLE_AREA(radius); /* 预处理后变成: 3.14159 * (radius) * (radius) */

    printf("半径 %.1f 的圆面积 = %.2f\n", radius, area);
    printf("这行 printf 在链接阶段会被找到 C 标准库的实现\n");
    printf("gcc 会自动链接 libc.so / msvcrt.dll\n");

    /* 用 extern 声明一个"别处定义"的变量（演示声明 vs 定义） */
    /* extern int g_pipeline_demo_counter; */ /* ← 如果没在别处定义，链接会报错！ */
    /* printf("g_pipeline_demo_counter = %d\n", g_pipeline_demo_counter); */
    /* 这个现象就是"多文件编译 + 链接"要解决的核心问题
     * 参见 demos/multi_file/ 里的完整示例 */

    return 0;
}