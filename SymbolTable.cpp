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
        global_entry.formal_count = nullptr;
        global_entry.local_scope = nullptr;

        global_scope.push_back(global_entry);
        it++;
    }
    return global_scope;
}