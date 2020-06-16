/*
 * @Date: 2020-06-13 17:07:18
 * @LastEditors: zyk
 * @LastEditTime: 2020-06-15 21:08:51
 * @FilePath: \compiler\parser.y
 */ 


%{
    #include<stdio.h>
    #include<unistd.h>
    #include"GrammarTree.h"
    #include"Nonterminals.h"
    void yyerror(const char* fmt, ...);
    extern int tuple_trigger;
%}

%error-verbose
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
%type <grammar_tree> ConstDef ConstInitVal ConstArraySubSeq ConstExp
%type <grammar_tree> ArraySubSeq Exp ConstInitValSeq VarDecl VarDefSeq VarDef
%type <grammar_tree> InitVal InitValSeq Block FuncFParams
%type <grammar_tree> FuncFParam BlockItemSeq BlockItem Stmt LVal Cond
%type <grammar_tree> AddExp LOrExp Number PrimaryExp UnaryOp UnaryExp
%type <grammar_tree> FuncRParams MulExp RelExp EqExp LAndExp Program



%%
Program : CompUnit {
    $$ = CreateGrammarTree(Program, 1, $1); 
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
}
;

CompUnit:
    Decl {
    $$ = CreateGrammarTree(CompUnit, 1, $1);
}
|   FuncDef {
    $$ = CreateGrammarTree(CompUnit, 1, $1);
}
|   CompUnit Decl {
    $$ = CreateGrammarTree(CompUnit, 2, $1, $2);
}
|   CompUnit FuncDef {
    $$ = CreateGrammarTree(CompUnit, 2, $1, $2);
}
;

Decl:
    ConstDecl {
    $$ = CreateGrammarTree(Decl, 1, $1);
}
|   VarDecl {
    $$ = CreateGrammarTree(Decl, 1, $1);
}
;

ConstDecl:
    T_Const BType ConstDefSeq ';' {
    $$ = CreateGrammarTree(ConstDecl, 3, $1, $2, $3);
}
;

ConstDefSeq:
    ConstDef {
    $$ = CreateGrammarTree(ConstDefSeq, 1, $1);
}
|   ConstDefSeq ',' ConstDef {
    $$ = CreateGrammarTree(ConstDefSeq, 2, $1, $3);
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
}
|   T_Identifier ConstArraySubSeq '=' ConstInitVal {
    $$ = CreateGrammarTree(ConstDef, 3, $1, $2, $4);
}
;

ConstArraySubSeq:
    '[' ConstExp ']' {
    $$ = CreateGrammarTree(ConstArraySubSeq, 1, $2);
}
| ConstArraySubSeq '[' ConstExp ']' {
    $$ = CreateGrammarTree(ConstArraySubSeq, 2, $1, $3);
}
;

ArraySubSeq:
     {
    $$ = CreateGrammarTree(ArraySubSeq, 0);
}
| ArraySubSeq '[' Exp ']' {
    $$ = CreateGrammarTree(ArraySubSeq, 2, $1, $3);
}
;

ConstInitVal:
    ConstExp {
    $$ = CreateGrammarTree(ConstInitVal, 1, $1);
}
|   '{' ConstInitValSeq '}' {
    $$ = CreateGrammarTree(ConstInitVal, 1, $2);
}
|   '{' '}' {
    $$ = CreateGrammarTree(ConstInitVal, 0); // FIXME: 检查cgt如何应对空规则
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
}
|   BType T_Identifier '(' FuncFParams ')' Block {
    $$ = CreateGrammarTree(FuncDef, 4, $1, $2, $4, $6);
}
;

FuncFParams:
    FuncFParam {
    $$ = CreateGrammarTree(FuncFParams, 1, $1);
}
|   FuncFParams ',' FuncFParam {
    $$ = CreateGrammarTree(FuncFParams, 2, $1, $3);
}
;

FuncFParam:
    BType T_Identifier {
    $$ = CreateGrammarTree(FuncFParam, 2, $1, $2);
}
|   BType T_Identifier '[' ']' ArraySubSeq {
    $$ = CreateGrammarTree(FuncFParam, 3, $1, $2, $5);
}
;

