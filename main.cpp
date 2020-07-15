/*
 * @Date: 2020-07-01 15:19:44
 * @LastEditors: zyk
 * @LastEditTime: 2020-07-15 21:41:20
 * @FilePath: /compiler/main.cpp
 */

#include "main.h"

int tuple_trigger = 1;
int gmerror;            // the errors in grammar analyzing

int main(int argc, char **argv) {
  gmerror = 0;
  if (argc > 1) {
    if (!(yyin = fopen(argv[1], "r"))) {
      perror(argv[1]);
      return 1;
    }
  }
  if (tuple_trigger) {
    std::cout << "\nNow print the two-tuples of \"Lexical Analyzing\":\n";
    cout << "__________________________________________________\n\n";
  }
  yyparse();
  return 0;
}