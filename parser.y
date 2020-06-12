/*
 * @Author: zyk
 * @Date: 2020-05-31 19:00:41
 * @LastEditTime: 2020-06-12 21:19:50
 * @LastEditors: zyk
 * @Description: Bison input file to generate the parser for the compiler.
 * @FilePath: \compiler\parser.y
 */ 

%{

#include "scanner.h" // for yylex
#include "parser.h"
#include "errors.h" // TODO: add or remove this header

#define TYPE_INT 1
#define TYPE_CONST 2
#define TYPE_VOID 3

void yyerror(const char *msg); // standard error-handling routine

%}
 
/* yylval 
 * ------
 * Here we define the type of the yylval global variable that is used by
 * the scanner to store attibute information about the token just scanned
 * and thus communicate that information to the parser. 
 */

%union {
    // terminals
    int integerConstant;
    char *stringConstant;
    char t_identifier[MaxIdentLen+1]; // +1 for terminating null

    // ast_decl.h
    List<Def *> *defList;
    Def *def;
    List<Decl *> *declList;
    Decl *decl;
    VarDecl *varDecl;
    FnDecl *fnDecl;

    // ast_type.h
    Type *type;
    
    // ast.h
    Identifier *identifier;
    // ast_expr.h
    List<Expr *> *exprList;
    Expr *expr;
    IntConstant *intConstant;
    StringConstant *stringConstant;
    Operator *operator;
    BinaryExpr *binaryExpr;
    LValue *lvalue;
    UnaryExpr *unaryExpr;
    ArithmeticExpr *arithmeticExpr;
    RelationalExpr *relationalExpr;
    EqualityExpr *equalityExpr;
    LogicalExpr *logicalExpr;
    AssignExpr *assignExpr;
    Call *call;
    InitValue* initValue;
    List<InitValue *> *initValueList;
    // ast_block.h
    Block *block;
    BlockItem *blockItem;
    List<BlockItem *> *blockItemList;
    Stmt *stmt;
    ConditionalStmt *condStmt;
    WhileStmt *whileStmt;
    IfStmt *ifStmt;
    BreakStmt *breakStmt;
    ReturnStmt *returnStmt;
    ContinueStmt *continueStmt;
}


/* Tokens
 * ------
 * Here we tell yacc about all the token types that we are using.
 * Bison will assign unique numbers to these and export the #define
 * in the generated y.tab.h header file.
 */

%token   T_Void T_Int T_Const
%token   T_While T_If T_Else T_Return T_Break T_Continue
%token   T_LessEqual T_GreaterEqual T_Equal T_NotEqual T_Dims
%token   T_And T_Or


%token   <t_identifier> T_Identifier
%token   <stringConstant> T_StringConstant 
%token   <integerConstant> T_IntConstant


// these are virtual tokens that aren't really parsed.
// used for precedence operations
%token   T_UnaryMinus
%token   T_NoElse

/* Non-terminal types
 * ------------------
 * In order for yacc to assign/access the correct field of $$, $1, we
 * must to declare which field is appropriate for the non-terminal.
 * As an example, this first type declaration establishes that the DeclList
 * non-terminal uses the field named "declList" in the yylval union. This
 * means that when we are setting $$ for a reduction for DeclList ore reading
 * $n which corresponds to a DeclList nonterminal we are accessing the field
 * of the union named "declList" which is of type List<Decl*>.
 * pp2: You'll need to add many of these of your own.
 */

%type <declList>      DeclList 
%type <decl>          Decl
%type <varDecl>       ConstDecl
%type <varDecl>       VarDecl
%type <fnDecl>        FnDecl
%type <type>          Type
%type <defList>       ConstDefList
%type <def>           ConstDef
%type <exprList>      ConstInitVal
%type <initValue>     ConstInitValList
%type <identifier>    Identifier
%type <exprList>      ArrayCExpList
%type <expr>          Exp
%type <varDecl>       Var
%type <defList>       VarDefList
%type <def>           VarDef
%type <exprList>      InitVal
%type <initValue>     InitValList
%type <defList>       FuncFParamList
%type <def>           FuncFParam
%type <block>         Block
%type <blockItemList> BlockItemList
%type <blockItem>     BlockItem
%type <stmt>          Stmt
// %type <ifStmt>        
// %type <whileStmt>
// %type <breakStmt>
// %type <returnStmt>
// %type <continueStmt>
%type <lValue>        Lval
%type <condStmt>      Cond
%type <operator>      Operator
%type <arithmeticExpr>AddExp
%type <logicalExpr>   LOrExp
%type <expr>          PrimaryExp
%type <intConstant>   Number
%type <unaryExpr>     UnaryExp
%type <exprList>      FuncRParamList
%type <Operator>      UnaryOp
%type <arithmeticExpr>MulExp
%type <relationalExpr>RelExp
%type <equalityExpr>  EqExp
%type <logicalExpr>   LAndExp


