/*
 * File: SymbolTable.cpp
 * Project: compiler
 * File Created: Sunday, 2nd August 2020 7:39:21 pm
 * Author: zyk
 * -----
 * Last Modified: Monday, 10th August 2020 5:40:53 pm
 * Modified By: zyk
 * -----
 * 2020 - HUST
 */

#include "SymbolTable.h"
#include "ArrayInfo.h"

extern ArrayInitVal ZERO, LEFT, RIGHT;

// 把local_scope中的entry填入global_scope中 注意**local_scope中不能有Block**
GlobalScope *AddLocalIntoGlobal(GlobalScope *global_scope,
                                LocalScope *local_scope) {
  if (global_scope == nullptr) {
    global_scope = (GlobalScope *)malloc(sizeof(GlobalScope));
    global_scope->clear();
  }
  GlobalScopeEntry *global_entry =
      (GlobalScopeEntry *)malloc(sizeof(GlobalScopeEntry));
  for (LocalScope::iterator it = local_scope->begin(); it != local_scope->end();
       it++) {
    global_entry->name = (*it)->name;

    // it 不是函数
    global_entry->formal_scope = nullptr;
    global_entry->local_scope = nullptr;

    global_entry->is_const = (*it)->is_const;
    global_entry->is_func = false;
    global_entry->is_void = false;
    global_entry->is_array = (*it)->is_array;

    if (global_entry->is_array)
      global_entry->array_info = (*it)->array_info;
    if (global_entry->is_const && !global_entry->is_array)
      global_entry->int_init_value = (*it)->int_init_value;
    if (!global_entry->is_const && !global_entry->is_array)
      global_entry->int_init_expr = (*it)->int_init_expr;
    global_scope->push_back(global_entry);
  }
  for (LocalScope::iterator it = local_scope->begin(); it != local_scope->end();
       it++) {
    free(*it);
  }
  local_scope->clear();
  free(local_scope);
  return global_scope;
}

// 把local_scope放入另一个local_scope中
LocalScope *AddLocalIntoLocal(LocalScope *head, LocalScope *tail) {
  if (head == nullptr && tail == nullptr)
    return nullptr;

  else if (head == nullptr)
    return tail;

  else if (tail == nullptr)
    return head;

  // head != null and tail != null

  LocalScope::iterator it = tail->begin();
  while (it != tail->end()) {
    // add *it into head
    head->push_back(*it);
    it++;
  }
  return head;
}

// 把entry放入scope中 且返回加入后的scope指针
GlobalScope *AddEntryIntoGlobalScope(GlobalScope *scope,
                                     GlobalScopeEntry *entry) {
  if (scope == nullptr) {
    scope = (GlobalScope *)malloc(sizeof(GlobalScope));
    scope->clear();
  }

  scope->push_back(entry);
  return scope;
}

FormalScope *AddEntryIntoFormalScope(FormalScope *scope,
                                     FormalScopeEntry *entry) {
  if (scope == nullptr) {
    scope = (FormalScope *)malloc(sizeof(FormalScope));
    scope->clear();
  }

  scope->push_back(entry);
  return scope;
}

LocalScope *AddEntryIntoLocalScope(LocalScope *scope, LocalScopeEntry *entry) {
  if (scope == nullptr) {
    scope = (LocalScope *)malloc(sizeof(LocalScope));
    scope->clear();
  }

  scope->push_back(entry);
  return scope;
}

// 把int值放入dims vector中 若dims = null 则需要新建一个vector
vector<int> *AddIntoDimsVector(vector<int> *dims, int d) {
  if (!dims) {
    dims = (vector<int> *)malloc(sizeof(vector<int>));
    dims->clear();
  }
  dims->push_back(d);
  return dims;
}

// 把GrammarTree指针放入 exprs vector中 若 exprs = null 则需要新建一个vector
vector<GrammarTree> *AddIntoExprsVector(vector<GrammarTree> *exprs,
                                        GrammarTree e) {
  if (!exprs) {
    exprs = (vector<GrammarTree> *)malloc(sizeof(vector<GrammarTree>));
    exprs->clear();
  }
  exprs->push_back(e);
  return exprs;
}

// 把value加入到vector中 若 init = null 则需要新建一个vector
vector<ArrayInitValue> *NewArrayInitValueFromInt(vector<ArrayInitValue> *init,
                                                 int value) {
  if (!init) {
    init = (vector<ArrayInitValue> *)malloc(sizeof(vector<ArrayInitValue>));
    init->clear();
  }
  ArrayInitValue v = (ArrayInitValue)malloc(sizeof(ArrayInitVal));
  v->type = Value;
  v->value = value;

  init->push_back(v);
  return init;
}

