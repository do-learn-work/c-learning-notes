/*
 * grammar/02_control_flow.c
 * 控制流语法：if-else、switch-case、for、while、do-while
 *             break、continue、goto
 * 对应刷题：Day2-Day3
 * 编译：gcc -Wall -std=c17 02_control_flow.c -o 02.exe
 *
 * ==========================================
 * 语法速查表
 * ==========================================
 * 1. if-else：   if (条件) { ... } else if (条件) { ... } else { ... }
 * 2. switch：    switch (整数值) { case 常量: ... break; default: ... }
 * 3. for：       for (初始化; 条件; 更新) { ... }
 * 4. while：     while (条件) { ... }
 * 5. do-while：  do { ... } while (条件);   ← 末尾有分号！
 * 6. break：     跳出最近的 switch / 循环
 * 7. continue：  跳过当前循环剩余部分，进入下一次迭代
 * 8. goto：      跳转到标签（不推荐，但刷题有时会用到）
 * ==========================================
 */

#include <stdio.h>
#include "utf8_console.h"

/* ================================================================
 * 一、if-else 链（最常用的分支）
 * ================================================================
 *
 * 【语法模板】
 *   if (条件1) {
 *       语句1;
 *   } else if (条件2) {
 *       语句2;
 *   } else if (条件3) {
 *       语句3;
 *   } else {
 *       兜底语句;   ← 可选
 *   }
 *
 * 【规则】
 *   - 条件为真（非 0）→ 执行对应语句，后面的 else-if 不再判断
 *   - 所有条件都为假 → 执行 else（如果有的话）
 *   - 可以嵌套多层（但超过 3 层建议重构）
 *
 * 【坑点】
 *   1. if (n = 5) ← 赋值不是比较！编译器可能警告，但逻辑上永远为真
 *   2. 大括号建议永远加，哪怕只有一句（防止以后加语句时出 bug）
 *   3. if-else 链的顺序：最具体的条件放前面（先判断等边三角形再判断等腰）
 */

void demo_if_else(void)
{
    printf("=== if-else ===\n");

    /* 基础示例：成绩评级 */
    int score = 85;
    if (score >= 90)
    {
        printf("等级 A\n");
    }
    else if (score >= 80)
    {
        printf("等级 B\n"); // ← score=85 命中这里
    }
    else if (score >= 60)
    {
        printf("等级 C\n");
    }
    else
    {
        printf("等级 D\n");
    }

    /* 嵌套示例：判断三角形类型
     * 刷题坑：先判等边（最具体），再判等腰，最后普通
     *   如果先判等腰，等边三角形也会被当成等腰！
     */
    int a = 3, b = 3, c = 3;
    if (a + b <= c || a + c <= b || b + c <= a)
    {
        printf("不能构成三角形\n");
    }
    else if (a == b && b == c)
    { // 等边先判
        printf("等边三角形\n");
    }
    else if (a == b || b == c || a == c)
    { // 再判等腰
        printf("等腰三角形\n");
    }
    else
    {
        printf("普通三角形\n");
    }

    /* 条件运算符（三目运算符）：expr1 ? expr2 : expr3
     * 等价于 if-else，但只能写表达式，不能写语句
     */
    int x = 10, y = 20;
    int max_val = (x > y) ? x : y; // ← 替代 if-else 取最大值
    printf("max(%d, %d) = %d\n", x, y, max_val);
}

/* ================================================================
 * 二、switch-case（多分支等值判断）
 * ================================================================
 *
 * 【语法模板】
 *   switch (整数/字符/枚举表达式) {
 *       case 常量1:
 *           语句1;
 *           break;
 *       case 常量2:
 *           语句2;
 *           break;
 *       default:       ← 可选，所有 case 都不匹配时执行
 *           兜底语句;
 *           break;
 *   }
 *
 * 【核心规则】
 *   1. switch 表达式只能是 int/char/枚举（不能是 float/double！）
 *   2. case 后面必须是**常量**（不能是变量、不能是范围）
 *   3. case 标签后是冒号 : ，不是分号 ;
 *   4. break 跳出整个 switch，**忘加会穿透到下一个 case**（fall-through）
 *   5. case 和大括号对齐（C 社区规范，case 是跳转标签，平级）
 *
 * 【合法利用 fall-through 的场景】
 *   case 1:
 *   case 2:
 *   case 3:
 *       printf("上旬\n");
 *       break;
 *   输入 1、2、3 都输出上旬，故意不加 break 让它们穿透到同一段代码
 */

