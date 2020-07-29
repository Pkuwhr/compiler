/*
 * @Date: 2020-06-13 17:07:18
 * @LastEditors: zyk
 * @LastEditTime: 2020-07-29 20:46:42
 * @FilePath: \compiler\parser.y
 */ 


%{
    #include<stdio.h>
    #include<unistd.h>

    #include"GrammarTree.h"
    #include"Nonterminals.h"
    #include"ScopeStack.h"
    #include"ArrayInfo.h"
    #include"SymbolTable.h"
    
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
Program : CompUnit { // GlobalScope
    // 1. 语法分析 建立AST
    $$ = CreateGrammarTree(Program, 1, $1); 
    // 2. 打印二元组和语法树
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

CompUnit: // GlobalScope
    Decl {
    $$ = CreateGrammarTree(CompUnit, 1, $1);
    // 新建一个GlobalScopeEntry 因为Decl默认为LocalScopeEntry 所以需要对其进行转换
    $$->global_scope = AddLocalIntoGlobal($$->global_scope, $1->local_scope);
}
|   FuncDef {
    $$ = CreateGrammarTree(CompUnit, 1, $1);
    // FuncDef已经是GlobalScopeEntry 直接加入即可
    $$->global_scope = AddEntryIntoGlobalScope($$->global_scope, $1->global_entry);
}
|   CompUnit Decl {
    $$ = CreateGrammarTree(CompUnit, 2, $1, $2);
    // 将Decl转换为GlobalScopeEntry 然后连接到CompUnit上
    $$->global_scope = AddLocalIntoGlobal($1->global_scope, $2->local_scope);
}
|   CompUnit FuncDef {
    $$ = CreateGrammarTree(CompUnit, 2, $1, $2);
    // 将FuncDef连接到CompUnit上
    $$->global_scope = AddEntryIntoGlobalScope($1->global_scope, $2->global_entry);
}
;

Decl: // LocalScope
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

ConstDecl: // LocalScope
    T_Const BType ConstDefSeq ';' {
    $$ = CreateGrammarTree(ConstDecl, 3, $1, $2, $3);
    // 为ConstDefSeq(LocalScope)中的变量添加类型isConst
    $$->local_scope = AttachTypeToLocalScope($3->local_scope, /* isConst */ true);
}
;

ConstDefSeq: // LocalScope
    ConstDef {
    $$ = CreateGrammarTree(ConstDefSeq, 1, $1);
    // 新建一个LocalScope 放入ConstDef
    $$->local_scope = AddEntryIntoLocalScope($$->local_scope, $1->local_entry);
}
|   ConstDefSeq ',' ConstDef {
    $$ = CreateGrammarTree(ConstDefSeq, 2, $1, $3);
    // 将ConstDef连在ConstDefSeq后
    $$->local_scope = AddEntryIntoLocalScope($1->local_scope, $3->local_entry);
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

ConstDef: // LocalScopeEntry
    T_Identifier '=' ConstInitVal {
    $$ = CreateGrammarTree(ConstDef, 2, $1, $3);
    // 新建一个LocalScopeEntry
    $$->local_entry = NewLocalIntEntry($1->string_value, false, false, NewInitValue($3->int_value), NULL);
}
|   T_Identifier ConstArraySubSeq '=' ConstInitVal {
    $$ = CreateGrammarTree(ConstDef, 3, $1, $2, $4);
    // 新建一个LocalScopeEntry
    $$->local_entry = NewLocalArrayEntry($1->string_value, 
                                         true, false, 
                                         NewArrayInfo($2->dims, NULL, $4->array_init_value), 
                                         NULL);
}
;

ConstArraySubSeq: // dims
    '[' Exp ']' {
    $$ = CreateGrammarTree(ConstArraySubSeq, 1, $2);
    // 此为第一维的大小
    CheckExprValue($2, stack);
    if (!$2->is_constant_expr) {
        yyerror("Line %d: ConstExp needed!", yylineno);
    }
    $$->dims = AddIntoDimsVector($$->dims, $2->expr_value);
}
| ConstArraySubSeq '[' Exp ']' {
    $$ = CreateGrammarTree(ConstArraySubSeq, 2, $1, $3);
    CheckExprValue($3, stack);
    if (!$3->is_constant_expr) {
        yyerror("Line %d: ConstExp needed!", yylineno);
    }
    $$->dims = AddIntoDimsVector($1->dims, $3->expr_value);
}
;

// 此项与ConstArraySubSeq的区别在于这里的Exp不要求在编译时就能计算出值
// 故这里的Exp在ArrayInfo中保存为GrammarTree*
ArraySubSeq: // exprs
    '[' Exp ']' {
    $$ = CreateGrammarTree(ArraySubSeq, 0, -1);
    // 新建一个空的选择符
    $$->exprs = AddIntoExprsVector($$->exprs, $2);
}
| ArraySubSeq '[' Exp ']' {
    $$ = CreateGrammarTree(ArraySubSeq, 2, $1, $3);
    // 将Exp对应的值放在选择符中
    $$->exprs = AddIntoExprsVector($1->exprs, $3);
}
;

ConstInitVal: // array_init_value
    Exp {
    $$ = CreateGrammarTree(ConstInitVal, 1, $1);
    // 计算Exp的值 这里的Exp需要能在编译时就求出值
    CheckExprValue($1, stack);
    if (!$1->is_constant_expr) {
        yyerror("Line %d: ConstExp needed!\n", yylineno);
    }
    $$->array_init_value = AddExprIntoArrayInitValue($$->array_init_value, $1->expr_value, false);
}
|   '{' ConstInitValSeq '}' {
    $$ = CreateGrammarTree(ConstInitVal, 1, $2);
    $$->array_init_value = MergeArrayInitValue($$->array_init_value, $2->array_init_value);
}
|   '{' '}' {
    $$ = CreateGrammarTree(ConstInitVal, 0, -1);
    // 这一部分的子数组初始化为全零
    $$->array_init_value = MergeArrayInitValue($$->array_init_value, NULL);
}
;

ConstInitValSeq: // array_init_value
    ConstInitVal {
    $$ = CreateGrammarTree(ConstInitValSeq, 1, $1);
    $$->array_init_value = MergeArrayInitValue($$->array_init_value, $1->array_init_value);
}
|   ConstInitValSeq ',' ConstInitVal {
    $$ = CreateGrammarTree(ConstInitValSeq, 2, $1, $3);
    $$->array_init_value = MergeArrayInitValue($1->array_init_value, $3->array_init_value);
}
;


VarDecl: // LocalScope
    BType VarDefSeq ';' {
    $$ = CreateGrammarTree(VarDecl, 2, $1, $2);
    $$->local_scope = AttachTypeToLocalScope($2->local_scope, false);
}
;

VarDefSeq: // LocalScope
    VarDef {
    $$ = CreateGrammarTree(VarDefSeq, 1, $1);
    $$->local_scope = AddEntryIntoLocalScope($$->local_scope, $1->local_entry);
}
|   VarDefSeq ',' VarDef {
    $$ = CreateGrammarTree(VarDefSeq, 2, $1, $3);
    $$->local_scope = AddEntryIntoLocalScope($1->local_scope, $3->local_entry);
}
;

VarDef: // LocalScopeEntry
    T_Identifier {
    $$ = CreateGrammarTree(VarDef, 1, $1);
    $$->local_entry = NewLocalEntry($1->string_value, false, false, NULL, NULL);
}
|   T_Identifier ConstArraySubSeq {
    $$ = CreateGrammarTree(VarDef, 2, $1, $2);
    $$->local_entry = NewLocalEntry($1->string_value, true, false, NewArrayInfo($2->dims, NULL, NULL), NULL);
}
|   T_Identifier '=' InitVal {
    $$ = CreateGrammarTree(VarDef, 2, $1, $3);
    if ($3->child->type != Exp) {
        yyerror("Line %d: InitVal must be Exp type!\n", yylineno);
    }
    $$->local_entry = NewLocalEntry($1->string_value, false, false, NewInitValue(false, 0, $3->lchild), NULL);
} 
|   T_Identifier ConstArraySubSeq '=' InitVal {
    $$ = CreateGrammarTree(VarDef, 3, $1, $2, $4);
    $$->local_entry = NewLocalEntry($1->string_value, true, false, NewArrayInfo($2->dims, $4->exprs, NULL), NULL);
}
;

InitVal: // array_init_value
    Exp {
    $$ = CreateGrammarTree(InitVal, 1, $1);
    // TODO: 记录exp
}
|   '{' InitValSeq '}' {
    $$ = CreateGrammarTree(InitVal, 1, $2);
}
;

InitValSeq: // array_init_value
    InitVal {
    $$ = CreateGrammarTree(InitValSeq, 1, $1);
}
|   InitValSeq ',' InitVal {
    $$ = CreateGrammarTree(InitValSeq, 2, $1, $3);
}
;

FuncDef: // GlobalScopeEntry
    BType T_Identifier '(' ')' Block {
    $$ = CreateGrammarTree(FuncDef, 3, $1, $2, $5);
    // 新建一个FormalScopeEntry和GlobalScopeEntry
    // FormalScope置为Null
    // LocalScopeEntry置为Block
    $$->global_entry = NewGlobalEntry($2->string_value, true, $1->lchild->type == T_Void, 
                                      NULL, 0, NULL, $5->local_scope);
}
|   BType T_Identifier '(' FuncFParams ')' Block {
    $$ = CreateGrammarTree(FuncDef, 4, $1, $2, $4, $6);
    // 新建一个FormalScopeEntry和GlobalScopeEntry
    $$->global_entry = NewGlobalEntry($2->string_value, true, $1->lchild->type == T_Void,
                                      NULL, $4->formal_scope->size(), $4->formal_scope, $6->local_scope);
}
;

FuncFParams: // FormalScope
    FuncFParam {
    $$ = CreateGrammarTree(FuncFParams, 1, $1);
    // 新建FormalScope 加入FuncParam
    $$->formal_scope = AddEntryIntoFormalScope($$->formal_scope, $1->formal_entry);
}
|   FuncFParams ',' FuncFParam {
    $$ = CreateGrammarTree(FuncFParams, 2, $1, $3);
    $$->formal_scope = AddEntryIntoFormalScope($1->formal_scope, $3->formal_entry);
}
;

FuncFParam: // FormalScopeEntry
    BType T_Identifier {
    $$ = CreateGrammarTree(FuncFParam, 2, $1, $2);
    // 新建FormalScopeEntry
    $$->formal_entry = NewFormalEntry($2->string_value, false, NULL);
}
|   BType T_Identifier '[' ']' ArraySubSeq {
    $$ = CreateGrammarTree(FuncFParam, 3, $1, $2, $5);
    $$->formal_entry = NewFormalEntry($2->string_value, true, NewArrayInfo($5->dims, NULL, NULL));
}
;

Block: // LocalScopeEntry
    '{' '}' {
    $$ = CreateGrammarTree(Block, 0, -1);
    // Block直接设置为空的LocalScope
    $$->local_scope = NULL;
}
|   '{' BlockItemSeq '}' {
    $$ = CreateGrammarTree(Block, 1, $2);
    $$->local_scope = $2->local_scope;
}
;

BlockItemSeq: // LocalScope
    BlockItem {
    $$ = CreateGrammarTree(BlockItemSeq, 1, $1);
    $$->local_scope = AddLocalIntoLocal($$->local_scope, $1->local_scope);
}
|   BlockItemSeq BlockItem {
    $$ = CreateGrammarTree(BlockItemSeq, 2, $1, $2);
    $$->local_scope = AddLocalIntoLocal($1->local_scope, $2->local_scope);
    
}
;

BlockItem: // LocalScope
    Decl {
    $$ = CreateGrammarTree(BlockItem, 1, $1);
    $$->local_scope = $1->local_scope;
}
|   Stmt {
    // 直接在这里处理Stmt中的Block
    $$ = CreateGrammarTree(BlockItem, 1, $1);
    // 1. Stmt -> Block
    if ($$->lchild->type == Block) {
        // add 1 entry
        LocalScopeEntry *blk = NewLocalEntry(NULL, false, true, NULL, $$->lchild->local_scope);
        $$->local_scope = AddEntryIntoLocalScope($$->local_scope, blk);
    }
    // 2. Stmt -> T_If '(' Exp ')' Stmt( -> Block)
    else if ($$->lchild->type == T_If &&
             $$->lchild->rchild->rchild->rchild == NULL) {
        // add 1 entry
        LocalScopeEntry *if_blk = NewLocalEntry(NULL, false, true, NULL, $$->lchild->rchild->rchild->lchild->local_scope);
        $$->local_scope = AddEntryIntoLocalScope($$->local_scope, if_blk);
    }
    // 3. Stmt -> T_If '(' Exp ')' Stmt T_Else Stmt
    else if ($$->lchild->type == T_If &&
             $$->lchild->rchild->rchild->rchild == T_Else) {
        // add 2 entries
        LocalScopeEntry *if_blk = NewLocalEntry(NULL, false, true, NULL, $$->lchild->rchild->rchild->lchild->local_scope);
        LocalScopeEntry *else_blk = NewLocalEntry(NULL, false, true, NULL, $$->lchild->rchild->rchild->rchild->rchild->lchild->local_scope);
        $$->local_scope = AddEntryIntoLocalScope($$->local_scope, if_blk);
        $$->local_scope = AddEntryIntoLocalScope($$->local_scope, else_blk);
    }
    // 4. Stmt -> T_While '(' Exp ')' Stmt
    else if ($$->lchild->type == T_While) {
        // add 1 entry
        LocalScopeEntry *while_blk = NewLocalEntry(NULL, false, true, NULL, $$->lchild->rchild->rchild->lchild->local_scope);
        $$->local_scope = AddEntryIntoLocalScope($$->local_scope, while_blk);
    }
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
|   T_Identifier {
    $$ = CreateGrammarTree(LVal, 1, $1);
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