vector<ArrayInitValue> *NewArrayInitValueFromExpr(vector<ArrayInitValue> *init,
                                                  GrammarTree expr) {
  if (!init) {
    init = (vector<ArrayInitValue> *)malloc(sizeof(vector<ArrayInitValue>));
    init->clear();
  }
  ArrayInitValue e = (ArrayInitValue)malloc(sizeof(ArrayInitVal));
  e->type = Exp;
  e->expr = expr;

  init->push_back(e);
  return init;
}

// 把tail中的元素连接到head后面 head tail都可以为Null
// add_par = true 时 需要在前后添加 { 和 }
vector<ArrayInitVal *> *MergeArrayInitValue(vector<ArrayInitValue> *head,
                                            vector<ArrayInitValue> *tail,
                                            bool add_par) {
  if (!head) {
    head = (vector<ArrayInitValue> *)malloc(sizeof(vector<ArrayInitValue>));
    head->clear();
  }
  if (add_par) {
    // add begin
    head->push_back(&LEFT);
  }
  if (tail) {

    for (vector<ArrayInitValue>::iterator it = tail->begin(); it != tail->end();
         it++) {
      head->push_back(*it);
    }
  }
  if (add_par) {
    // add end
    head->push_back(&RIGHT);
  }
  return head;
}

// 为local_scope中的entry设置统一的isConst标记
// 返回添加好的scope指针
LocalScope *AttachTypeToLocalScope(LocalScope *local_scope, bool is_const) {
  LocalScope::iterator it = local_scope->begin();

  while (it != local_scope->end()) {
    (*it)->is_const = is_const;
  }
  return local_scope;
}

// 组装一个ArrayInfo 返回其地址
ArrayInfo *NewArrayInfo(vector<int> *_dims,
                        vector<ArrayInitValue> *_raw_values) {
  ArrayInfo *instance = (ArrayInfo *)malloc(sizeof(ArrayInfo));
  instance->dims = _dims;
  instance->raw_values = _exprs;
  return instance;
}

// 新建一个ArrayInfo 只存储初值 其它字段置为null
ArrayInfo *NewInitValue(bool is_const, int const_value, GrammarTree var_value) {
  ArrayInfo *instance = (ArrayInfo *)malloc(sizeof(ArrayInfo));
  instance->const_init_value = const_value;
  instance->var_init_value = var_value;
  // FIXME is_const貌似没用
  return instance;
}

// 新建一个local_entry
// array / const array
LocalScopeEntry *NewLocalEntry(char *_name, bool _is_const,
                               ArrayInfo *_array_info) {
  LocalScopeEntry *instance =
      (LocalScopeEntry *)malloc(sizeof(LocalScopeEntry));

  char *cache = (char *)malloc(sizeof(char) * strlen(_name));
  instance->name = cache;

  instance->is_const = _is_const;
  instance->is_array = true;
  instance->is_block = false;

  instance->array_info = _array_info;

  return instance;
}
// int
LocalScopeEntry *NewLocalEntry(char *_name, GrammarTree _int_init_expr) {
  LocalScopeEntry *instance =
      (LocalScopeEntry *)malloc(sizeof(LocalScopeEntry));

  char *cache = (char *)malloc(sizeof(char) * strlen(_name));
  instance->name = cache;

  instance->is_const = false;
  instance->is_array = false;
  instance->is_block = false;

  instance->int_init_expr = _int_init_expr;

  return instance;
}
// const int
LocalScopeEntry *NewLocalEntry(char *_name, int _int_init_value) {
  LocalScopeEntry *instance =
      (LocalScopeEntry *)malloc(sizeof(LocalScopeEntry));

  char *cache = (char *)malloc(sizeof(char) * strlen(_name));
  instance->name = cache;

  instance->is_const = true;
  instance->is_array = false;
  instance->is_block = false;

  instance->int_init_value = _int_init_value;

  return instance;
}
// block
LocalScopeEntry *NewLocalEntry(char *_name,
                               vector<LocalScopeEntry *> *_embedded_scope) {
  LocalScopeEntry *instance =
      (LocalScopeEntry *)malloc(sizeof(LocalScopeEntry));

  char *cache = (char *)malloc(sizeof(char) * strlen(_name));
  instance->name = cache;

  instance->is_const = false;
  instance->is_array = false;
  instance->is_block = true;

  instance->embedded_scope = _embedded_scope;

  return instance;
}

// 新建一个formal_entry
FormalScopeEntry *NewFormalEntry(char *_name, bool _is_array,
                                 ArrayInfo *_array_info) {
  FormalScopeEntry *instance =
      (FormalScopeEntry *)malloc(sizeof(FormalScopeEntry));

  char *cache = (char *)malloc(sizeof(char) * strlen(_name));
  instance->name = cache;

  instance->is_array = _is_array;
  // FIXME 这里直接传指针可能会有问题
  instance->array_info = _array_info;
  return instance;
}

