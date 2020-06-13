/*
 * @Author: lfq
 * @Date: 2020-05-28 17:19:45
 * @LastEditTime: 2020-05-29 21:24:04
 * @LastEditors: Please set LastEditors
 * @Description: stmtBlock defination
 * @FilePath: \compiler\ast_block.h
 */ 

#include "list.h"
#include "ast.h"
#include "hashtable.h"
#include "ast_type.h"



class Decl;
class VarDecl;
class Expr;
class IntConstant;
class Location;
class CodeGenerator;

Hashtable<Decl *>* __globalST;
class CompUnit : public Node
{
    protected:
        List<Decl*> *decls;
    public:
        CompUnit(List<Decl*> *declList);
        void Check();
        void Emit();
};

class BlockItem : public Node
{
    public:
        BlockItem() : Node() {}
        BlockItem(yyltype loc) : Node(loc) {}
        virtual Type* Check(Hashtable<Decl*>* symbolTable)=0;
        virtual Location* Emit(CodeGenerator *cg)=0;
};



class Stmt : public BlockItem
{
    public:
     char* exitLabel;

     Stmt() : BlockItem() {}
     Stmt(yyltype loc) : BlockItem(loc) {}
     virtual Type* Check(Hashtable<Decl*>* symbolTable)=0;
     virtual Location* Emit(CodeGenerator *cg)=0;
};

class Block : public Stmt
{
    protected:
        List<BlockItem*> *BItems;
    public:
        Block(List<BlockItem*> *BlockItems);
        virtual Type* Check(Hashtable<Decl*>* symbolTable);
        virtual Location* Emit(CodeGenerator *cg);
};

class ConditionalStmt : public Stmt
{
  protected:
    Expr *test;
    Stmt *body;
  
  public:
    ConditionalStmt(Expr *testExpr, Stmt *body);
    virtual Type* Check(Hashtable<Decl*>* symbolTable);
    virtual Location* Emit(CodeGenerator *cg);
};

class WhileStmt : public ConditionalStmt 
{
  public:
    WhileStmt(Expr *test, Stmt *body) : ConditionalStmt(test, body) {}
    virtual Type* Check(Hashtable<Decl*>* symbolTable);
    virtual Location* Emit(CodeGenerator *cg);
};

class IfStmt : public ConditionalStmt 
{
  protected:
    Stmt *elseBody;
  
  public:
    IfStmt(Expr *test, Stmt *thenBody, Stmt *elseBody);
    virtual Type* Check(Hashtable<Decl*>* symbolTable);
    virtual Location* Emit(CodeGenerator *cg);
};

class BreakStmt : public Stmt 
{
  public:
    BreakStmt(yyltype loc) : Stmt(loc) {}
    virtual Type* Check(Hashtable<Decl*>* symbolTable);
    virtual Location* Emit(CodeGenerator *cg);
};

class ReturnStmt : public Stmt  
{
  protected:
    Expr *expr;
  
  public:
    bool noReturn;
    ReturnStmt(yyltype loc, Expr *expr);
    virtual Type* Check(Hashtable<Decl*>* symbolTable);
    virtual Location* Emit(CodeGenerator *cg);
};

class ContinueStmt : public Stmt 
{
  public:
    ContinueStmt(yyltype loc) : Stmt(loc) {}
    virtual Type* Check(Hashtable<Decl*>* symbolTable);
    virtual Location* Emit(CodeGenerator *cg);
};

