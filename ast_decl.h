/*
 * @Author: lfq
 * @Date: 2020-05-28 14:44:21
 * @LastEditTime: 2020-05-29 21:14:48
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \Rigel\ast_decl.h
 */

/* File: ast_decl.h
 * ----------------
 * In our parse tree, Decl nodes are used to represent and
 * manage declarations. There are 4 subclasses of the base class,
 * specialized for declarations of variables, functions, classes,
 * and interfaces.
 *
 * pp3: You will need to extend the Decl classes to implement 
 * semantic processing including detection of declaration conflicts 
 * and managing scoping issues.
 */



#include "ast.h"
#include "list.h"
#include "hashtable.h"
#include "codegen.h"
#include "ast_block.h"
#include "utility.h"
#include "ast_type.h"
#include "list.h"
#include "ast_expr.h"

class Type;
class Identifier;
class BlockItem;
class Decl;
class FnDecl;
class Node;

class Def : public Node{
  public:
    List<Expr*> *InitValueList;
    Identifier *name;
    Def(Identifier *n,List<Expr*> *ivl){
      InitValueList=ivl;
      name=n;
      name->SetParent(this);
      InitValueList->SetParentAll(this);
    }
    Type* Check(Hashtable<Decl*>* symbolTable);
    Location* Emit(CodeGenerator *cg);
};

class Decl : public BlockItem
{
  protected:
    List<Def*> *defs;
  
  public:
    Decl(){}
    Decl(List<Def*> *d) : BlockItem(*(d->Nth(0)->name->GetLocation())) 
    {
      Assert(d->Nth(0)->name != NULL);
      defs->SetParentAll(this);
    }
    //friend ostream& operator<<(ostream& out, Decl *d) { return out << d->id; }
    //const char* GetName() { return id->GetName(); }
    //Identifier* GetIdentifier() { return id; }
    //virtual bool CheckCompatibilityInClass(Decl* newDecl, Hashtable<Decl *>* symbolTable) ;
    virtual Type* Check(Hashtable<Decl*>* symbolTable);
    virtual Location* Emit(CodeGenerator *cg);
};

class VarDecl : public Decl 
{
  protected:
    Type *type;

    
  public:
    VarDecl(List<Def*> *d, Type *t):Decl(d) {
      Assert(d != NULL && t != NULL);
      (type=t)->SetParent(this);
    }
    Type* GetType() { return type; }
    
    //virtual bool CheckCompatibilityInClass(Decl* newDecl, Hashtable<Decl *>* symbolTable);
    virtual Type* Check(Hashtable<Decl*>* symbolTable);
    virtual Location* Emit(CodeGenerator *cg);
    
};



class FnDecl : public Decl 
{
  protected:
    List<VarDecl*> *formals;
    Type *returnType;
    Stmt *body;
    Identifier *id;
    
  public:
    FnDecl(Identifier *n, Type *r, List<VarDecl*> *d) 
    {
      Assert(n != NULL && r!= NULL && d != NULL);
      (returnType=r)->SetParent(this);
      (formals=d)->SetParentAll(this);
      body = NULL;
    }
    void SetFunctionBody(Stmt *b) {(body=b)->SetParent(this);}
    virtual Type* Check(Hashtable<Decl*>* symbolTable);
    virtual Location* Emit(CodeGenerator *cg);
    static bool CheckSignature(FnDecl *o_f, FnDecl *c_f);

    List<VarDecl*>* GetFormals() { return formals; }
    Type* GetReturnType() { return returnType; }
    virtual bool CheckCompatibilityInClass(Decl* newDecl, Hashtable<Decl *>* symbolTable);
    FnDecl* MakeCopy() { FnDecl* n = new FnDecl(id, returnType, formals); n->SetFunctionBody(body); return n; }
};

//#include "ast_decl.cc"