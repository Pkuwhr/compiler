/*
 * @Date: 2020-06-13 17:07:18
 * @LastEditors: zyk
 * @LastEditTime: 2020-07-15 15:33:41
 * @FilePath: /compiler/GrammarTree.h
 */

#ifndef _GRAMMARTREE_H
#define _GRAMMARTREE_H

#define MAXSTRING 36
#define VOIDLINE -1

#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>
#include<string.h>

#include"SymbolTable.h"

extern char *yytext;
extern FILE *yyin;
extern int yylineno;

extern int yyparse();

typedef struct GrammarTreeNode {
    int line;       // the number of its line
    int type;       // bison自动生成的枚举常量，标记终结符/非终结符类型
    struct GrammarTreeNode *lchild;     // lchild指向孩子节点
    struct GrammarTreeNode *rchild;     // rchild指向兄弟节点
    union           // the value of this grammar unit
    {
        char *string_value;
        int int_value;
    };
	union
	{
		LocalScopeEntry local_scope;
        FormalScopeEntry formal_scope;
        GlobalScopeEntry global_scope;
	};
} GrammarTreeNode;

typedef struct GrammarTreeNode *GrammarTree;

/* Create GrammarTree Using Chile-Brother representation
 * name: the name of the grammar unit
 * num: the number of grammar unit in the varible parameter list
*/
GrammarTree CreateGrammarTree(int type, int num, ...);

/* Traverse GrammarTree Using Pre-Order
 * tree: the grammar tree
 * level: the number of the level
 */
void TraverseGrammarTree(GrammarTree gmtree, int level);

#endif
