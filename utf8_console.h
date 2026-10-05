/*
 * grammar/utf8_console.h
 * Windows 控制台 UTF-8 初始化（解决中文输出乱码）
 *
 * 【乱码根因】
 *   源码是 UTF-8，gcc 编译后中文以 UTF-8 字节原样写进 exe；
 *   而 Windows 控制台默认代码页是 936(GBK)，会把 UTF-8 字节当 GBK 解码
 *   → 出现「鏁版嵁绫诲瀷澶у皬」这类乱码。
 *
 * 【解决】
 *   在 main 开头调用 init_utf8_console()，把控制台输入/输出代码页设为
 *   65001(UTF-8)，等价于手动执行 chcp 65001，但对每个程序自动生效。
 *
 * 【用法】
 *   #include "utf8_console.h"
 *   int main(void) { init_utf8_console(); ... }
 *
 * 非 Windows 平台（Linux/macOS）为空实现，不影响编译。
 */
#ifndef UTF8_CONSOLE_H
#define UTF8_CONSOLE_H

#ifdef _WIN32
#include <windows.h>
#endif

/* static：每个 .c 各持一份，避免多文件链接冲突 */
static void init_utf8_console(void)
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8); /* printf 输出按 UTF-8 解析 */
    SetConsoleCP(CP_UTF8);       /* scanf 读入按 UTF-8 解析 */
#endif
}

#endif /* UTF8_CONSOLE_H */
