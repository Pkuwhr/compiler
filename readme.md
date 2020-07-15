<!--
 * @Date: 2020-06-13 17:07:18
 * @LastEditors: zyk
 * @LastEditTime: 2020-07-15 12:14:02
 * @FilePath: /compiler/readme.md
-->

# 说明

环境: vscode

首先需要设置vscode终端为git bash,设置方法: Ctrl+Shift+P 输入shell,再进行选择即可

build方法: 快捷键"Ctrl+Shift+B"

清理文件方法: mingw32-make clean

-----

环境：CLion

首先需要执行`make`编译`.l`和`.y`文件，然后才能使用`CMake`执行和调试。

<!-- TODO: 修改`CMakeLists.txt` -->

## Makefile

- `make`
- `make strip`: 在执行`make`基础上对可执行文件应用`strip`操作，并删除中间文件。
- `make clean`
