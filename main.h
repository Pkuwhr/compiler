/*
 * @Date: 2020-07-01 15:18:34
 * @LastEditors: zyk
 * @LastEditTime: 2020-07-15 21:42:49
 * @FilePath: /compiler/main.h
 */

#ifndef _MAIN_H
#define _MAIN_H

#include <iostream>
#include <cstdio>

#include "GrammarTree.h"
#include "parser.tab.h"
extern "C" {
    
    extern FILE *yyin;
}

using namespace std;

#endif // _MAIN_H