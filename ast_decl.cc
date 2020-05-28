/* File: ast_decl.cc
 * -----------------
 * Implementation of Decl node classes.
 */
#include "ast.h"
#include "ast_decl.h"
#include "ast_type.h"
#include "ast_block.h"
#include "list.h"


extern Hashtable<Decl *>* __globalST;

Decl::Decl(Identifier* n) : BlockItem(*(n->GetLocation())) {
    Assert(n != NULL);
    (id=n)->SetParent(this); 
}


VarDecl::VarDecl(Identifier *n, Type *t) : Decl(n) {
    Assert(n != NULL && t != NULL);
    (type=t)->SetParent(this);
}

Location * VarDecl::Emit(CodeGenerator* cg) {
    //TODO
    return NULL;
}

Type * VarDecl::Check(Hashtable<Decl *>* symbolTable) {
    //TODO
    return NULL;
}


bool VarDecl::CheckCompatibilityInClass(Decl* newDecl, Hashtable<Decl *>* symbolTable) {
    //TODO
    return true;
}

bool FnDecl::CheckCompatibilityInClass(Decl* newDecl, Hashtable<Decl *>* symbolTable) {
    //TODO
    return true;
}

FnDecl::FnDecl(Identifier *n, Type *r, List<VarDecl*> *d) : Decl(n) {
    Assert(n != NULL && r!= NULL && d != NULL);
    (returnType=r)->SetParent(this);
    (formals=d)->SetParentAll(this);
    body = NULL;
}

Location * FnDecl::Emit(CodeGenerator* cg) {
    //todo
    return NULL;
}

Type * FnDecl::Check(Hashtable<Decl *>* parentST) {
    //TODO
    return NULL;
}

bool FnDecl::CheckSignature(FnDecl *o_f, FnDecl *c_f) {
    //TODO
    
    return true;
}

void FnDecl::SetFunctionBody(Stmt *b) { 
    (body=b)->SetParent(this);
}
