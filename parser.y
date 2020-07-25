/*
 * @Date: 2020-06-13 17:07:18
 * @LastEditors: zyk
 * @LastEditTime: 2020-07-25 02:24:50
 * @FilePath: /compiler/parser.y
 */ 


%{
    #include<stdio.h>
    #include<unistd.h>

    #include"GrammarTree.h"
    #include"Nonterminals.h"
    #include"ScopeStack.h"
    #include"ArrayInfo.h"
    
    int yylex();
    void yyerror(const char* fmt, ...);
    
    extern int tuple_trigger;
    extern int semantic_check;
    extern int gmerror;
    extern int yylineno;

    ScopeStack stack;
%}

%define parse.error verbose
%locations

%union {
    GrammarTree grammar_tree;
}

// 以下为终结符集
%token <grammar_tree> T_Void T_Int T_Const
%token <grammar_tree> T_While T_If T_Else T_Return T_Break T_Continue
%token <grammar_tree> T_LessEqual T_GreaterEqual T_Equal T_NotEqual
%token <grammar_tree> T_And T_Or
%token <grammar_tree> T_Identifier T_StringConstant T_IntConstant
%token <grammar_tree> '+' '-' '*' '/' '%' '=' '!' '<' '>'

// 用于消除if-else语句的移进/归约冲突
%token   T_NoElse
%nonassoc T_NoElse
%nonassoc T_Else

// 以下为非终结符集
%type <grammar_tree> CompUnit Decl FuncDef ConstDecl BType ConstDefSeq
%type <grammar_tree> ConstDef ConstInitVal ConstArraySubSeq
%type <grammar_tree> ArraySubSeq Exp ConstInitValSeq VarDecl VarDefSeq VarDef
%type <grammar_tree> InitVal InitValSeq Block FuncFParams
%type <grammar_tree> FuncFParam BlockItemSeq BlockItem Stmt LVal
%type <grammar_tree> FuncRParams Program

// precedence
%right '='
%left T_Or
%left T_And
%left T_Equal T_NotEqual
%left '<' '>' T_GreaterEqual T_LessEqual
%left '+' '-'
%left '*' '/' '%'
%right '!'
%right '('

%%
Program : CompUnit {
    // 1. 语法分析 建立AST
    $$ = CreateGrammarTree(Program, 1, $1); 
    // 完成创建AST 打印二元组和语法树
    if (tuple_trigger)
    {
        printf("__________________________________________________\n\n");
        printf("The two-tuples of \"Lexical Analyzing\" are printed!\n");
        printf("__________________________________________________\n");
    }
    if (!gmerror) {
        printf("\nNow print the grammar-tree of \"Grammar Analyzing\":\n");
        printf("__________________________________________________\n\n"); 
        TraverseGrammarTree($$, 0);
        printf("__________________________________________________\n\n"); 
        printf("The grammar-tree of \"Grammar Analyzing\" is printed!\n\n"); 
    }
    // 把CompUnit的GlobalScope作为Program的GlobalScope
    $$->global_scope = $1->global_scope;
    InitScopeStack(stack);
    Scope scope;
    scope.type = ScopeGlobal;
    scope.global = $$->global_scope;
    // 若semantic_check开启 则执行ScopeTrial进行静态语义检查
    if (semantic_check) 
    {
        ScopeTrial($$, scope, stack);
    }
    // 没有语义错误时 打印符号表
    if (!smerror)
    {
        printf("\nNow print the symbol tables of \"Semantic Analyzing\":\n");
        printf("__________________________________________________\n\n"); 
        DisplayGlobalScope($1->global_scope, "Program"); // TODO: impl this
        printf("__________________________________________________\n\n"); 
        printf("The symbol tables of \"Semantic Analyzing\" is printed!\n\n"); 
    }
}
;

