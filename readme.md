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



## EBNF

```
编译单元		CompUnit	→	DeclList 
声明列表		DeclList		→	DeclList Decl | Decl 
声明		Decl		→	ConstDecl | VarDecl | FnDecl

常量声明		ConstDecl		→	'const' Type ConstDefList ';' 
基本类型		Type		→	'int' | 'void'                               // void 只会在 function 之前出现
常量定义列表	ConstDefList	→	ConstDef | ConstDefList ',' ConstDef

常数定义		ConstDef		→	Identifier '=' ConstInitVal        // 这里 ArrayCExpList 可以为空
                                                                    				// ArrayCExpList 在读入时忽略第一个参数，入a[3][4],忽略3只读入4
数组参数列表	ArrayCExpList	→	/* 可以为空 */ | ArrayCExpList '['Exp']'

常量初值		ConstInitVal	→	Exp  | '{' ConstInitValList '}'
常量初值列表	ConstInitValList	→	ConstInitVal  | ConstInitValList ',' ConstInitVal | /* 可以为空 */

变量声明		VarDecl		→	Var ';' 
变量		Var		→	Type VarDefList
变量定义列表	VarDefList	→	VarDef | VarDefList ',' VarDef

变量定义		VarDef		→	Identifier | Identifier '=' InitVal 

变量初值		InitVal		→	Exp | '{' InitValList '}'
变量初值列表	InitValList		→	InitVal  | InitValList ',' InitVal |  //可以为空


函数定义		FnDecl		→	Type Identifier '(' FuncFParamList ')' Block   
函数形参表	FuncFParamList	→	FuncFParam | FuncFParamList ',' FuncFParam |  //可以为空

函数形参		FuncFParam	→	Type Identifier

语句块		Block		→	'{' BlockItemList '}'
语句块项列表	BlockItemList	→	BlockItemList BlockItem | //可以为空
语句块项		BlockItem		→	Decl | Stmt 


语句		Stmt		→	LVal '=' Exp ';'
                          | ';' | Exp ';' | Block 
                          | 'if' '(' Cond ')' Stmt 
                          | 'if' '(' Cond ')' Stmt 'else' Stmt
                          | 'while' '(' Cond ')' Stmt 
                          | 'break' ';'  
                          | 'continue' ';' 
                          | 'return' ';' 
                          | 'return' Exp ';'

表达式		Exp                → AddExp   // SysY 表达式是 int 型表达式 
条件表达式	Cond           → LOrExp  

左值表达式	LVal           → Identifier
标识符      Identifier  →   T_Identifier | Identifier ArrayCExpList | Identifier '[' ']' ArrayCExpList

基本表达式	PrimaryExp     → '(' Exp ')' | LVal | Number | String

数值		Number               → T_IntConstant 
字符串      String          →   T_StringConstant
一元表达式	UnaryExp       → PrimaryExp | Identifier '(' FuncRParamList ')'  | UnaryOp UnaryExp 
单目运算符	UnaryOp        → '+' | '−' | '!'    注：'!'仅出现在条件表达式中 
函数实参表	FuncRParamList → Exp FuncRParamList  |          //可以为空 

乘除模表达式	MulExp       → UnaryExp | MulExp '*' UnaryExp | MulExp '/' UnaryExp | MulExp '%' UnaryExp 
加减表达式	AddExp         → MulExp | AddExp '+' MulExp | AddExp '-' MulExp 
关系表达式	RelExp         → AddExp | RelExp ('<' | '>' | '<=' | '>=') AddExp 
相等性表达式	EqExp        → RelExp | EqExp ('==' | '!=') RelExp 
逻辑与表达式	LAndExp      → EqExp | LAndExp '&&' EqExp 
逻辑或表达式	LOrExp       → LAndExp | LOrExp '||' LAndExp 

说明：
尽量去掉{}和[]匹配，所有{}匹配用List表示，在动作中同样也建立一个相应对象的List，有关List的详细信息参见list.h
非终结符都有其对应的类，使用new创建相应对象，在构造函数中自动完成ast搭建

关于array的声明中Type只记录其元素类型，数组大小记录在Identifier中
Identifier是非终结符 T_Identifier是终结符
```