Block:
    '{' '}' {
    $$ = CreateGrammarTree(Block, 0); // FIXME:
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
    $$ = CreateGrammarTree(Stmt, 0);
}
|   Block {
    $$ = CreateGrammarTree(Stmt, 1, $1);
}
|   T_If '(' Cond ')' Stmt %prec T_NoElse {
    $$ = CreateGrammarTree(Stmt, 3, $1, $3, 5);
}
|   T_If '(' Cond ')' Stmt T_Else Stmt {
    $$ = CreateGrammarTree(Stmt, 5, $1, $3, $5, $6, $7);
}
|   T_While '(' Cond ')' Stmt {
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
    AddExp {
    $$ = CreateGrammarTree(Exp, 1, $1);
}
;

Cond:
    LOrExp {
    $$ = CreateGrammarTree(Cond, 1, $1);
}
;

LVal:
    T_Identifier ArraySubSeq {
    $$ = CreateGrammarTree(LVal, 2, $1, $2);
}
;

PrimaryExp:
    '(' Exp ')' {
    $$ = CreateGrammarTree(PrimaryExp, 1, $2);
}
|   LVal {
    $$ = CreateGrammarTree(PrimaryExp, 1, $1);
}
|   Number {
    $$ = CreateGrammarTree(PrimaryExp, 1, $1);
}
;

Number:
    T_IntConstant {
    $$ = CreateGrammarTree(Number, 1, $1);
}
;

UnaryExp:
    PrimaryExp {
    $$ = CreateGrammarTree(UnaryExp, 1, $1);
}
|   T_Identifier '(' FuncRParams ')' {
    $$ = CreateGrammarTree(UnaryExp, 2, $1, $3);
}
|   T_Identifier '(' ')' {
    $$ = CreateGrammarTree(UnaryExp, 1, $1);
}
|   UnaryOp UnaryExp {
    $$ = CreateGrammarTree(UnaryExp, 2, $1, $2);
}
;

UnaryOp:
    '+' {
    $$ = CreateGrammarTree(UnaryOp, 1, $1);
}
|   '-' {
    $$ = CreateGrammarTree(UnaryOp, 1, $1);
}
|   '!' {
    $$ = CreateGrammarTree(UnaryOp, 1, $1);
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

MulExp:
    UnaryExp {
    $$ = CreateGrammarTree(MulExp, 1, $1);
}
|   MulExp '*' UnaryExp {
    $$ = CreateGrammarTree(MulExp, 3, $1, $2, $3);
}
|   MulExp '/' UnaryExp {
    $$ = CreateGrammarTree(MulExp, 3, $1, $2, $3);
}
|   MulExp '%' UnaryExp {
    $$ = CreateGrammarTree(MulExp, 3, $1, $2, $3);
}
;

AddExp:
    MulExp {
    $$ = CreateGrammarTree(AddExp, 1, $1);
}
|   AddExp '+' MulExp {
    $$ = CreateGrammarTree(AddExp, 3, $1, $2, $3);
}
|   AddExp '-' MulExp {
    $$ = CreateGrammarTree(AddExp, 3, $1, $2, $3);
}
;

RelExp:
    AddExp {
    $$ = CreateGrammarTree(RelExp, 1, $1);
}
|   RelExp '<' AddExp {
    $$ = CreateGrammarTree(RelExp, 3, $1, $2, $3);
}
|   RelExp T_LessEqual AddExp {
    $$ = CreateGrammarTree(RelExp, 3, $1, $2, $3);
}
|   RelExp '>' AddExp {
    $$ = CreateGrammarTree(RelExp, 3, $1, $2, $3);
}
|   RelExp T_GreaterEqual AddExp {
    $$ = CreateGrammarTree(RelExp, 3, $1, $2, $3);
}
;
 
EqExp:
    RelExp {
    $$ = CreateGrammarTree(EqExp, 1, $1);
}
|   EqExp T_Equal RelExp {
    $$ = CreateGrammarTree(EqExp, 3, $1, $2, $3);
}
|   EqExp T_NotEqual RelExp {
    $$ = CreateGrammarTree(EqExp, 3, $1, $2, $3);
}
;

LAndExp:
    EqExp {
    $$ = CreateGrammarTree(LAndExp, 1, $1);
}
|   LAndExp T_And EqExp {
    $$ = CreateGrammarTree(LAndExp, 3, $1, $2, $3);
}
;

LOrExp:
    LAndExp {
    $$ = CreateGrammarTree(LOrExp, 1, $1);
}
|   LOrExp T_Or LAndExp {
    $$ = CreateGrammarTree(LOrExp, 3, $1, $2, $3);
}
;

ConstExp:
    AddExp {
    $$ = CreateGrammarTree(ConstExp, 1, $1);
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

