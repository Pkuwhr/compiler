/*
 * @Date: 2020-07-15 21:53:37
 * @LastEditors: zyk
 * @LastEditTime: 2020-07-26 16:13:15
 * @FilePath: /compiler/ScopeStack.h
 */
#ifndef _SCOPESTACK_H
#define _SCOPESTACK_H

#include "GrammarTree.h"
#include "SymbolTable.h"
#include "Nonterminals.h"
#include "parser.tab.h"

#include <string>
#include <iostream>
#include <vector>
#include <cassert>

using namespace std;

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

void InitScopeStack(ScopeStack &stack);
void DestroyScopeStack(ScopeStack &stack);
int ScopeStackLength(ScopeStack &stack);
void ScopeStackPush(ScopeStack &stack, Scope scope);
Scope &ScopeStackPop(ScopeStack &stack);
Scope &GetStackTop();

ScopeEntry TraverseScopeStack(ScopeStack &stack, string name);
void CheckExprValue(GrammarTree tree, ScopeStack &stack); // TODO: stack is not used
void ScopeTrial(GrammarTree tree, Scope scope, ScopeStack &stack);

#endif