CompUnit:
    Decl {
    $$ = CreateGrammarTree(CompUnit, 1, $1);
    // 新建一个GlobalScopeEntry 因为Decl默认为LocalScopeEntry 所以需要对其进行转换
    $$->global_scope = AddLocalIntoGlobal();
}
|   FuncDef {
    $$ = CreateGrammarTree(CompUnit, 1, $1);
    // FuncDef已经是GlobalScopeEntry 直接加入即可
    $$->global_scope = AddGlobalIntoGlobal();
}
|   CompUnit Decl {
    $$ = CreateGrammarTree(CompUnit, 2, $1, $2);
    // 将Decl转换为GlobalScopeEntry 然后连接到CompUnit上
    $$->global_scope = AddLocalIntoGlobal();
}
|   CompUnit FuncDef {
    $$ = CreateGrammarTree(CompUnit, 2, $1, $2);
    // 将FuncDef连接到CompUnit上
    $$->global_scope = AddGlobalIntoGlobal();
}
;

Decl:
    ConstDecl {
    $$ = CreateGrammarTree(Decl, 1, $1);
    // 直接赋值即可
    $$->local_scope = $1->local_scope;
}
|   VarDecl {
    $$ = CreateGrammarTree(Decl, 1, $1);
    // 直接赋值即可
    $$->local_scope = $1->local_scope;
}
;

ConstDecl:
    T_Const BType ConstDefSeq ';' {
    $$ = CreateGrammarTree(ConstDecl, 3, $1, $2, $3);
    // 为ConstDefSeq(LocalScope)中的变量添加类型const和BType
    AttachTypeToLocalScope();
}
;

ConstDefSeq:
    ConstDef {
    $$ = CreateGrammarTree(ConstDefSeq, 1, $1);
    // 新建一个LocalScope 放入ConstDef
    $$->local_scope = AddLocalIntoLocal();
}
|   ConstDefSeq ',' ConstDef {
    $$ = CreateGrammarTree(ConstDefSeq, 2, $1, $3);
    // 将ConstDef连在ConstDefSeq后
    $$->local_scope = AddLocalIntoLocal();
}
;

BType:
    T_Void {
    $$ = CreateGrammarTree(BType, 1, $1);
}
|   T_Int {
    $$ = CreateGrammarTree(BType, 1, $1);
}
;

ConstDef:
    T_Identifier '=' ConstInitVal {
    $$ = CreateGrammarTree(ConstDef, 2, $1, $3);
    // 新建一个LocalScopeEntry
    $$->local_entry = NewLocalIntEntry();
}
|   T_Identifier ConstArraySubSeq '=' ConstInitVal {
    $$ = CreateGrammarTree(ConstDef, 3, $1, $2, $4);
    // 新建一个LocalScopeEntry
    $$->local_entry = NewLocalArrayEntry();
}
;

ConstArraySubSeq:
    '[' Exp ']' {
    $$ = CreateGrammarTree(ConstArraySubSeq, 1, $2);
    // 此为第一维的大小
    $$->array_info = (ArrayInfo *)malloc(sizeof(ArrayInfo));
    // TODO: 检查Exp是否能求值为一常量
    $$->array_info->dims.clear();
    $$->array_info->dims.push_back($2->int_value);
}
| ConstArraySubSeq '[' Exp ']' {
    $$ = CreateGrammarTree(ConstArraySubSeq, 2, $1, $3);
    $$->array_info = $1->array_info;
    $$->array_info->push_back($3->int_value);
}
;

// 此项与ConstArraySubSeq的区别在于这里的Exp不要求在编译时就能计算出值
// 故这里的Exp在ArrayInfo中保存为GrammarTree*
ArraySubSeq:
    '[' Exp ']' {
    $$ = CreateGrammarTree(ArraySubSeq, 0, -1);
    // 新建一个空的选择符
    $$->array_info = (ArrayInfo *)malloc(sizeof(ArrayInfo));
    $$->array_info->exprs.clear();
    $$->array_info->exprs.push_back($2);
}
| ArraySubSeq '[' Exp ']' {
    $$ = CreateGrammarTree(ArraySubSeq, 2, $1, $3);
    // 将Exp对应的值放在选择符中
    $$->array_info = $1->array_info;
    $$->array_info->push_back($3);
}
;

ConstInitVal:
    Exp {
    $$ = CreateGrammarTree(ConstInitVal, 1, $1);
    // 计算Exp的值 这里的Exp需要能在编译时就求出值
    GetExpValue();
}
|   '{' ConstInitValSeq '}' {
    $$ = CreateGrammarTree(ConstInitVal, 1, $2);
    // TODO: 数组初始化
}
|   '{' '}' {
    $$ = CreateGrammarTree(ConstInitVal, 0, -1);
    // 这一部分的子数组初始化为全零
}
;

