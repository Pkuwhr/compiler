/*
 * @Author: lfq
 * @Date: 2020-05-28 21:34:28
 * @LastEditTime: 2020-05-29 17:05:23
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \compiler\ast.cc
 */ 



#include "ast.h"
#include "ast_type.h"
#include "ast_decl.h"
#include <string.h> // strdup
#include <stdio.h>  // printf

Node::Node(yyltype loc) {
    location = new yyltype(loc);
    parent = NULL;
}

Node::Node() {
    location = NULL;
    parent = NULL;
}
	 
Identifier::Identifier(yyltype loc, const char *n, List<Expr*> *el) : Node(loc) {
    (Elist = el)->SetParentAll(this);
    name = strdup(n);
} 

