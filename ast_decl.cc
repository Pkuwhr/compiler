/*
 * @Author: your name
 * @Date: 2020-05-29 00:35:53
 * @LastEditTime: 2020-05-29 11:42:26
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \compiler\ast_decl.cc
 */ 
/* File: ast_decl.cc
 * -----------------
 * Implementation of Decl node classes.
 */
#include "ast.h"
#include "ast_decl.h"
#include "ast_type.h"
#include "ast_block.h"
#include "list.h"


Location * Def::Emit(CodeGenerator* cg) {
    //TODO
    return NULL;
}

Type * Def::Check(Hashtable<Decl *>* symbolTable) {
    //TODO
    return NULL;
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


