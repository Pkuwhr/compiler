/*
 * @Date: 2020-06-13 17:07:18
 * @LastEditors: zyk
 * @LastEditTime: 2020-06-13 22:11:26
 * @FilePath: \compiler\GrammarTree.y
 */ 


%{
    #include<stdio.h>
    #include<unistd.h>
    #include"GrammarTree.h"
    #include"lex.yy.c"
    void yyerror(const char* fmt, ...);
%}

%error-verbose
%locations

%union {
    GrammarTree grammar_tree;
}

// 以下为终结符集
%token <grammar_tree> T_Void T_Int T_Const
%token <grammar_tree> T_While T_If T_Else T_Return T_Break T_Continue
%token <grammar_tree> T_LessEqual T_GreaterEqual T_Equal T_NotEqual T_Dims
%token <grammar_tree> T_And T_Or
%token <grammar_tree> T_Identifier T_StringConstant T_IntConstant

// 用于消除if-else语句的移进/归约冲突
%token   T_NoElse
%nonassoc T_NoElse
%nonassoc T_Else

// 以下为非终结符集
%type <grammar_tree> CompUnit Decl FuncDef ConstDecl BType ConstDefSeq
%type <grammar_tree> ConstDef ConstInitVal ConstArraySubSeq ConstExp
%type <grammar_tree> ArraySubSeq Exp ConstInitValSeq VarDecl VarDefSeq VarDef
%type <grammar_tree> InitVal InitValSeq Block FuncFParams
%type <grammar_tree> FuncFParam BlockItemSeq BlockItem Stmt LVal Cond
%type <grammar_tree> AddExp LOrExp Number PrimaryExp UnaryOp UnaryExp
%type <grammar_tree> FuncRParams MulExp RelExp EqExp LAndExp



%%
Program : CompUnit {
    //
}
;

CompUnit:
    Decl {
    // 
}
|   FuncDef {
    //
}
|   CompUnit Decl {
    //
}
|   CompUnit FuncDef {
    //
}
;

Decl:
    ConstDecl {
    //
}
|   VarDecl {
    //
}
;

ConstDecl:
    T_Const BType ConstDefSeq ';' {
    //
}
;

ConstDefSeq:
    ConstDef {
    //
}
|   ConstDefSeq ',' ConstDef {
    //
}
;

BType:
    T_Void {
    //
}
|   T_Int {
    //
}
;

ConstDef:
    T_Identifier '=' ConstInitVal {
    //
}
|   T_Identifier ConstArraySubSeq '=' ConstInitVal {
    //
}
;

ConstArraySubSeq:
    '[' ConstExp ']' {
    //
}
| ConstArraySubSeq '[' ConstExp ']' {
    //
}
;

ArraySubSeq:
    '[' Exp ']' {
    //
}
| ArraySubSeq '[' Exp ']' {
    //
}
;

ConstInitVal:
    ConstExp {
    //
}
|   '{' ConstInitValSeq '}' {
    //
}
|   '{' '}' {
    //
}
;

ConstInitValSeq:
    ConstInitVal {
    //
}
|   ConstInitValSeq ',' ConstInitVal {
    //
}
;

VarDecl:
    BType VarDefSeq ';' {
    //
}
;

VarDefSeq:
    VarDef {
    //
}
|   VarDefSeq ',' VarDef {
    //
}
;

VarDef:
    T_Identifier {
    //
}
|   T_Identifier ConstArraySubSeq {
    //
}
|   T_Identifier '=' InitVal {
    //
} 
|   T_Identifier ConstArraySubSeq '=' InitVal {
    //
}
;

InitVal:
    Exp {
    //    
}
|   '{' InitValSeq '}' {
    //
}
;

InitValSeq:
    InitVal {
    //
}
|   InitValSeq ',' InitVal {
    //
}
;

FuncDef:
    BType T_Identifier '(' ')' Block {
    //
}
|   BType T_Identifier '(' FuncFParams ')' Block {
    //
}
;

FuncFParams:
    FuncFParam {
    //
}
|   FuncFParams ',' FuncFParam {
    //
}
;

FuncFParam:
    BType T_Identifier {
    //
}
|   BType T_Identifier '[' ']' ArraySubSeq {
    //
}
;

Block:
    '{' '}' {
    //
}
|   '{' BlockItemSeq '}' {
    //
}
;

BlockItemSeq:
    BlockItem {
    //
}
|   BlockItemSeq BlockItem {
    //
}
;

BlockItem:
    Decl {
    //
}
|   Stmt {
    //
}
;

Stmt:
    LVal '=' Exp ';' {
    //
}
|   Exp ';' {
    //
}  
|   ';' {
    //
}
|   Block {
    //
}
|   T_If '(' Cond ')' Stmt %prec T_NoElse {
    //
}
|   T_If '(' Cond ')' Stmt T_Else Stmt {
    //
}
|   T_While '(' Cond ')' Stmt {
    //
}
|   T_Break ';' {
    //
}
|   T_Continue ';' {
    //
}
|   T_Return Exp ';' {
    //
}
|   T_Return ';' {
    //
}
;

Exp:
    AddExp {
    //
}
;

Cond:
    LOrExp {
    //
}
;

LVal:
    T_Identifier ArraySubSeq {
    //
}
;

PrimaryExp:
    '(' Exp ')' {
    //
}
|   LVal {
    //
}
|   Number {
    //
}
;

Number:
    T_IntConstant {
    //
}
;

UnaryExp:
    PrimaryExp {
    //
}
|   T_Identifier '(' FuncRParams ')' {
    //
}
|   T_Identifier '(' ')' {
    //
}
|   UnaryOp UnaryExp {
    //
}
;

UnaryOp:
    '+' {
    //
}
|   '-' {
    //
}
|   '!' {
    //
}
;

FuncRParams:
    Exp {
    //
}
|   FuncRParams ',' Exp {
    //
}
;

MulExp:
    UnaryExp {
    //
}
|   MulExp '*' UnaryExp {
    //
}
|   MulExp '/' UnaryExp {
    //
}
|   MulExp '%' UnaryExp {
    //
}
;

AddExp:
    MulExp {
    //
}
|   AddExp '+' MulExp {
    //
}
|   AddExp '-' MulExp {
    //
}
;

RelExp:
    AddExp {
    //
}
|   RelExp '<' AddExp {
    //
}
|   RelExp T_LessEqual AddExp {
    //
}
|   RelExp '>' AddExp {
    //
}
|   RelExp T_GreaterEqual AddExp {
    //
}
;
 
EqExp:
    RelExp {
    //
}
|   EqExp T_Equal RelExp {
    //
}
|   EqExp T_NotEqual RelExp {
    //
}
;

LAndExp:
    EqExp {
    //
}
|   LAndExp T_And EqExp {
    //
}
;

LOrExp:
    LAndExp {
    //
}
|   LOrExp T_Or LAndExp {
    //
}
;

ConstExp:
    AddExp {
    //
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