void demo_switch(void)
{
    printf("=== switch-case ===\n");

    /* 基础示例：星期判断（Day2 刷题原题） */
    int weekday = 3;
    switch (weekday)
    {
    case 1:
        printf("Mon\n");
        break;
    case 2:
        printf("Tue\n");
        break;
    case 3:
        printf("Wed\n"); // ← weekday=3 命中这里
        break;
    case 4:
        printf("Thu\n");
        break;
    case 5:
        printf("Fri\n");
        break;
    case 6:
        printf("Sat\n");
        break;
    case 7:
        printf("Sun\n");
        break;
    default:
        printf("invalid\n"); // ← 输入 0/8/-1 等非法值时执行
        break;
    }

    /* fall-through 合法利用：分类统计 */
    int month = 2;
    switch (month)
    {
    case 1:
    case 2:
    case 3:
        printf("Q1（第一季度）\n"); // ← 1、2、3 月都走到这里
        break;
    case 4:
    case 5:
    case 6:
        printf("Q2（第二季度）\n");
        break;
    default:
        printf("下半年\n");
        break;
    }
}

/* ================================================================
 * 三、for 循环（最常用，计数循环）
 * ================================================================
 *
 * 【语法模板】
 *   for (初始化; 循环条件; 每次迭代后的更新) {
 *       循环体;
 *   }
 *
 * 【执行流程】
 *   1. 初始化（只执行 1 次）
 *   2. 判断条件：假 → 跳出循环；真 → 执行循环体
 *   3. 执行更新语句
 *   4. 回到步骤 2
 *
 * 【规则】
 *   - C99 起支持在 for 里声明变量：for (int i = 0; ...) ← 推荐！
 *   - for 里的三个表达式都可以省略（死循环：for (;;) { ... }）
 *   - 循环变量 i / j / k 是社区默认命名惯例
 *
 * 【坑点】
 *   1. for 括号里的分号是分号！最后一个不是逗号！
 *      for (int i = 0, i < 10, i++) ← 错！分号分隔三个部分
 *      for (int i = 0; i < 10; i++) ← 对
 *   2. 循环条件用 ; 结尾会变成空循环体（常见笔误）
 *      for (int i = 0; i < 10; i++); ← 分号在这！循环体是空的
 *
 * 【刷题关联】遍历数组、计数求和、打印图形
 */

void demo_for(void)
{
    printf("=== for 循环 ===\n");

    /* 基础：1 加到 100 */
    int sum = 0;
    for (int i = 1; i <= 100; i++)
    {
        sum += i;
    }
    printf("1+2+...+100 = %d\n", sum);

    /* 倒序：10 到 1 */
    printf("倒序：");
    for (int i = 10; i >= 1; i--)
    {
        printf("%d ", i);
    }
    printf("\n");

    /* 步进 2：打印奇数 */
    printf("奇数：");
    for (int i = 1; i <= 10; i += 2)
    {
        printf("%d ", i);
    }
    printf("\n");

    /* 嵌套 for：打印乘法口诀（经典练手） */
    printf("乘法口诀：\n");
    for (int i = 1; i <= 9; i++)
    {
        for (int j = 1; j <= i; j++)
        {
            printf("%d×%d=%-3d", j, i, i * j);
        }
        printf("\n");
    }
    /* 外层 i 控制行数（1~9），内层 j 控制每行的列数（1~i） */
}

/* ================================================================
 * 四、while 循环（条件循环）
 * ================================================================
 *
 * 【语法模板】
 *   while (循环条件) {
 *       循环体;
 *   }
 *
 * 【执行流程】
 *   1. 判断条件：假 → 跳出；真 → 执行循环体
 *   2. 回到步骤 1
 *
 * 【和 for 的区别】
 *   - for：计数循环，知道要循环多少次
 *   - while：条件循环，不知道要循环多少次（比如"一直读入直到遇到 EOF"）
 *
 * 【坑点】
 *   - 循环体里必须有能让条件变成假的语句，否则死循环
 */

