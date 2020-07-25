/*
 * @Date: 2020-07-15 21:53:37
 * @LastEditors: zyk
 * @LastEditTime: 2020-07-25 10:25:51
 * @FilePath: /compiler/ScopeStack.cpp
 */
#include "ScopeStack.h"
#include "Nonterminals.h"

void InitScopeStack(ScopeStack &stack) { stack.clear(); }

void DestroyScopeStack(ScopeStack &stack) { stack.clear(); }

int ScopeStackLength(ScopeStack &stack) { return stack.size(); }

void ScopeStackPush(ScopeStack &stack, Scope scope) { stack.push_back(scope); }

Scope &ScopeStackPop(ScopeStack &stack) {
  Scope &top = stack.at(stack.size() - 1);
  stack.pop_back();
  return top;
}

Scope &GetStackTop(ScopeStack &stack) { return stack.at(stack.size() - 1); }

ScopeEntry TraverseScopeStack(ScopeStack &stack, char *name) {
  // traverse the stack and find certain name
  ScopeEntry result;
  LocalScope *local_symtable;
  FormalScope *formal_symtable;
  GlobalScope *global_symtable;

  for (ScopeStack::iterator it = stack.end(); it != stack.begin(); it--) {
    switch (it->type) {
    case ScopeLocal:
      local_symtable = it->local;
      for (LocalScope::iterator i = local_symtable->begin();
           i != local_symtable->end(); i++) {
        if (!strcmp(i->name, name)) {
          result.type = ScopeLocal;
          result.local_entry = i.base();
          return result;
        }
      }
      break;
    case ScopeFormal:
      formal_symtable = it->formal;
      for (FormalScope::iterator i = formal_symtable->begin();
           i != formal_symtable->end(); i++) {
        if (!strcmp(i->name, name)) {
          result.type = ScopeFormal;
          result.formal_entry = i.base();
          return result;
        }
      }
      break;
    case ScopeGlobal:
      global_symtable = it->global;
      for (GlobalScope::iterator i = global_symtable->begin();
           i != global_symtable->end(); i++) {
        if (!strcmp(i->name, name)) {
          result.type = ScopeGlobal;
          result.global_entry = i.base();
          return result;
        }
      }
      break;

    default:
      break;
    }
  }
  result.type = ScopeNone;
  result.local_entry = nullptr;
  return result;
}

ExprType *GetExprType(GrammarTree tree, ScopeStack &stack) {
  // TODO: find the type of expr represented by 'tree'
// 这个可能并不需要？但是需要一个函数判断expr是不是常数
  if (!tree)
    return nullptr;
  ExprType *result = nullptr;
}


void ScopeTrial(GrammarTree tree, Scope scope, ScopeStack &stack) {
  // TODO: traverse the tree and calculate semantic fault
  string PASS = "[Pass]";
  if (!tree)
    return;
  if (tree->type == Program) {
    ScopeStackPush(stack, scope);
  } else if (tree->type == CompUnit) {
    cout << "Check if main function exists: ";
    cout << "Check main's signature: ";
  } else if (tree->type == ConstExp) {
    cout << "Check if ConstExp evaluates to const: ";
  }
}