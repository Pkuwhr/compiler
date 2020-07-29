/*
 * @Date: 2020-07-15 16:10:45
 * @LastEditors: zyk
 * @LastEditTime: 2020-07-29 21:02:58
 * @FilePath: \compiler\ArrayInfo.cpp
 */

#include "ArrayInfo.h"

int ArrayInfo::size() { return this->dims.size(); }

// 判断index是否能被factors中的某个数整除
bool isLastElement(int index, vector<int> &factors) {
  for (vector<int>::iterator it = factors.begin(); it != factors.end(); it++) {
    if (index % *it == 0)
      return true;
  }
  return false;
}

int ArrayInfo::getInt(vector<int> &index) {
  return 0;
  // assert(index.size() == dims.size());

  // int linear_idx = 0;

  // vector<int> factors;
  // factors.clear();
  
  // // calculate the linear idx of target
  // for (int i = 0; i < index.size(); i++) {
  //   int factor = 1;
  //   for (int j = i + 1; j < dims.size(); j++) {
  //     factor *= dims[j];
  //   }
  //   factors.push_back(factor);
  //   linear_idx += index[i] * factor;
  // }

  // int pointer = 0; // 当前处理到的位置
  // int real_idx = 0; // pointer对应的元素在数组中实际的位置 real_idx <= pointer
  // int result = 0; // 所求位置元素的值

  // while (real_idx < linear_idx) {
  //   if (!init_values[pointer]->isSeparator) {
  //     real_idx++;
  //     result = init_values[pointer];
  //   } else {
  //     while (!isLastElement(real_idx, factors)) {
  //       real_idx += 1; // TODO: 这里可以优化 real_idx每次增加到
  //       result = 0;
  //     }
  //   }
  //   pointer++;
  // }

  // return result;
}

// TODO
GrammarTree ArrayInfo::getExpr(vector<int> &index) { return nullptr; }

void MergeInitValIntoSeq(vector<ArrayInitValue> &init_val,
                         vector<ArrayInitValue> &seq) {
  // append init_val to seq
  for (vector<ArrayInitValue>::iterator it = init_val.begin();
       it != init_val.end(); it++) {
    seq.push_back(*it);
  }
}

vector<ArrayInitValue> *NewInitValFromExp(GrammarTree exp) {
  vector<ArrayInitValue> *rev =
      (vector<ArrayInitValue> *)malloc(sizeof(vector<ArrayInitValue>));
  ArrayInitValue val = (ArrayInitValue)malloc(sizeof(ArrayInitValue));
  val->isSeparator = false;
  val->expr = exp;
  rev->push_back(val);
  
  return rev;
}

void PushSeparator(vector<ArrayInitValue> &init_val) {
  ArrayInitValue sep = (ArrayInitValue)malloc(sizeof(ArrayInitValue));
  sep->isSeparator = true;
  init_val.push_back(sep);
}