// 新建一个global_entry
GlobalScopeEntry *NewGlobalEntry(char *_name, bool _is_function, bool _is_void,
                                 ArrayInfo *_array_info, int _formal_count,
                                 FormalScope *_formal_scope,
                                 LocalScope *_local_scope) {
  GlobalScopeEntry *instance =
      (GlobalScopeEntry *)malloc(sizeof(GlobalScopeEntry));

  char *cache = (char *)malloc(sizeof(char) * strlen(_name));
  instance->name = cache;

  instance->is_func = _is_function;
  instance->is_void = _is_void;
  instance->formal_count = _formal_count;
  // FIXME 这里直接传指针可能会有问题
  instance->array_info = _array_info;
  instance->formal_scope = _formal_scope;
  instance->local_scope = _local_scope;
  return instance;
}

//------------------
// Display Functions
//------------------

void DisplayArrayInfo(ArrayInfo *array_info) {
  // todo
  puts("Not Implemented!");
}

void DisplayLocalScope(LocalScope *local_symtable) {
  // for (LocalScope::iterator it = local_symtable->begin();
  //      it != local_symtable->end(); it++) {
  //   if ((*it)->is_array) {
  //     puts("┌────────────────┬─────────────┐");
  //     puts("| Name           | is Constant |");
  //     printf("| %14s ", (*it)->name);
  //     if ((*it)->is_const)
  //       printf("| True        |\n");
  //     else
  //       printf("| False       |\n");
  //     printf("Array %s's Info Vector\n", (*it)->name);
  //     DisplayArrayInfo((*it)->array_info);
  //   } else if ((*it)->is_block) {
  //     puts("Embedded Scope");
  //     // DisplayLocalScope((*it)->embedded_scope);
  //   } else {
  //     puts("┌────────────────┬─────────────┬───────────────┐");
  //     puts("| Name           | is Constant | Init Value    |");
  //     printf("| %14s ", (*it)->name);
  //     if ((*it)->is_const)
  //       printf("| True        ");
  //     else
  //       printf("| False       ");
  //     printf("| %13d |\n", 0); // TODO: print real init value
  //   }
  //   puts("├────────────────┼─────────────┼───────────────┤");
  // }
  // puts("└────────────────┴─────────────┴───────────────┘");
}

void DisplayFormalScope(FormalScope *formal_symtable) {
  // for (FormalScope::iterator it = formal_symtable->begin();
  //      it != formal_symtable->end(); it++) {
  //   puts("| Name           | Type  |");
  //   printf("| %14s ", (*it)->name);
  //   if ((*it)->is_array) {
  //     puts("| Array |");
  //     DisplayArrayInfo((*it)->array_info);
  //   } else
  //     puts("|  Int  |");
  // }
}

void DisplayGlobalScope(GlobalScope *global_symtable) {
  // // 打印全局变量
  // printf("Global Variables:\n");
  // puts("┌────────────────┬─────────────┐");
  // puts("| Name           | is Constant |");
  // for (GlobalScope::iterator it = global_symtable->begin();
  //      it != global_symtable->end(); it++) {
  //   if (!(*it)->is_func) {
  //     // todo
  //   }
  // }

  // // 打印函数信息
  // printf("\nFunctions:\n\n");
  // for (GlobalScope::iterator it = global_symtable->begin();
  //      it != global_symtable->end(); it++) {
  //   if ((*it)->is_func) {
  //     printf("Function %s:\n", (*it)->name);
  //     printf("┌─────────────┬─────────────┐\n");
  //     printf("│ Return Type │ Para Number │\n");
  //     puts("├─────────────┼─────────────┤");
  //     if ((*it)->is_void)
  //       cout << "│ Void        ";
  //     else
  //       cout << "│ Int         ";
  //     printf("│ %11d │\n", (*it)->formal_count);
  //     cout << "└─────────────┴─────────────┘" << endl;
  //     // 打印函数形参
  //     if ((*it)->formal_count && (*it)->formal_scope) {
  //       cout << "Function " << (*it)->name << "'s Formal Scope\n";
  //       DisplayFormalScope((*it)->formal_scope);
  //       cout << "End of Function " << (*it)->name << "'s Formal Scope\n";
  //     }
  //     // 打印函数局部作用域
  //     if ((*it)->local_scope) {
  //       cout << "Function " << (*it)->name << "'s Local Scope\n";
  //       DisplayLocalScope((*it)->local_scope);
  //       cout << "End of Function " << (*it)->name << "'s Local Scope\n";
  //     }
  //   }
  // }
}