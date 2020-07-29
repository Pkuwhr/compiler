/*
 * @Date: 2020-07-15 14:59:31
 * @LastEditors: zyk
 * @LastEditTime: 2020-07-29 21:05:17
 * @FilePath: \compiler\SymbolTable.cpp
 */ 

#include"SymbolTable.h"
#include"ArrayInfo.h"

// 把local_scope中的entry填入global_scope中 注意**local_scope中不能有Block**
GlobalScope *AddLocalIntoGlobal(GlobalScope *global_scope,LocalScope *local_scope) {
    if(global_scope==NULL) {
        global_scope = (GlobalScope *) malloc(sizeof(GlobalScope));
    }

    LocalScope::iterator it = local_scope->begin();
    while ( it != local_scope->end() ) {
        // add *it into global_scope
        LocalScopeEntry local_entry = *it;
        GlobalScopeEntry global_entry;

        global_entry.name = local_entry.name;
        global_entry.is_func = false;
        global_entry.is_void = false;
        // FIXME 这里貌似有歧义
        // Local
        // global_entry.local_scope = &local_entry;

        global_scope->push_back(global_entry);
        it++;
    }
    return global_scope;
}
// 把local_scope放入另一个local_scope中
// ! 注意head为Null的情况 !
LocalScope *AddLocalIntoLocal(LocalScope *head, LocalScope *tail) {
    if (head == NULL) {
        head = (LocalScope *) malloc(sizeof(LocalScope));
    }

    LocalScope::iterator it = tail->begin();
    while (it != tail->end()) {
        // add *it into head
        LocalScopeEntry local_entry = *it;

        head->push_back(local_entry);
        it++;
    }
    return head;
}
// 把entry放入scope中 且返回加入后的scope指针
// ! 若scope = nullptr 则需要创建一个scope !
GlobalScope *AddEntryIntoGlobalScope(GlobalScope *scope, GlobalScopeEntry *entry) {
    if (scope == NULL ) {
        scope = (GlobalScope*) malloc(sizeof(GlobalScope));
    }

    scope->push_back(*entry);
    return scope;
}
FormalScope *AddEntryIntoFormalScope(FormalScope *scope,FormalScopeEntry *entry) {
    if (scope == NULL ) {
        scope = (FormalScope*) malloc(sizeof(FormalScope));
    }

    scope->push_back(*entry);
    return scope;
}
LocalScope *AddEntryIntoLocalScope(LocalScope *scope, LocalScopeEntry *entry) {
    if (scope == NULL ) {
        scope = (LocalScope*) malloc(sizeof(LocalScope));
    }
    
    scope->push_back(*entry);
    return scope;
}

// 把int值放入dims vector中 若dims = null 则需要新建一个vector
vector<int> *AddIntoDimsVector(vector<int> *dims, int d) {
    // FIXME
    dims->push_back(d);
    return dims;
}
// 把GrammarTree指针放入 exprs vector中 若 exprs = null 则需要新建一个vector
vector<int> *AddIntoExprsVector(vector<int> *exprs, int e) {
    // FIXME 
    exprs->push_back(e);
    return exprs;
}
// 把ArrayInitValue加入到vector中 若 init = null 则需要新建一个vector
// add_sep = true时 添加value后 继续添加一个sep
vector<ArrayInitValue> *AddExprIntoArrayInitValue(vector<ArrayInitValue> *init,int value, bool add_sep) {
    ArrayInitValue tmp;
    tmp->isSeparator=add_sep;
    tmp->value = value;
    init->push_back(tmp);
    return init;
}
// 把tail中的元素连接到head后面 head tail都可以为Null
// ! 连接之后务必加上一个sep分隔符 !
vector<ArrayInitValue> *MergeArrayInitValue(vector<ArrayInitValue> *head,vector<ArrayInitValue> *tail) {
    vector<ArrayInitValue>::iterator it = tail->begin();
    while(it != tail->end()) {
        head->push_back(*it);
        it++;
    }
    // 加上sep
    ArrayInitValue last;
    last->isSeparator=true;
    head->push_back(last);
    return head;
}
// 为local_scope中的entry设置统一的isConst标记
// 返回添加好的scope指针
LocalScope *AttachTypeToLocalScope(LocalScope *local_scope, bool is_const) {
    LocalScope::iterator it = local_scope->begin();

    while( it != local_scope->end()) {
        it->is_const = is_const;
    }
    return local_scope;
}

// 组装一个ArrayInfo 返回其地址
ArrayInfo *NewArrayInfo(vector<int> *_dims, vector<GrammarTree> *_exprs,vector<ArrayInitValue> *_init_values) {
    ArrayInfo* instance = (ArrayInfo*) malloc(sizeof(ArrayInfo));
    instance->dims = *_dims;
    instance->exprs = *_exprs;
    instance->init_values = *_init_values;
    return instance;
}
// 新建一个ArrayInfo 只存储初值 其它字段置为null
ArrayInfo *NewInitValue(bool is_const, int const_value, GrammarTree var_value) {
    ArrayInfo* instance = (ArrayInfo*) malloc(sizeof(ArrayInfo));
    instance->const_init_value = const_value;
    instance->var_init_value = var_value;
    // FIXME is_const貌似没用
    return instance;
}
// 新建一个local_entry
LocalScopeEntry *NewLocalEntry(char *_name, bool _is_array, bool _is_block,ArrayInfo *_array_info, LocalScope *local_scope) {
    LocalScopeEntry* instance = (LocalScopeEntry*) malloc(sizeof(LocalScopeEntry));

    char* cache = (char*)malloc(sizeof(char) * strlen(_name));
    instance->name = cache;

    instance->is_array = _is_array;
    instance->is_block = _is_block;
    // FIXME 这里直接传指针可能会有问题
    instance->array_info = _array_info;
    // FIXME 这里直接传指针可能会有问题
    instance->embedded_scope = local_scope;
    return instance;
}
// 新建一个formal_entry
FormalScopeEntry *NewFormalEntry(char *_name, bool _is_array,ArrayInfo *_array_info) {
    FormalScopeEntry* instance = (FormalScopeEntry*) malloc(sizeof(FormalScopeEntry));

    char* cache = (char*)malloc(sizeof(char) * strlen(_name));
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
    GlobalScopeEntry* instance = (GlobalScopeEntry*) malloc(sizeof(GlobalScopeEntry));

    char* cache = (char*)malloc(sizeof(char) * strlen(_name));
    instance->name = cache;

    instance->is_func=_is_function;
    instance->is_void=_is_void;
    instance->formal_count=_formal_count;
    // FIXME 这里直接传指针可能会有问题
    instance->array_info = _array_info;
    instance->formal_scope = _formal_scope;
    instance->local_scope = _local_scope;
    return instance;
}
