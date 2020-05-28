/*
 * @Author: lfq
 * @Date: 2020-05-28 14:44:21
 * @LastEditTime: 2020-05-28 23:11:10
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

#ifndef _H_ast_decl
#define _H_ast_decl

#include "ast.h"
#include "list.h"
#include "hashtable.h"
#include "codegen.h"
#include "ast_block.h"
#include "utility.h"


class Type;
class Identifier;
class BlockItem;
class Decl;
class FnDecl;

class Decl : public BlockItem
{
  protected:
    Identifier *id;
  
  public:
    Decl();
    Decl(Identifier *name) ;
    friend ostream& operator<<(ostream& out, Decl *d) { return out << d->id; }
    const char* GetName() { return id->GetName(); }
    Identifier* GetIdentifier() { return id; }
    virtual bool CheckCompatibilityInClass(Decl* newDecl, Hashtable<Decl *>* symbolTable) = 0;
    virtual Type* Check(Hashtable<Decl*>* symbolTable)=0;
    virtual Location* Emit(CodeGenerator *cg)=0;
};

class VarDecl : public Decl 
{
  protected:
    Type *type;
    
  public:
    VarDecl(Identifier *name, Type *type);
    Type* GetType() { return type; }
    
    virtual bool CheckCompatibilityInClass(Decl* newDecl, Hashtable<Decl *>* symbolTable);
    virtual Type* Check(Hashtable<Decl*>* symbolTable);
    virtual Location* Emit(CodeGenerator *cg);
    
};



class FnDecl : public Decl 
{
  protected:
    List<VarDecl*> *formals;
    Type *returnType;
    Stmt *body;
    
  public:
    FnDecl();
    FnDecl(Identifier *name, Type *returnType, List<VarDecl*> *formals);
    void SetFunctionBody(Stmt *b);
    virtual Type* Check(Hashtable<Decl*>* symbolTable);
    virtual Location* Emit(CodeGenerator *cg);
    static bool CheckSignature(FnDecl *o_f, FnDecl *c_f);

    List<VarDecl*>* GetFormals() { return formals; }
    Type* GetReturnType() { return returnType; }
    virtual bool CheckCompatibilityInClass(Decl* newDecl, Hashtable<Decl *>* symbolTable);
    FnDecl* MakeCopy() { FnDecl* n = new FnDecl(id, returnType, formals); n->SetFunctionBody(body); return n; }
};

#endif
