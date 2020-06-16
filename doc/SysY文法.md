<!--
 * @Date: 2020-06-13 23:28:40
 * @LastEditors: zyk
 * @LastEditTime: 2020-06-15 21:09:24
 * @FilePath: \compiler\doc\SysY文法.md
--> 
# SysY文法

原文档《[SysY语言定义](https://gitlab.eduxiji.net/nscscc/docs/-/blob/master/SysY语言定义.pdf)》中采用EBNF范式表示，为了使用Bison自动生成语法分析器，需要将其修改为基本的上下文无关文法表示法。

| 说明             | 左部               | 右部                                                         |
| ---------------- | ------------------ | ------------------------------------------------------------ |
| 编译单元         | `CompUnit`         | `Decl`<br />`FuncDef`<br />`CompUnit Decl`<br />`CompUnit FuncDef` |
| 声明             | `Decl`             | `ConstDecl`<br />`VarDecl`                                   |
| 常量声明         | `ConstDecl`        | `'const' BType ConstDefSeq ';'`                              |
| 常量定义列表     | `ConstDefSeq`      | `ConstDef`<br />`ConstDefSeq ',' ConstDef`                   |
| 基本类型         | `BType`            | `'int'`<br />`void`                                          |
| 常数定义         | `ConstDef`         | `Ident '=' ConstInitVal`<br />`Ident ConstArraySubSeq '=' ConstInitVal` |
| 数组常量下标列表 | `ConstArraySubSeq` | `'[' ConstExp ']'`<br />`ConstArraySubSeq '[' ConstExp ']'`  |
| 数组下标列表     | `ArraySubSeq`      | `epsilon`<br />`ArraySubSeq '[' Exp ']'`                 |
| 常量初值         | `ConstInitVal`     | `ConstExp`<br />`'{' ConstInitValSeq '}'`<br />`'{' '}'`     |
| 常量初值列表     | `ConstInitValSeq`  | `ConstInitVal`<br />`ConstInitValSeq ',' ConstInitVal`       |
| 变量声明         | `VarDecl`          | `BType VarDefSeq ';'`                                        |
| 变量定义列表     | `VarDefSeq`        | `VarDef`<br />`VarDefSeq ',' VarDef`                         |
| 变量定义         | `VarDef`           | `Ident`<br />`Ident ConstArraySubSeq`<br />`Ident '=' InitVal`<br />`Ident ConstArraySubSeq '=' InitVal` |
| 变量初值         | `InitVal`          | `Exp`<br />`'{' InitValSeq '}'`                              |
| 变量初值列表     | `InitValSeq`       | `InitVal`<br />`InitValSeq ',' InitVal`                      |
| 函数定义         | `FuncDef`          | `BType Ident '(' ')' Block`<br />`BType Ident '(' FuncFParams ')' Block` |
| 函数形参表       | `FuncFParams`      | `FuncFParam`<br />`FuncFParams ',' FuncFParam`               |
| 函数形参         | `FuncFParam`       | `BType Ident`<br />`BType Ident '[' ']' ArraySubSeq`         |
| 语句块           | `Block`            | `'{' '}'`<br />`'{' BlockItemSeq '}'`                        |
| 语句块项列表     | `BlockItemSeq`     | `BlockItem`<br />`BlockItemSeq BlockItem`                    |
| 语句块项         | `BlockItem`        | `Decl`<br />`Stmt`                                           |
| 语句             | `Stmt`             | `LVal '=' Exp ';'`<br />`Exp ';'`<br />`';'`<br />`Block`<br />`'if' '(' Cond ')' Stmt`<br />`'if' '(' Cond ')' Stmt 'else' Stmt`<br />`'while' '(' Cond ')' Stmt`<br />`'break' ';'`<br />`'continue' ';'`<br />`'return' Exp ';'`<br />`'return' ';'` |
| 表达式           | `Exp`              | `AddExp`                                                     |
| 条件表达式       | `Cond`             | `LOrExp`                                                     |
| 左值表达式       | `LVal`             | `Ident ArraySubSeq`                                          |
| 基本表达式       | `PrimaryExp`       | `'(' Exp ')'`<br />`LVal`<br />`Number`                      |
| 数值             | `Number`           | `IntConst`                                                   |
| 一元表达式       | `UnaryExp`         | `PrimaryExp`<br />`Ident '(' FuncRParams ')'`<br />`Ident '(' ')'`<br />`UnaryOp UnaryExp` |
| 单目运算符       | `UnaryOp`          | `'+'`<br />`'-'`<br />`'!'`                                  |
| 函数实参表       | `FuncRParams`      | `Exp`<br />`FuncRParams ',' Exp`                             |
| 乘除模表达式     | `MulExp`           | `UnaryExp`<br />`MulExp '*' UnaryExp`<br />`MulExp '/' UnaryExp`<br />`MulExp '%' UnaryExp` |
| 加减表达式       | `AddExp`           | `MulExp`<br />`AddExp '+' MulExp`<br />`AddExp '-' MulExp`   |
| 关系表达式       | `RelExp`           | `AddExp`<br />`RelExp '<' AddExp`<br />`RelExp '<=' AddExp`<br />`RelExp '>' AddExp`<br />`RelExp '>=' AddExp` |
| 相等性表达式     | `EqExp`            | `RelExp`<br />`EqExp '==' RelExp`<br />`EqExp '!=' RelExp`   |
| 逻辑与表达式     | `LAndExp`          | `EqExp`<br />`LAndExp '&&' EqExp`                            |
| 逻辑或表达式     | `LOrExp`           | `LAndExp`<br />`LOrExp '||' LAndExp`                         |
| 常量表达式       | `ConstExp`         | `AddExp`                                                     |

