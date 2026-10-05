# ============================================================
# C 语言学习笔记与实践 —— 构建脚本
#
# 用法（在 Git Bash 中运行）：
#   make            编译全部程序
#   make grammar    只编译 grammar/
#   make demos      只编译 demos/（含多文件示例）
#   make solutions  只编译 solutions/
#   make multi      只编译 demos/multi_file 多文件示例
#   make clean      删除所有生成的 .exe
#   make list       列出所有源文件
#   make help       显示帮助
#
# 注意 1：Windows 下 MinGW 默认把 make 命名为 mingw32-make.exe，
#         已在 ~/.bashrc 中 alias 为 make（仅限 bash）。
#         若在 cmd / PowerShell 中使用，请把命令换成 mingw32-make。
#
# 注意 2：所有子目录下的程序都 #include "utf8_console.h"，
#         而该头文件唯一存放于仓库根目录，故 -I. 不可省略。
#
# 注意 3【重要】：Windows 版 GNU Make 会把 Makefile 内容按系统 ANSI
#         代码页（简体中文 Windows 为 GBK）读取，再转成 UTF-8 输出。
#         结果是：注释里的中文没问题，但被 shell 执行的行里的中文会
#         变成乱码。因此下面所有 @echo / @printf 的输出一律用 ASCII
#         （英文），不要把这里改回中文。
# ============================================================

# 用 := 而不是 ?= 的原因：CC / LDLIBS 是 GNU Make 的隐含变量，
# make 自身已给 CC 赋值 cc；用 ?= 判定「已定义」为真，赋值会被静默忽略，
# 结果编译器变成不存在的 cc。而命令行赋值（make CC=clang）本来就优先于
# :=，所以用 := 既能固定默认值、又保留了命令行覆盖能力。两全其美。
CC       := gcc
CFLAGS   := -Wall -Wextra -std=c17
INCLUDES := -I. -Idemos/multi_file
# sqrt()(util.c) 与 fabs()(grammar/06) 需要数学库。
# MinGW 与 glibc >= 2.34 已把数学函数并入默认库，加了无副作用；
# 较老的 Linux 不加会链接失败。一律加上以保证可移植。
LDLIBS   := -lm
RM       := rm -f

# 一旦配方失败就删除半成品目标。少了这句，编译被 Ctrl+C 中断时
# 会留下时间戳比源码还新的残缺 .exe，下次 make 会误判「已最新」跳过它。
.DELETE_ON_ERROR:

# ---- 源文件（自动发现，新增文件无需改这里）----
ROOT_SRC      := $(wildcard hello.c)
GRAMMAR_SRC   := $(wildcard grammar/*.c)
DEMOS_SRC     := $(wildcard demos/*.c)
SOLUTIONS_SRC := $(wildcard solutions/*.c)
MULTI_SRC     := demos/multi_file/main.c demos/multi_file/util.c

# ---- 对应的可执行文件（与源文件同目录）----
ROOT_BIN      := $(ROOT_SRC:.c=.exe)
GRAMMAR_BIN   := $(GRAMMAR_SRC:.c=.exe)
DEMOS_BIN     := $(DEMOS_SRC:.c=.exe)
SOLUTIONS_BIN := $(SOLUTIONS_SRC:.c=.exe)
MULTI_BIN     := demos/multi_file/multi.exe

ALL_BIN := $(ROOT_BIN) $(GRAMMAR_BIN) $(DEMOS_BIN) $(SOLUTIONS_BIN) $(MULTI_BIN)

.PHONY: all grammar demos solutions multi clean list help

# ---- 默认目标 ----
all: $(ALL_BIN)
	@echo ""
	@echo "All done: $(words $(ALL_BIN)) programs built."
	@echo "Tip: demos/bug_demo.c shows an intentional -Wuninitialized warning."

# ---- 分组目标 ----
grammar:   $(GRAMMAR_BIN)
demos:     $(DEMOS_BIN) $(MULTI_BIN)
solutions: $(SOLUTIONS_BIN)
multi:     $(MULTI_BIN)

# ---- 通用规则：单个 .c 编译为同目录同名 .exe ----
%.exe: %.c utf8_console.h
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@ $(LDLIBS)

# ---- 例外：根目录的 hello.c 不依赖 utf8_console.h ----
hello.exe: hello.c
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@ $(LDLIBS)

# ---- 例外：多文件示例需同时编译 main.c 和 util.c ----
$(MULTI_BIN): $(MULTI_SRC)
	$(CC) $(CFLAGS) $(INCLUDES) $(MULTI_SRC) -o $@ $(LDLIBS)

# ---- 清理 ----
clean:
	$(RM) hello.exe demos/*.exe demos/multi_file/multi.exe grammar/*.exe solutions/*.exe
	@echo "Cleaned: all generated .exe removed."

# ---- 列出源文件 ----
list:
	@echo "root:        $(ROOT_SRC)"
	@echo "grammar/:    $(GRAMMAR_SRC)"
	@echo "demos/:      $(DEMOS_SRC)"
	@echo "solutions/:  $(SOLUTIONS_SRC)"
	@echo "multi_file/: $(MULTI_SRC)"

# ---- 帮助 ----
help:
	@echo "Usage: make [target]"
	@echo ""
	@echo "  all        Build all programs (default)"
	@echo "  grammar    Build the 6 grammar topics under grammar/"
	@echo "  demos      Build topic demos under demos/ (incl. multi-file demo)"
	@echo "  solutions  Build the 7 exercise solutions under solutions/"
	@echo "  multi      Build the demos/multi_file multi-file demo"
	@echo "  clean      Remove all generated .exe"
	@echo "  list       List all source files"
	@echo "  help       Show this message"
	@echo ""
	@echo "Variables (overridable):"
	@echo "  make CC=clang        Use another compiler"
	@echo "  make LDLIBS=' '      Do not link libm"
	@echo "  make CFLAGS='-O2 -g' Replace the default warning/std flags"
