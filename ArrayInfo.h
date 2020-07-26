/*
 * @Date: 2020-07-15 16:06:28
 * @LastEditors: zyk
 * @LastEditTime: 2020-07-26 16:43:12
 * @FilePath: /compiler/ArrayInfo.h
 */ 

#ifndef _ARRAYINFO_H
#define _ARRAYINFO_H

#include <vector>
// #include <iostream>
#include <cstring>
#include <cstdlib>

#include "GrammarTree.h"

using namespace std;

typedef struct ArrayInitValue {
    bool isSeparator; // 每当遇到一个'}'时 添加一个Separator到vector中
    int value; // 遇到正常数字时 添加其到vector中
} ArrayInitValue;

typedef struct ArrayInfo {
    int const_init_value; // 常量初值
    GrammarTree var_init_value; // 变量初值
    vector<int> dims; // 数组维度向量
    vector<GrammarTree> exprs; // 运行时计算的表达式
    vector<ArrayInitValue> init_values; // 含分隔符的数组初值
} ArrayInfo;

// 把init_val中的值附在seq后
void MergeInitValIntoSeq(vector<ArrayInitValue> &init_val, vector<ArrayInitValue> &seq);
vector<ArrayInitValue> *NewInitValFromExp(GrammarTree exp);
void PushSeparator(vector<ArrayInitValue> *init_val);


#endif
