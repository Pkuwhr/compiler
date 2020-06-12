/*
 * @Author: zyk
 * @Date: 2020-05-31 18:16:59
 * @LastEditTime: 2020-05-31 20:24:01
 * @LastEditors: zyk
 * @Description: You should not need to modify this file. It declare a few constants,
 *              types, variables,and functions that are used and/or exported by
 *              the lex-generated scanner.
 * @FilePath: \compiler\scanner.h
 */ 


#ifndef _H_scanner
#define _H_scanner

#include <stdio.h>

#define MaxIdentLen 31    // Maximum length for identifiers fixme: not sure if sysY define this

extern char *yytext;      // Text of lexeme just scanned


int yylex();              // Defined in the generated lex.yy.c file

void InitScanner();                 // Defined in scanner.l user subroutines
const char *GetLineNumbered(int n); // ditto
 
#endif