void demo_while(void)
{
    printf("=== while 循环 ===\n");

    /* 基础：读入若干整数直到输入 0，求和 */
    /*
    int total = 0, num;
    printf("输入整数（0 结束）：");
    scanf("%d", &num);
    while (num != 0) {
        total += num;
        scanf("%d", &num);   // ← 循环体里必须更新 num！否则死循环
    }
    printf("总和 = %d\n", total);
    */

    /* 更简洁的写法（把 scanf 写在条件里） */
    /*
    int total = 0, num;
    while (scanf("%d", &num) == 1 && num != 0) {
        total += num;
    }
    printf("总和 = %d\n", total);
    */
    /* scanf 返回值是成功读入的变量个数：
     * scanf("%d", &num) → 返回 1 表示成功读入，返回 EOF 表示输入结束
     * 这是刷题里读入多组数据的标准写法！
     */

    /* 用 while 实现：求一个数的各位之和 */
    int n = 12345;
    int digit_sum = 0;
    while (n > 0)
    {
        digit_sum += n % 10; // 取最后一位
        n /= 10;             // 去掉最后一位
    }
    printf("12345 各位之和 = %d\n", digit_sum); // 1+2+3+4+5 = 15
}

/* ================================================================
 * 五、do-while 循环（至少执行一次）
 * ================================================================
 *
 * 【语法模板】
 *   do {
 *       循环体;
 *   } while (循环条件);   ← 末尾必须有分号！
 *
 * 【和 while 的区别】
 *   - while：先判断条件，可能一次都不执行
 *   - do-while：先执行一次循环体，再判断条件
 *
 * 【使用场景】很少用，典型场景是"至少要读入一次用户输入"
 */

void demo_do_while(void)
{
    printf("=== do-while ===\n");

    /* 确保至少执行一次：判断一个数是不是回文数（从后往前读一样） */
    int num = 12321;
    int original = num;
    int reversed = 0;
    do
    {
        reversed = reversed * 10 + num % 10;
        num /= 10;
    } while (num > 0); // 末尾分号！
    printf("%d 反转 = %d, %s\n",
           original, reversed,
           original == reversed ? "是回文数" : "不是回文数");
    /* num=0 时循环体也要执行一次（把 0 加进去），所以用 do-while 更合适 */
}

/* ================================================================
 * 六、break 和 continue（循环的「紧急出口」和「跳过键」）
 * ================================================================
 *
 * 【break】
 *   - 跳出最近的 switch / for / while / do-while
 *   - 只能跳出一层！多层嵌套需要配合标志变量或 goto
 *   - 使用场景：找到目标后提前结束（比把条件写复杂更清晰）
 *
 * 【continue】
 *   - 跳过当前循环剩余部分，直接进入下一次迭代
 *   - 只在循环里有意义（switch 里写 continue 没用）
 *   - 使用场景：过滤掉某些元素，继续处理剩下的
 */

void demo_break_continue(void)
{
    printf("=== break 和 continue ===\n");

    /* break 示例：找到第一个能被 7 整除的数 */
    for (int i = 1; i <= 100; i++)
    {
        if (i % 7 == 0)
        {
            printf("1-100 中第一个被 7 整除的数：%d\n", i);
            break; // ← 找到就结束循环，不用跑到 100
        }
    }

    /* continue 示例：打印 1-20 中不是 3 的倍数的数 */
    printf("1-20 中非 3 的倍数：");
    for (int i = 1; i <= 20; i++)
    {
        if (i % 3 == 0)
        {
            continue; // ← 跳过本次循环，不执行后面的 printf
        }
        printf("%d ", i);
    }
    printf("\n");
}

int main(void)
{
    init_utf8_console(); /* 让中文正常显示（详见 utf8_console.h） */
    demo_if_else();
    printf("\n");
    demo_switch();
    printf("\n");
    demo_for();
    printf("\n");
    demo_while();
    printf("\n");
    demo_do_while();
    printf("\n");
    demo_break_continue();
    return 0;
}