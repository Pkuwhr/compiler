/*
 * @Author: your name
 * @Date: 2020-05-28 23:11:57
 * @LastEditTime: 2020-05-29 16:48:26
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \Rigel\expr.cc
 */ 

/* File: ast_expr.cc
 * -----------------
 * Implementation of expression node classes.
 */

#include <string.h>
#include "ast_expr.h"
#include "ast_type.h"
#include "ast_decl.h"
#include "errors.h"
#include "codegen.h"

extern Hashtable<Decl *>* __globalST;

Type* Expr::Check(Hashtable<Decl*>* parentST) { 
    return NULL;
}

Location* Expr::Emit(CodeGenerator *cg) {
    return NULL;
}


IntConstant::IntConstant(yyltype loc, int val) : Expr(loc) {
    value = val;
}

Type* IntConstant::Check(Hashtable<Decl*>* parentST) { 
    return new Type(2);
}

Location* IntConstant::Emit(CodeGenerator *cg) {
    //TODO
    return NULL;
}

Operator::Operator(yyltype loc, const char *tok) : Node(loc) {
    Assert(tok != NULL);
    strncpy(tokenString, tok, sizeof(tokenString));
}

BinaryExpr::BinaryExpr(Expr *l, Operator *o, Expr *r) 
  : Expr(Join(l->GetLocation(), r->GetLocation())) {
    Assert(l != NULL && o != NULL && r != NULL);
    (op=o)->SetParent(this);
    (left=l)->SetParent(this); 
    (right=r)->SetParent(this);
}
  
LValue::LValue(yyltype loc, Expr *b, Expr *s) : Expr(loc) {
    (base=b)->SetParent(this); 
    (subscript=s)->SetParent(this);
}

Type* LValue::Check(Hashtable<Decl*>* parentST) {
    //TODO

    
    return NULL;
}

Location* LValue::Emit(CodeGenerator *cg) {
    //TODO
    return NULL;
}

Location* LValue::EmitMemoryLocation(CodeGenerator *cg) {
    //TODO
    return NULL;
}


Call::Call(yyltype loc, Identifier *f, List<Expr*> *a) : Expr(loc)  {
    Assert(f != NULL && a != NULL); // b can be be NULL (just means no explicit base)
    (field=f)->SetParent(this);
    (actuals=a)->SetParentAll(this);
}

Type* Call::Check(Hashtable<Decl*>* parentST) {
    //TODO

    return NULL;
}

Location* Call::Emit(CodeGenerator *cg) {
    //TODO
    return NULL;
}


Location* AssignExpr::Emit(CodeGenerator *cg) {
    //TODO
    return NULL;
}

Type* ArithmeticExpr::Check(Hashtable<Decl*>* parentST) { 
    //TODO
    return NULL;
}

Location* ArithmeticExpr::Emit(CodeGenerator *cg) {
    //TODO
    return NULL;
}

Type* RelationalExpr::Check(Hashtable<Decl*>* parentST) { 
    //TODO
    return NULL;
}

Location* RelationalExpr::Emit(CodeGenerator *cg) {
    //TODO
    return NULL;
}

Type* EqualityExpr::Check(Hashtable<Decl*>* parentST) {
    //TODO
    return NULL;
}

Location* EqualityExpr::Emit(CodeGenerator *cg) {
    //TODO
    return NULL;
}

Type* LogicalExpr::Check(Hashtable<Decl*>* parentST) { 
    //TODO
    return NULL;
}

Location* LogicalExpr::Emit(CodeGenerator *cg) {
    //TODO
    return NULL;
}




