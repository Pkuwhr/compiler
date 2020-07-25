/*
 * @Date: 2020-06-13 17:07:18
 * @LastEditors: zyk
 * @LastEditTime: 2020-07-25 10:24:21
 * @FilePath: /compiler/GrammarTree.h
 */

#ifndef _GRAMMARTREE_H
#define _GRAMMARTREE_H

#define MAXSTRING 36
#define VOIDLINE -1

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "SymbolTable.h"
#include "ArrayInfo.h"

using namespace std;


typedef struct GrammarTreeNode {
  int line; // the number of its line
  int type; // bison自动生成的枚举常量，标记终结符/非终结符类型
  struct GrammarTreeNode *lchild; // lchild指向孩子节点
  struct GrammarTreeNode *rchild; // rchild指向兄弟节点
  union                           // the value of this grammar unit
  {
    char *string_value;
    int int_value; // 用于IntConstant或Exp
  };
  union {
    // ! 语义分析时节点可能具有的类型
    // TODO: 还缺少Expr类型
    LocalScope *local_scope;
    FormalScope *formal_scope;
    GlobalScope *global_scope;
    LocalScopeEntry *local_entry;
    FormalScopeEntry *formal_entry;
    GlobalScopeEntry *global_entry;
    ArrayInfo *array_info;
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
