/*
 * @Author: lfq
 * @Date: 2020-05-28 21:36:57
 * @LastEditTime: 2020-05-28 21:48:24
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \Rigel\ast_type.cc
 */ 

/* File: ast_type.cc
 * -----------------
 * Implementation of type node classes.
 */

#include <string.h>
#include "ast_type.h"
#include "ast_decl.h"
#include "ast_block.h"
#include "errors.h"
#include "hashtable.h"
 
/* Class constants
 * ---------------
 * These are public constants for the built-in base types (int, double, etc.)
 * They can be accessed with the syntax Type::intType. This allows you to
 * directly access them and share the built-in types where needed rather that
 * creates lots of copies.
 */

extern Hashtable<Decl *>* __globalST;

Type::Type(int n) {
    Assert(n);
    typeKind = n;
}

bool Type::ValidityChecker(Hashtable<Decl *>* parentST, Type* type) {
    //TODO
    return false;
}

ArrayType::ArrayType(yyltype loc, Type *et) : Type(loc) {
    Assert(et != NULL);
    (elemType=et)->SetParent(this);
}