ConstInitValSeq:
    ConstInitVal {
    $$ = CreateGrammarTree(ConstInitValSeq, 1, $1);
}
|   ConstInitValSeq ',' ConstInitVal {
    $$ = CreateGrammarTree(ConstInitValSeq, 2, $1, $3);
}
;


VarDecl:
    BType VarDefSeq ';' {
    $$ = CreateGrammarTree(VarDecl, 2, $1, $2);
    // 为LocalScope中的变量添加类型
    AttachTypeToLocalScope();
}
;

VarDefSeq:
    VarDef {
    $$ = CreateGrammarTree(VarDefSeq, 1, $1);
}
|   VarDefSeq ',' VarDef {
    $$ = CreateGrammarTree(VarDefSeq, 2, $1, $3);
}
;

VarDef:
    T_Identifier {
    $$ = CreateGrammarTree(VarDef, 1, $1);
}
|   T_Identifier ConstArraySubSeq {
    $$ = CreateGrammarTree(VarDef, 2, $1, $2);
}
|   T_Identifier '=' InitVal {
    $$ = CreateGrammarTree(VarDef, 2, $1, $3);
} 
|   T_Identifier ConstArraySubSeq '=' InitVal {
    $$ = CreateGrammarTree(VarDef, 3, $1, $2, $4);
}
;

InitVal:
    Exp {
    $$ = CreateGrammarTree(InitVal, 1, $1);
}
|   '{' InitValSeq '}' {
    $$ = CreateGrammarTree(InitVal, 1, $2);
}
;

InitValSeq:
    InitVal {
    $$ = CreateGrammarTree(InitValSeq, 1, $1);
}
|   InitValSeq ',' InitVal {
    $$ = CreateGrammarTree(InitValSeq, 2, $1, $3);
}
;

FuncDef:
    BType T_Identifier '(' ')' Block {
    $$ = CreateGrammarTree(FuncDef, 3, $1, $2, $5);
    // 新建一个FormalScopeEntry和GlobalScopeEntry
    // FormalScope置为Null
    // LocalScopeEntry置为Block
}
|   BType T_Identifier '(' FuncFParams ')' Block {
    $$ = CreateGrammarTree(FuncDef, 4, $1, $2, $4, $6);
    // 新建一个FormalScopeEntry和GlobalScopeEntry
}
;

FuncFParams:
    FuncFParam {
    $$ = CreateGrammarTree(FuncFParams, 1, $1);
    // 新建FormalScope 加入FuncParam
}
|   FuncFParams ',' FuncFParam {
    $$ = CreateGrammarTree(FuncFParams, 2, $1, $3);
}
;

FuncFParam:
    BType T_Identifier {
    $$ = CreateGrammarTree(FuncFParam, 2, $1, $2);
    // 新建FormalScopeEntry
}
|   BType T_Identifier '[' ']' ArraySubSeq {
    $$ = CreateGrammarTree(FuncFParam, 3, $1, $2, $5);
}
;

Block:
    '{' '}' {
    $$ = CreateGrammarTree(Block, 0, -1);
    // Block直接设置为空的LocalScopeEntry
}
|   '{' BlockItemSeq '}' {
    $$ = CreateGrammarTree(Block, 1, $2);
}
;

BlockItemSeq:
    BlockItem {
    $$ = CreateGrammarTree(BlockItemSeq, 1, $1);
}
|   BlockItemSeq BlockItem {
    $$ = CreateGrammarTree(BlockItemSeq, 2, $1, $2);
}
;

BlockItem:
    Decl {
    $$ = CreateGrammarTree(BlockItem, 1, $1);
}
|   Stmt {
    $$ = CreateGrammarTree(BlockItem, 1, $1);
}
;

