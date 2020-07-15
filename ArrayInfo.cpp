/*
 * @Date: 2020-07-15 16:10:45
 * @LastEditors: zyk
 * @LastEditTime: 2020-07-15 16:35:39
 * @FilePath: /compiler/ArrayInfo.cpp
 */ 

#include "ArrayInfo.h"

bool ArrayInitValue::init(vector<int> dims, int **pvalues) {
    int ele_cnt = 1; // 数组中元素总数
    
    vector<int>::iterator it;
    for (it = dims.begin(); it != dims.end(); it++) {
        ele_cnt *= *it;
    }

    // cout << ele_cnt << endl;
    
    *pvalues = (int *) malloc(ele_cnt * sizeof(int));
    if (!(*pvalues)) {
        return false;
    }
    else {
        memset(*pvalues, 0, ele_cnt * sizeof(int));
        return true;
    }
}

// int main(int argc, char const *argv[])
// {
//     // test array info
//     vector<int> dims;

//     dims.clear();
//     dims.push_back(2);
//     dims.push_back(2);
//     dims.push_back(4);

//     ArrayInitValue val;
//     val.init(dims, &(val.values));

//     for (int i = 0; i < 2; ++i)
//         for (int j = 0; j < 2; ++j)
//             for (int k = 0; k < 4; ++k)
//                 cout << val.values[i*2*4+j*4+k] << endl;

//     return 0;
// }
