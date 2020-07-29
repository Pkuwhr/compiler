/*
 * @Date: 2020-07-01 12:45:00
 * @LastEditors: zyk
 * @LastEditTime: 2020-07-26 12:34:05
 * @FilePath: \compiler\SymbolTable.h
 */

#ifndef _SYMBOLTABLE_H
#define _SYMBOLTABLE_H

#include <cstdlib>
#include <iostream>
#include <vector>

#include "ArrayInfo.h"

using namespace std;

// 局部作用域符号表
typedef struct LocalScopeEntry {
  char *name;
  bool is_const, is_array, is_block;
  ArrayInfo *array_info;
  vector<LocalScopeEntry> *embedded_scope; // 内嵌域符号表
} LocalScopeEntry;

typedef vector<LocalScopeEntry> LocalScope;

// 函数形参作用域符号表
typedef struct FormalScopeEntry {
  char *name;
  bool is_array;
  ArrayInfo *array_info;
} FormalScopeEntry;

typedef vector<FormalScopeEntry> FormalScope;

typedef struct GlobalScopeEntry {
  char *name;
  bool is_func, is_void;
  union {
    int formal_count; // 函数形参个数
    ArrayInfo *array_info;
  };
  FormalScope *formal_scope; // 函数形参域
  LocalScope *local_scope;   // 函数体域
} GlobalScopeEntry;

typedef vector<GlobalScopeEntry> GlobalScope;

// 把local_scope中的entry填入global_scope中 注意**local_scope中不能有Block**
GlobalScope *AddLocalIntoGlobal(GlobalScope *global_scope,
                                LocalScope *local_scope);
// 把local_scope放入另一个local_scope中
// ! 注意head为Null的情况 !
LocalScope *AddLocalIntoLocal(LocalScope *head, LocalScope *tail);
// 把entry放入scope中 且返回加入后的scope指针
// ! 若scope = nullptr 则需要创建一个scope !
GlobalScope *AddEntryIntoGlobalScope(GlobalScope *scope,
                                     GlobalScopeEntry *entry);
FormalScope *AddEntryIntoFormalScope(FormalScope *scope,
                                     FormalScopeEntry *entry);
LocalScope *AddEntryIntoLocalScope(LocalScope *scope, LocalScopeEntry *entry);

// 把int值放入dims vector中 若dims = null 则需要新建一个vector
vector<int> *AddIntoDimsVector(vector<int> *dims, int d);
// 把GrammarTree指针放入 exprs vector中 若 exprs = null 则需要新建一个vector
vector<int> *AddIntoExprsVector(vector<int> *exprs, int e);
// 把ArrayInitValue加入到vector中 若 init = null 则需要新建一个vector
// add_sep = true时 添加value后 继续添加一个sep
vector<ArrayInitValue> *AddExprIntoArrayInitValue(vector<ArrayInitValue> *init,
                                                  int value, bool add_sep);
// 把tail中的元素连接到head后面 head tail都可以为Null
// ! 连接之后务必加上一个sep分隔符 !
vector<ArrayInitValue> *MergeArrayInitValue(vector<ArrayInitValue> *head,
                                            vector<ArrayInitValue> *tail);
// 为local_scope中的entry设置统一的isConst标记
// 返回添加好的scope指针
LocalScope *AttachTypeToLocalScope(LocalScope *local_scope, bool is_const);

// 组装一个ArrayInfo 返回其地址
ArrayInfo *NewArrayInfo(vector<int> *_dims, vector<GrammarTree> *_exprs,
                        vector<ArrayInitValue> *_init_values);
// 新建一个ArrayInfo 只存储初值 其它字段置为null
ArrayInfo *NewInitValue(bool is_const, int const_value, GrammarTree var_value);
// 新建一个local_entry
LocalScopeEntry *NewLocalEntry(char *_name, bool _is_array, bool _is_block,
                               ArrayInfo *_array_info, LocalScope *local_scope);
// 新建一个formal_entry
FormalScopeEntry *NewFormalEntry(char *_name, bool _is_array,
                                 ArrayInfo *_array_info);
// 新建一个global_entry
GlobalScopeEntry *NewGlobalEntry(char *_name, bool _is_function, bool _is_void,
                                 ArrayInfo *_array_info, int _formal_count,
                                 FormalScope *_formal_scope,
                                 LocalScope *_local_scope);

#endif // _SYMBOLTABLE_H
