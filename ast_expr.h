/*
 * @Author: lfq
 * @Date: 2020-05-28 20:08:00
 * @LastEditTime: 2020-06-05 20:59:03
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \Rigel\ast_exp.h
 */ 

#include "ast.h"
#include "ast_block.h"
#include "list.h"

class Type;

class Expr : public Stmt 
{
  public:
    Expr(yyltype loc) : Stmt(loc) {}
    Expr() : Stmt() {}
    virtual Type* Check(Hashtable<Decl*>* symbolTable);
    virtual Location* Emit(CodeGenerator *cg);
    Type* cachedType;
};

class IntConstant : public Expr 
{
  protected:
    int value;
  
  public:
    int GetValue() { return value; }
    IntConstant(yyltype loc, int val);
    virtual Type* Check(Hashtable<Decl*>* symbolTable);
    virtual Location* Emit(CodeGenerator *cg);
};

class StringConstant : public Expr 
{ 
  protected:
    char *value;
    
  public:
    StringConstant(yyltype loc, const char *val);
    virtual Type* Check(Hashtable<Decl*>* symbolTable);
    virtual Location* Emit(CodeGenerator *cg);
};

class Operator : public Node 
{
  protected:
    char tokenString[4];
    
  public:
    Operator(yyltype loc, const char *tok);
    friend ostream& operator<<(ostream& out, Operator *o) { return out << o->tokenString; }
    virtual Type* Check(Hashtable<Decl*>* symbolTable) { return NULL; }
    virtual Location* Emit(CodeGenerator *cg) { return NULL; }
    char* GetToken() { return tokenString; }
};

class BinaryExpr : public Expr
{
  protected:
    Operator *op;
    Expr *left, *right; // left will be NULL if unary
    
  public:
    BinaryExpr(Expr *lhs, Operator *op, Expr *rhs);  // for binary
    virtual Type* Check(Hashtable<Decl*>* symbolTable) = 0;
    virtual Location* Emit(CodeGenerator *cg) = 0;
};

class LValue : public Expr 
{
  protected:
    Expr *base;
    List<Expr *> *offset;
    
  public:
    LValue(yyltype loc, Expr *base, List<Expr *> *offset);
    virtual Type* Check(Hashtable<Decl*>* symbolTable);
    virtual Location* Emit(CodeGenerator *cg);
    Location* EmitMemoryLocation(CodeGenerator *cg);
};

class UnaryExpr : public Expr
{
  protected:
    Operator *op;
    Expr *right; 
    
  public:
    UnaryExpr(Operator *op, Expr *rhs);  // for binary
    virtual Type* Check(Hashtable<Decl*>* symbolTable);
    virtual Location* Emit(CodeGenerator *cg);
}; 

class ArithmeticExpr : public BinaryExpr 
{
  public:
    ArithmeticExpr(Expr *lhs, Operator *op, Expr *rhs) : BinaryExpr(lhs,op,rhs) {}
    virtual Type* Check(Hashtable<Decl*>* symbolTable);
    virtual Location* Emit(CodeGenerator *cg);
};

class RelationalExpr : public BinaryExpr 
{
  public:
    RelationalExpr(Expr *lhs, Operator *op, Expr *rhs) : BinaryExpr(lhs,op,rhs) {}
    virtual Type* Check(Hashtable<Decl*>* symbolTable);
    virtual Location* Emit(CodeGenerator *cg);
};

class EqualityExpr : public BinaryExpr 
{
  public:
    EqualityExpr(Expr *lhs, Operator *op, Expr *rhs) : BinaryExpr(lhs,op,rhs) {}
    const char *GetPrintNameForNode() { return "EqualityExpr"; }
    virtual Type* Check(Hashtable<Decl*>* symbolTable);
    virtual Location* Emit(CodeGenerator *cg);
};

class LogicalExpr : public BinaryExpr 
{
  public:
    LogicalExpr(Expr *lhs, Operator *op, Expr *rhs) : BinaryExpr(lhs,op,rhs) {}
    const char *GetPrintNameForNode() { return "LogicalExpr"; }
    virtual Type* Check(Hashtable<Decl*>* symbolTable);
    virtual Location* Emit(CodeGenerator *cg);
};

class AssignExpr : public BinaryExpr 
{
  public:
    AssignExpr(Expr *lhs, Operator *op, Expr *rhs) : BinaryExpr(lhs,op,rhs) { isLocalVariable = false; leftType = NULL; rightType = NULL; }
    const char *GetPrintNameForNode() { return "AssignExpr"; }
    virtual Type* Check(Hashtable<Decl*>* symbolTable);
    virtual Location* Emit(CodeGenerator *cg);

    Type* leftType;
    Type* rightType;
    bool isLocalVariable;
};

class Call : public Expr 
{
  protected:
    Identifier *field;
    List<Expr*> *actuals;
    
  public:
    Call(yyltype loc, Identifier *field, List<Expr*> *args);
    virtual Type* Check(Hashtable<Decl*>* symbolTable);
    virtual Location* Emit(CodeGenerator *cg);    
};

class InitValue : public Expr
{
  protected:
    List<Expr*> *InitValueList;
  
  public:
    InitValue(List<Expr*> *InitValueList);
    virtual Type* Check(Hashtable<Decl*>* symbolTable);
    virtual Location* Emit(CodeGenerator *cg);
};

