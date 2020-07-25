/*
 * @Date: 2020-07-15 21:53:37
 * @LastEditors: zyk
 * @LastEditTime: 2020-07-22 11:26:10
 * @FilePath: /compiler/ScopeStack.h
 */
#ifndef _SCOPESTACK_H
#define _SCOPESTACK_H

#include "GrammarTree.h"
#include "SymbolTable.h"
#include <string>
#include <iostream>
#include <vector>

using namespace std;

#define STACK_INIT_SIZE 100
#define STACKINCREMENT 20

extern int smerror;

typedef enum ScopeType {
  ScopeLocal,
  ScopeFormal,
  ScopeGlobal,
  ScopeNone
} ScopeType;

typedef struct Scope {
  ScopeType type;
  union {
    LocalScope *local;
    FormalScope *formal;
    GlobalScope *global;
  };
} Scope;

typedef struct ScopeEntry {
  ScopeType type;
  union {
    LocalScopeEntry *local_entry;
    FormalScopeEntry *formal_entry;
    GlobalScopeEntry *global_entry;
  };
} ScopeEntry;

typedef vector<Scope&> ScopeStack;

typedef SymbolCategory ExprType;

void InitScopeStack(ScopeStack &stack);
void DestroyScopeStack(ScopeStack &stack);
int ScopeStackLength(ScopeStack &stack);
void ScopeStackPush(ScopeStack &stack, Scope scope);
Scope &ScopeStackPop(ScopeStack &stack);
Scope &GetStackTop();

ScopeEntry TraverseScopeStack(ScopeStack &stack, string name);
ExprType *GetExprType(GrammarTree tree, ScopeStack &stack);
void ScopeTrial(GrammarTree tree, Scope scope, ScopeStack &stack);

#endif