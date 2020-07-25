/*
 * @Date: 2020-07-01 12:45:00
 * @LastEditors: zyk
 * @LastEditTime: 2020-07-25 10:37:13
 * @FilePath: /compiler/SymbolTable.h
 */

#ifndef _SYMBOLTABLE_H
#define _SYMBOLTABLE_H

#include <cstdlib>
#include <iostream>
#include <vector>

#include "ArrayInfo.h"
#include "SysYCategory.h"

using namespace std;

// 局部作用域符号表
typedef struct LocalScopeEntry {
  char *name;
  SymbolCategory symbol_type;
  union {
    int int_init_val;
    ArrayInfo *array_info;
  };
  vector<LocalScopeEntry> *embedded_scope; // 内嵌域符号表
} LocalScopeEntry;

typedef vector<LocalScopeEntry> LocalScope;

// 函数形参作用域符号表
typedef struct FormalScopeEntry {
  char *name;
  SymbolCategory formal_type; // 形参类型
} FormalScopeEntry;

typedef vector<FormalScopeEntry> FormalScope;

typedef struct GlobalScopeEntry {
  char *name;
  SymbolCategory symbol_type; // 全局符号类型
  ReturnCategory rev_type;    // 函数返回值类型
  union {
    int init_val;     // 整型常量初值
    int formal_count; // 函数形参个数
    ArrayInfo *array_info;
  };
  FormalScope *formal_scope; // 函数形参域
  LocalScope *local_scope;   // 函数体域
} GlobalScopeEntry;

typedef vector<GlobalScopeEntry> GlobalScope;

GlobalScope &AddLocalIntoGlobal(GlobalScope &global_scope,
                                LocalScope &local_scope);
// TODO
GlobalScope &AddGlobalIntoGlobal(GlobalScope &global_scope_1,
                                 GlobalScope &global_scope_2);


void AttachTypeToLocalScope(LocalScope local_scope,SymbolCategory category);
LocalScope &AddLocalIntoLocal(LocalScope &local_scope_1,LocalScope &local_scope_2);
LocalScopeEntry &NewLocalIntEntry(char* name,int val);
LocalScopeEntry &NewLocalArrayEntry(char* name,ArrayInfo *array_info);

#endif // _SYMBOLTABLE_H
