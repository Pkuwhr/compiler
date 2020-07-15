/*
 * @Date: 2020-07-15 16:06:28
 * @LastEditors: zyk
 * @LastEditTime: 2020-07-15 16:34:06
 * @FilePath: /compiler/ArrayInfo.h
 */ 

#ifndef _ARRAYINFO_H
#define _ARRAYINFO_H

#include <vector>
// #include <iostream>
#include <cstring>

using namespace std;

typedef struct ArrayInitValue {
    int *values; // 动态创建数组存储初值

    bool init(vector<int> dims, int **pvalues); // 输入维度 创建数组
} ArrayInitValue;

#endif
