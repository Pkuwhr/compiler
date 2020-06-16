<!--
 * @Date: 2020-06-13 17:07:18
 * @LastEditors: zyk
 * @LastEditTime: 2020-06-15 20:21:00
 * @FilePath: \compiler\readme.md
--> 
# 说明

环境: vscode

首先需要设置vscode终端为git bash,设置方法: Ctrl+Shift+P 输入shell,再进行选择即可

build方法: 快捷键"Ctrl+Shift+B"

清理文件方法: mingw32-make clean

## Makefile

- `make`
- `make strip`: 在执行`make`基础上对可执行文件应用`strip`操作，并删除中间文件。
- `make clean`
