/*
 * @Author: lfq
 * @Date: 2020-05-28 11:58:08
 * @LastEditTime: 2020-05-28 14:41:01
 * @LastEditors: Please set LastEditors
 * @Description: copy from 13.
 *               This is a debug tool. 
 *               Use "Assert" in any place you want to test the errors if you has included "utility.h"
 *               After fixing ever bugs, please remove this file.
 * @FilePath: \compiler\utility.h
 */ 



#ifndef _H_utility
#define _H_utility

#include <stdlib.h>
#include <stdio.h>



void Failure(const char *format, ...);



#define Assert(expr)  \
  ((expr) ? (void)0 : Failure("Assertion failed: %s, line %d:\n    %s", __FILE__, __LINE__, #expr))



void PrintDebug(const char *key, const char *format, ...);



void SetDebugForKey(const char *key, bool val);



bool IsDebugOn(const char *key);



void ParseCommandLine(int argc, char *argv[]);
     
#endif
