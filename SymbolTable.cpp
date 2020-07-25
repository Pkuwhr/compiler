/*
 * @Date: 2020-07-15 14:59:31
 * @LastEditors: zyk
 * @LastEditTime: 2020-07-25 10:36:58
 * @FilePath: /compiler/SymbolTable.cpp
 */ 

#include"SymbolTable.h"

GlobalScope &AddLocalIntoGlobal(GlobalScope &global_scope, LocalScope &local_scope) {
    // 判断global_scope为空的情况
    if (&global_scope == NULL) {
        // FIXME: 这里的创建方式可能有问题
        GlobalScope temp;
        global_scope = temp;
        global_scope.clear();
    }

    LocalScope::iterator it = local_scope.begin();
    while (it != local_scope.end()) {
        // add *it into global_scope
        LocalScopeEntry local_entry = *it;
        GlobalScopeEntry global_entry;
        
        global_entry.name = local_entry.name;
        global_entry.symbol_type = local_entry.symbol_type;
        if (local_entry.symbol_type == ConstArray)
            global_entry.array_info = local_entry.array_info;
        else if (local_entry.symbol_type == ConstInt)
            global_entry.init_val = local_entry.int_init_val;
        global_entry.formal_count = 0;
        global_entry.local_scope = nullptr;

        global_scope.push_back(global_entry);
        it++;
    }
    return global_scope;
}

GlobalScope &AddGlobalIntoGlobal(GlobalScope &global_scope_1,GlobalScope &global_scope_2) {
    if(&global_scope_1 == NULL) {
        GlobalScope temp;
        global_scope_1 = temp;
        global_scope_1.clear();
    }

    GlobalScope::iterator it = global_scope_2.begin();
    while ( it != global_scope_2.end()) {
        // add *it into global_scope_1
        GlobalScopeEntry global_entry_2;

        global_entry_2=*it;
        global_scope_1.push_back(global_entry_2);
        it++;
    }
    return global_scope_1;
}

LocalScope &AddLocalIntoLocal(LocalScope &local_scope_1,LocalScope &local_scope_2) {
    if(&local_scope_1==NULL) {
        LocalScope temp;
        local_scope_1 = temp;
        temp.clear();
    }

    LocalScope::iterator it = local_scope_2.begin();

    while ( it != local_scope_2.end()) {
        LocalScopeEntry local_entry_2;

        local_entry_2 = *it;
        local_scope_1.push_back(local_entry_2);
        it++;
    }
    return local_scope_1;
}

void AttachTypeToLocalScope(LocalScope local_scope,SymbolCategory category) {
    LocalScope::iterator it = local_scope.begin();

    while ( it != local_scope.end()) {
        it->symbol_type = category;
    }
}

LocalScopeEntry &NewLocalIntEntry(char* name,int val) {
    LocalScopeEntry local_entry;

    char* cache = (char*)malloc(sizeof(char) * strlen(name));

    local_entry.name = cache;
    local_entry.symbol_type = VarInt;
    local_entry.int_init_val = val;
    return local_entry;
}

LocalScopeEntry &NewLocalArrayEntry(char* name,ArrayInfo *array_info) {
    LocalScopeEntry local_entry;

    char* cache = (char*)malloc(sizeof(char) * strlen(name));
    local_entry.name = cache;
    local_entry.symbol_type = VarArray;
    local_entry.array_info = array_info;
    return local_entry;
}