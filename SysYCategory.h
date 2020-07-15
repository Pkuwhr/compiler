/*
 * @Date: 2020-07-01 11:31:14
 * @LastEditors: zyk
 * @LastEditTime: 2020-07-15 15:22:48
 * @FilePath: /compiler/SysYCategory.h
 */

#ifndef _SYSYCATEGORY_H
#define _SYSYCATEGORY_H

// 符号类型
typedef enum SymbolCategory {
  VarArray,   // 数组变量
  ConstArray, // 数组常量
  VarInt,     // 整型变量
  ConstInt,   // 整型常量
  Function,   // 函数
  LibFunc,    // 库函数
  EmbeddedBlock,      // 内嵌语句块
  None        // 缺省类型
} SymbolCategory;

// 返回值类型
typedef enum ReturnCategory { Void, Integer } ReturnCategory;

#endif // _SYSYCATEGORY_H
