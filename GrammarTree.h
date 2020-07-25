/*
 * @Date: 2020-06-13 17:07:18
 * @LastEditors: zyk
 * @LastEditTime: 2020-07-25 13:55:00
 * @FilePath: /compiler/GrammarTree.h
 */

#ifndef _GRAMMARTREE_H
#define _GRAMMARTREE_H

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "ArrayInfo.h"
#include "SymbolTable.h"

using namespace std;

typedef struct GrammarTreeNode {
  //---------------
  // Grammar info
  //---------------
  int line; // the number of its line
  int type; // bison自动生成的枚举常量，标记终结符/非终结符类型
  struct GrammarTreeNode *lchild; // lchild指向孩子节点
  struct GrammarTreeNode *rchild; // rchild指向兄弟节点
  union                           // the value of this grammar unit
  {
    char *string_value; // 用于StringConstant
    int int_value;      // 用于IntConstant
  };

  //---------------
  // semantic info
  //---------------

  // processing Expr
  bool isVisited; // for debug
  bool is_constant_expr; // 若某Expr的子节点均为constant 则其本身也是constant
  int expr_value; // constant表达式的值可以在自底向上的语法分析中直接求出
  union {
    // 语义分析时节点可能具有的类型
    // scope vector
    LocalScope *local_scope;
    FormalScope *formal_scope;
    GlobalScope *global_scope;
    // scope entry (one line in vector)
    LocalScopeEntry *local_entry;
    FormalScopeEntry *formal_entry;
    GlobalScopeEntry *global_entry;

    // used by ConstDef/VarDef
    ArrayInfo *array_info;
    // used by (const)ArraySubSeq
    vector<int> *dims;
    // used by (const)InitVal. when there is only one exp,
    // the int will be returned in vector too
    vector<ArrayInitValue> *array_init_value;
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
