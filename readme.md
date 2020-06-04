<!--
 * @Date: 2020-05-31 17:39:01
 * @LastEditors: zyk
 * @LastEditTime: 2020-06-04 15:34:18
 * @FilePath: \compiler\readme.md
--> 
# 华为毕昇杯(华中科技大学Rigel队)

## `SysY`语言词法规范整理

* 关键字

```
int const void 
if else while break continue return 
```

* 标识符`Ident`

```
ident -> ident_nondigit
       | ident ident_nondigit
       | ident digit
```

即由大小写字母和数字组成，首字符不为数字。

（问题：是否可以有'_'？）

* 数值常量`IntConst`

```
intconst -> decimal-const | octal-const | hex-const

decimal-const -> nonzero-digit | decimal-const digit
octal-const -> 0 | octal-const octal-digit
hex-const -> hex-prefix hex-digit | hex-const hex-digit
```

十进制：非0数字+若干数字0~9
八进制：0开头+若干数字0~7
十六进制：`0x`或`0X`前缀+若干数字0~F

（问题：是否可以为负数？）

* 字符串常量

`putf()`库函数中使用。

例如：

```c
putf("%d: %d(%c), %d(%c)", n, a[0], a[0]+48, a[1], a[1]+48);
```

* 操作符和分隔符

```
+ - * / %
== != < > <= >=
! && ||
, ; = ( ) { } [ ]
```

算符的优先级和结合性与C语言一致。

* 注释

`SysY`语言中的注释规范与C语言一致，即分为单行和多行注释。

