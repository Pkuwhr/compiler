/*
 * @Date: 2020-07-15 16:06:28
 * @LastEditors: zyk
 * @LastEditTime: 2020-07-26 21:16:59
 * @FilePath: \compiler\ArrayInfo.h
 */

#ifndef _ARRAYINFO_H
#define _ARRAYINFO_H

#include <vector>
// #include <iostream>
#include <cassert>
#include <cstdlib>
#include <cstring>

#include "GrammarTree.h"

using namespace std;

typedef struct ArrayInitVal {
  bool isSeparator; // 每当遇到一个'}'时 添加一个Separator到vector中
  union {
    int value;        // 遇到正常数字时 添加其到vector中
    GrammarTree expr; // 未计算的表达式
  };
} ArrayInitVal;

typedef ArrayInitVal *ArrayInitValue;

typedef struct ArrayInfo {
  int const_init_value;               // 常量初值
  GrammarTree var_init_value;         // 变量初值
  vector<int> dims;                   // 数组维度向量
  vector<GrammarTree> exprs;          // 运行时计算的表达式
  vector<ArrayInitValue> init_values; // 含分隔符的数组初值

  // 获取数组的维数
  int size();
  /**
   * input: vector<int> index: 元素是每一维的索引 例如需要array[1][2]出的元素
   * 则输入[1, 2]这个vector output: int: 数组对应位置的元素
   * 若index.size()<dims.size() 则返回结果**未定义**
   */
  int get(vector<int> &index);

} ArrayInfo;

// 把init_val中的值附在seq后
void MergeInitValIntoSeq(vector<ArrayInitValue> &init_val,
                         vector<ArrayInitValue> &seq);
vector<ArrayInitValue> *NewInitValFromExp(GrammarTree exp);
void PushSeparator(vector<ArrayInitValue> &init_val);

#endif