Stmt:
    LVal '=' Exp ';' {
    $$ = CreateGrammarTree(Stmt, 3, $1, $2, $3);
}
|   Exp ';' {
    $$ = CreateGrammarTree(Stmt, 1, $1);
}  
|   ';' {
    $$ = CreateGrammarTree(Stmt, 0, -1);
}
|   Block {
    $$ = CreateGrammarTree(Stmt, 1, $1);
}
|   T_If '(' Exp ')' Stmt %prec T_NoElse {
    $$ = CreateGrammarTree(Stmt, 3, $1, $3, $5);
}
|   T_If '(' Exp ')' Stmt T_Else Stmt {
    $$ = CreateGrammarTree(Stmt, 5, $1, $3, $5, $6, $7);
}
|   T_While '(' Exp ')' Stmt {
    $$ = CreateGrammarTree(Stmt, 3, $1, $3, $5);
}
|   T_Break ';' {
    $$ = CreateGrammarTree(Stmt, 1, $1);
}
|   T_Continue ';' {
    $$ = CreateGrammarTree(Stmt, 1, $1);
}
|   T_Return Exp ';' {
    $$ = CreateGrammarTree(Stmt, 2, $1, $2);
}
|   T_Return ';' {
    $$ = CreateGrammarTree(Stmt, 1, $1);
}
;


Exp:
    '(' Exp ')' {
    $$ = CreateGrammarTree(Exp, 1, $2);
}
|   LVal {
    $$ = CreateGrammarTree(Exp, 1, $1);
}
|   T_StringConstant {
    $$ = CreateGrammarTree(Exp, 1, $1);
}
|   T_IntConstant {
    $$ = CreateGrammarTree(Exp, 1, $1);
}
|   T_Identifier '(' FuncRParams ')' {
    $$ = CreateGrammarTree(Exp, 2, $1, $3);
}
|   T_Identifier '(' ')' {
    $$ = CreateGrammarTree(Exp, 1, $1);
}
|   '+' Exp {
    $$ = CreateGrammarTree(Exp, 2, $1, $2);
}
|   '-' Exp {
    $$ = CreateGrammarTree(Exp, 2, $1, $2);
}
|   '!' Exp {
    $$ = CreateGrammarTree(Exp, 2, $1, $2);
}
|   Exp '*' Exp {
    $$ = CreateGrammarTree(Exp, 3, $1, $2, $3);
}
|   Exp '/' Exp {
    $$ = CreateGrammarTree(Exp, 3, $1, $2, $3);
}
|   Exp '%' Exp {
    $$ = CreateGrammarTree(Exp, 3, $1, $2, $3);
}
|   Exp '+' Exp {
    $$ = CreateGrammarTree(Exp, 3, $1, $2, $3);
}
|   Exp '-' Exp {
    $$ = CreateGrammarTree(Exp, 3, $1, $2, $3);
}
|   Exp '>' Exp {
    $$ = CreateGrammarTree(Exp, 3, $1, $2, $3);
}
|   Exp '<' Exp {
    $$ = CreateGrammarTree(Exp, 3, $1, $2, $3);
}
|   Exp T_LessEqual Exp {
    $$ = CreateGrammarTree(Exp, 3, $1, $2, $3);
}
|   Exp T_GreaterEqual Exp {
    $$ = CreateGrammarTree(Exp, 3, $1, $2, $3);
}
|   Exp T_Equal Exp {
    $$ = CreateGrammarTree(Exp, 3, $1, $2, $3);
}
|   Exp T_NotEqual Exp {
    $$ = CreateGrammarTree(Exp, 3, $1, $2, $3);
}
|   Exp T_And Exp {
    $$ = CreateGrammarTree(Exp, 3, $1, $2, $3);
}
|   Exp T_Or Exp {
    $$ = CreateGrammarTree(Exp, 3, $1, $2, $3);
}
;

LVal:
    T_Identifier ArraySubSeq {
    $$ = CreateGrammarTree(LVal, 2, $1, $2);
}
;


FuncRParams:
    Exp {
    $$ = CreateGrammarTree(FuncRParams, 1, $1);
}
|   FuncRParams ',' Exp {
    $$ = CreateGrammarTree(FuncRParams, 2, $1, $3);
}
;


%%

#include<stdarg.h>

void yyerror(const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    fprintf(stderr, "Error type B at Line %d Column %d: ", yylineno, yylloc.first_column);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, ".\n");
}

