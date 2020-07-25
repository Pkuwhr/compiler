/*
 * @Date: 2020-07-15 16:06:28
 * @LastEditors: zyk
 * @LastEditTime: 2020-07-25 10:23:52
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
    int *values; // 动态创建数组存储初值
    // TODO: 此数组初值的存储可能仍需要修改

    bool init(vector<int> dims, int **pvalues); // 输入维度 创建数组
} ArrayInitValue;

typedef struct ArrayInfo {
    vector<int> dims; // 数组维度向量
    vector<GrammarTree*> exprs; // 运行时计算的表达式

    // - 数组初值 -
    // * 使用时需要先分配空间
    // * 对于没有初值的数组 默认为Null
    struct ArrayInitValue *init_value;
} ArrayInfo;

#endif
