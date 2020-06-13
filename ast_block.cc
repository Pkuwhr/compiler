/*
 * @Author: lfq
 * @Date: 2020-05-28 21:49:50
 * @LastEditTime: 2020-05-28 22:03:49
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \compiler\ast_block.cc
 */ 



#include "ast.h"
#include "ast_block.h"
#include "ast_type.h"
#include "ast_decl.h"
#include "ast_expr.h"
#include "list.h"
#include "hashtable.h"
#include "errors.h"

Hashtable<Decl *>* __globalST;

CompUnit::CompUnit(List<Decl*> *d) {
    Assert(d != NULL);
    (decls=d)->SetParentAll(this);
}

void CompUnit::Check() {
    //TODO
}

void CompUnit::Emit() {
    //TODO
}



Block::Block(List<BlockItem*> *d) {
    Assert(d != NULL);
    (BItems=d)->SetParentAll(this);
}

Type* Block::Check(Hashtable<Decl*>* parentST) { 
    //TODO
    return NULL;
}

Location* Block::Emit(CodeGenerator *cg) {
    //TODO
    return NULL;
}

ConditionalStmt::ConditionalStmt(Expr *t, Stmt *b) { 
    Assert(t != NULL && b != NULL);
    (test=t)->SetParent(this); 
    (body=b)->SetParent(this);
}

Type* ConditionalStmt::Check(Hashtable<Decl*>* parentST) {
    //TODO
    return NULL;
}

Location* ConditionalStmt::Emit(CodeGenerator *cg) {
    return NULL;
}

Type* WhileStmt::Check(Hashtable<Decl*>* parentST) {  
    //TODO
    return NULL;
}

Location* WhileStmt::Emit(CodeGenerator *cg) {
    //TODO
    return NULL;
}

Type* BreakStmt::Check(Hashtable<Decl*>* parentST) {
    
    //TODO
    return NULL;
}

Location* BreakStmt::Emit(CodeGenerator *cg) {
    //TODO
    return NULL;
}

IfStmt::IfStmt(Expr *t, Stmt *tb, Stmt *eb): ConditionalStmt(t, tb) { 
    Assert(t != NULL && tb != NULL); // else can be NULL
    elseBody = eb;
    if (elseBody) elseBody->SetParent(this);
}

Type* IfStmt::Check(Hashtable<Decl*>* parentST) { 
    //TODO
    return NULL;
}

Location* IfStmt::Emit(CodeGenerator *cg) {
    //TODO
    return NULL;
}

ReturnStmt::ReturnStmt(yyltype loc, Expr *e) : Stmt(loc) { 
    Assert(e != NULL);
    (expr=e)->SetParent(this);
}

Type* ReturnStmt::Check(Hashtable<Decl*>* parentST) {
    //TODO
    return NULL;
}

Location* ReturnStmt::Emit(CodeGenerator *cg) {
    //TODO
    return NULL;
}

Type* ContinueStmt::Check(Hashtable<Decl*>* parentST) {
    
    //TODO
    return NULL;
}

Location* ContinueStmt::Emit(CodeGenerator *cg) {
    //TODO
    return NULL;
}