/* Precedence Assignment
 * ---------------------
 * Here we do some precedence assignments. Ones at the bototm have higher
 * priority than those at the top 
 */

%nonassoc   '='
%left       T_Or
%left       T_And
%nonassoc   T_Equal T_NotEqual
%nonassoc   '<' T_LessEqual '>' T_GreaterEqual
%left       '+' '-'
%left       '*' '/' '%'
%nonassoc   '!' T_UnaryMinus
%nonassoc   '['
%nonassoc   T_NoElse
%nonassoc   T_Else

%%

/* Rules
 * -----
 * All productions and actions should be placed between the start and stop
 * %% markers which delimit the Rules section.
 */

CompUnit:
DeclList  {
  CompUnit *compUnit = new CompUnit($1);
  compUnit->SetParent(NULL);
} ;

DeclList:
DeclList Decl {
  ($$ = $1)->Append($2);
} |
Decl {
  ($$ = new List<Decl *>)->Append($1);
} ;

Decl:
ConstDecl {
  $$ = $1;
} |
VarDecl {
  $$ = $1;
} |
FnDecl {
  $$ = $1;
}

ConstDecl:
T_Const Type ConstDefList ';' {
  $$ = new VarDecl($2, $1);
} ;

Type:
T_Int {
  $$ = new Type(TYPE_INT);
} |
T_Void {
  $$ = new Type(TYPE_VOID);
} ;

ConstDefList:
ConstDef {
  ($$ = new List<Def *>)->Append($1);
} |
ConstDefList ',' ConstDef {
  ($$ = $1)->Append($2);
} ;

ConstDef:
Identifier '=' ConstInitVal {
  $$ = new Def($1, $3);
}

ArrayCExpList:
ArrayCExpList '[' Exp ']' {
  ($$ = $1)->Append($2);
} |
: {
  // null rule
} ;

ConstInitVal:
Exp {
  $$// FIXME:
} ;

VarDecl:
Var ';' {
  $$ = $1;
} ;

Var:
Type VarDefList {
  $$ = new VarDecl($2, $1);
} ;

VarDefList:
VarDef {
  ($$ = new List<Def *>)->Append($1);
} |
VarDefList ',' VarDef {
  ($$ = $1)->Append($2);
} ;

VarDef:
Identifier {
  $$ = new Def($1, new List<Expr *>);
} |
Identifier '=' InitVal {
  $$ = new Def($1, $2);
} ;

InitVal:
Exp {
  ($$ = new List<Expr *>)->Append($1);
} |
'{' InitValList '}' {
  ($$ = )
} ;

InitValList:
InitVal {
  ()
} |
InitValList ',' InitVal;

Identifier:
T_Identifier {
  $$ = new Identifier(@1, $1, NULL);
} |
T_Identifier ArrayCExpList {
  List<Expr *>
  $$ = new Identifier(@1, $1, );
} |
T_Identifier '[' ']' ArrayCExpList {
  $$ = new Identifier(@1, $1, $2);
}



%%

/* The closing %% above marks the end of the Rules section and the beginning
 * of the User Subroutines section. All text from here to the end of the
 * file is copied verbatim to the end of the generated y.tab.c file.
 * This section is where you put definitions of helper functions.
 */

/* Function: InitParser
 * --------------------
 * This function will be called before any calls to yyparse().  It is designed
 * to give you an opportunity to do anything that must be done to initialize
 * the parser (set global variables, configure starting state, etc.). One
 * thing it already does for you is assign the value of the global variable
 * yydebug that controls whether yacc prints debugging information about
 * parser actions (shift/reduce) and contents of state stack during parser.
 * If set to false, no information is printed. Setting it to true will give
 * you a running trail that might be helpful when debugging your parser.
 * Please be sure the variable is set to false when submitting your final
 * version.
 */
void InitParser()
{
   PrintDebug("parser", "Initializing parser");
   yydebug = false;
}
