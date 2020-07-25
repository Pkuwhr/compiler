# SysY文法

原文档《[SysY语言定义](https://gitlab.eduxiji.net/nscscc/docs/-/blob/master/SysY语言定义.pdf)》中采用EBNF范式表示，为了使用Bison自动生成语法分析器，需要将其修改为基本的上下文无关文法表示法。

程序中使用二叉树存储**AST**，中间节点的左子节点表示其孩子，右子节点表示其兄弟。

| 说明             | 左部               | 右部                                                         |
| ---------------- | ------------------ | ------------------------------------------------------------ |
| 编译单元         | `CompUnit`         | `Decl`<br />`FuncDef`<br />`CompUnit Decl`<br />`CompUnit FuncDef` |
| 声明             | `Decl`             | `ConstDecl`<br />`VarDecl`                                   |
| 常量声明         | `ConstDecl`        | `'const' BType ConstDefSeq ';'`                              |
| 常量定义列表     | `ConstDefSeq`      | `ConstDef`<br />`ConstDefSeq ',' ConstDef`                   |
| 基本类型         | `BType`            | `'int'`<br />`void`                                          |
| 常数定义         | `ConstDef`         | `Ident '=' ConstInitVal`<br />`Ident ConstArraySubSeq '=' ConstInitVal` |
| 数组常量下标列表 | `ConstArraySubSeq` | `'[' Exp ']'`<br />`ConstArraySubSeq '[' Exp ']'`            |
| 数组下标列表     | `ArraySubSeq`      | `'[' Exp ']'`<br />`ArraySubSeq '[' Exp ']'`                 |
| 常量初值         | `ConstInitVal`     | `Exp`<br />`'{' ConstInitValSeq '}'`<br />`'{' '}'`          |
| 常量初值列表     | `ConstInitValSeq`  | `ConstInitVal`<br />`ConstInitValSeq ',' ConstInitVal`       |
| 变量声明         | `VarDecl`          | `BType VarDefSeq ';'`                                        |
| 变量定义列表     | `VarDefSeq`        | `VarDef`<br />`VarDefSeq ',' VarDef`                         |
| 变量定义         | `VarDef`           | `Ident`<br />`Ident ConstArraySubSeq`<br />`Ident '=' InitVal`<br />`Ident ConstArraySubSeq '=' InitVal` |
| 变量初值         | `InitVal`          | `Exp`<br />`'{' InitValSeq '}'`<br />`'{' '}'`               |
| 变量初值列表     | `InitValSeq`       | `InitVal`<br />`InitValSeq ',' InitVal`                      |
| 函数定义         | `FuncDef`          | `BType Ident '(' ')' Block`<br />`BType Ident '(' FuncFParams ')' Block` |
| 函数形参表       | `FuncFParams`      | `FuncFParam`<br />`FuncFParams ',' FuncFParam`               |
| 函数形参         | `FuncFParam`       | `BType Ident`<br />`BType Ident '[' ']' ArraySubSeq`         |
| 语句块           | `Block`            | `'{' '}'`<br />`'{' BlockItemSeq '}'`                        |
| 语句块项列表     | `BlockItemSeq`     | `BlockItem`<br />`BlockItemSeq BlockItem`                    |
| 语句块项         | `BlockItem`        | `Decl`<br />`Stmt`                                           |
| 语句             | `Stmt`             | `LVal '=' Exp ';'`<br />`Exp ';'`<br />`';'`<br />`Block`<br />`'if' '(' Exp ')' Stmt`<br />`'if' '(' Exp ')' Stmt 'else' Stmt`<br />`'while' '(' Exp ')' Stmt`<br />`'break' ';'`<br />`'continue' ';'`<br />`'return' Exp ';'`<br />`'return' ';'` |
| 表达式           | `Exp`              | `'(' Exp ')'`<br />`LVal`<br />`StringConstant`<br />`IntConstant`<br />`Ident '(' FuncRParams ')'`<br />`Ident '(' ')'`<br />`'+' Exp`<br />`'-' Exp`<br />`'!' Exp`<br />`Exp '*' Exp`<br />`Exp '/' Exp`<br />`Exp '%' Exp`<br />`Exp '+' Exp`<br />`Exp '-' Exp`<br />`Exp '<' Exp`<br />`Exp '>' Exp`<br />`Exp '<=' Exp`<br />`Exp '>=' Exp`<br />`Exp '==' Exp`<br />`Exp '!=' Exp`<br />`Exp '&&' Exp`<br />`Exp '||' Exp` |
| 左值表达式       | `LVal`             | `Ident ArraySubSeq`<br />`Ident`                             |
| 函数实参表       | `FuncRParams`      | `Exp`<br />`FuncRParams ',' Exp`                             |

