# C 语言学习笔记与实践

一套 C 语言从入门到进阶的学习代码库，按「语法讲解 → 专题demo → 练习解答」三条线组织，配合 VS Code + MinGW (GCC) 直接可编译运行。

## 目录结构

```
.
├── hello.c# 最小示例：环境验证
├── utf8_console.h          # Windows 控制台 UTF-8 初始化（解决中文输出乱码）
├── grammar/                # 语法专题讲解（6 讲，由浅入深）
│   ├── 01_variables_io.c         # 变量、类型、输入输出
│   ├── 02_control_flow.c         # 条件与循环
│   ├── 03_data_structures.c      # 数组、结构体、联合体
│   ├── 04_advanced.c             # 函数进阶与递归
│   ├── 05_pointer_advanced.c     # 指针与内存操作
│   ├── 06_c99_c11_features.c     # C99/C11 新特性
│   └── utf8_console.h
├── demos/                  # 专题 Demo（工程实践向）
│   ├── memory_model.c            # 栈/堆/全局/BSS/代码段内存布局
│   ├── stdlib_deep.c             # 标准库深入用法
│   ├── compile_pipeline.c        # 编译链接运行全流程
│   ├── bug_demo.c                # 典型 bug 复现与定位
│   └── multi_file/               # 多文件编译（main.c / util.c / util.h）
└── solutions/              # 每日练习解答（Day 1-7）
    ├── day01_basics.c            # 基础
    ├── day02_branch.c            # 分支
    ├── day03_loop.c              # 循环
    ├── day04_array.c             # 数组
    ├── day05_string.c            # 字符串
    ├── day06_function.c          # 函数
    └── day07_comprehensive.c     # 综合 + 复盘
```

## 环境要求

- **编译器**：GCC 16.2.0（WinLibs MinGW x64 / UCRT）
- **标准**：C17
- **调试**：GDB
- **编辑器**：VS Code + C/C++ 扩展（`C/C++: gcc 编译当前文件` 任务已配置）

## 编译与运行

单个文件编译（VS Code 中按 `F5` 或 `Ctrl+Shift+B` 亦可）：

```bash
# 根目录的hello.c 无需额外参数
gcc -Wall -Wextra -std=c17 hello.c -o hello.exe
./hello.exe

# grammar/ 下的文件自带头文件，直接编译
gcc -Wall -Wextra -std=c17 grammar/02_control_flow.c -o flow.exe

# demos/ 与 solutions/ 下的文件 #include "utf8_console.h"，
# 该头文件在仓库根目录，必须用 -I. 指定头文件搜索路径
gcc -Wall -Wextra -std=c17 -I. solutions/day07_comprehensive.c -o day07.exe
./day07.exe
```

> **关键**：从 `demos/` 和 `solutions/` 下编译时，务必带上 `-I.`（源码所在目录），
> 否则会报 `fatal error: utf8_console.h: No such file or directory`。

开启 AddressSanitizer 检测内存错误（需安装带 ASan 的 MinGW，普通 WinLibs 包不带 `libasan`）：

```bash
gcc -fsanitize=address -g -I. demos/memory_model.c -o memory_asan.exe
./memory_asan.exe
```

GDB 单步调试：

```bash
gcc -g grammar/05_pointer_advanced.c -o ptr.exe
gdb ./ptr.exe
```

多文件编译（同时需要根目录和子目录的头文件搜索路径）：

```bash
gcc -Wall -Wextra -std=c17 -I. -Idemos/multi_file \
    demos/multi_file/main.c demos/multi_file/util.c -o multi.exe
./multi.exe
```

## 中文输出乱码说明

源码为 UTF-8，GCC 编译后中文以 UTF-8 字节写入 exe，而 Windows 控制台默认代码页为 936(GBK)，
会把 UTF-8 字节按 GBK 解码，出现「鏁版嵁绫诲瀷澶у皬」这类乱码。

**解决**：在 `main()` 开头调用 `init_utf8_console()`，把控制台代码页设为 65001(UTF-8)，
等价于手动执行 `chcp 65001`，但对每个程序自动生效。非 Windows 平台为空实现，不影响编译。

## 说明

- 仓库只收录 `.c` / `.h` 源文件，编译产物（`*.exe`）已由 `.gitignore` 排除，随时可重新编译。
- `.vscode/` 未入库，因其中含本机编译器的绝对路径，不具备可移植性。
