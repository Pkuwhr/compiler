#include "ArrayInfo.h"

// 所有0值共用一个变量
ArrayInitVal ZERO, LEFT, RIGHT;

int ArrayInfo::size() { return this->dims->size(); }

void ArrayInfo::init() {
  if (!raw_values) return;
  init_values = (vector<ArrayInitValue> *)malloc(sizeof(vector<ArrayInitValue>));
  if (!init_values) {
    puts("Error: Cannot malloc space for init_values vector!!");
    exit(-1);
  }

  ZERO.type = Value;
  ZERO.value = 0;
  LEFT.type = Begin;
  RIGHT.type = End;

  const int max_layer =
      size(); // 超过最大层数后 只取{}里的第一个数字 忽略其它数字
  init_values->clear();

  int cur_layer = 0, left = 1, elem_index = 0;
  for (vector<int>::iterator it = dims->begin(); it != dims->end(); it++) {
    left *= *it;
  }

  for (vector<ArrayInitValue>::iterator it = raw_values->begin();
       it != raw_values->end(); it++) {
    switch ((*it)->type) {
    case Begin:
      cur_layer++;

      // 计算left值 即当前层剩余的变量数
      if (cur_layer > max_layer) {
        left = 1;
      } else {
        int modulo = 1;
        for (int i = 1; i <= max_layer - cur_layer + 1; ++i) {
          modulo *= (*dims)[dims->size() - i];
        }
        // modulo为本层变量数
        // 根据elem_index和modulo计算left
        left = (elem_index / modulo + 1) * modulo - elem_index;
      }
      break;
    case End:
      while (left) {
        init_values->push_back(&ZERO);
        left--;
        elem_index++;
      }
      cur_layer--;
      break;
    case Expr:
    case Value:
      if (left) {
        init_values->push_back(*it);
        left--;
        elem_index++;
      }
      break;
    default:
      break;
    }
  }
}

ArrayInitValue ArrayInfo::get_element(vector<int> indices) {
  int index = 0;
  for (int i = 0; i < indices.size(); ++i) {
    int factor = 1;
    for (int j = i + 1; j < dims->size(); ++j) {
      factor *= (*dims)[j];
    }
    index += indices[i] * factor;
  }
  return (*init_values)[index];
}
