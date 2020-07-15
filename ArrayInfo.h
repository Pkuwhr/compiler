/*
 * @Date: 2020-07-15 16:06:28
 * @LastEditors: zyk
 * @LastEditTime: 2020-07-15 21:25:23
 * @FilePath: /compiler/ArrayInfo.h
 */ 

#ifndef _ARRAYINFO_H
#define _ARRAYINFO_H

#include <vector>
// #include <iostream>
#include <cstring>
#include <cstdlib>

using namespace std;

typedef struct ArrayInitValue {
    int *values; // 动态创建数组存储初值

    bool init(vector<int> dims, int **pvalues); // 输入维度 创建数组
} ArrayInitValue;

typedef struct ArrayInfo {
    vector<int> dims; // 数组维度向量

    // - 数组初值 -
    // * 使用时需要先分配空间
    // * 对于没有初值的数组 默认为Null
    struct ArrayInitValue *init_value;
} ArrayInfo;

#endif
