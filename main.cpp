/*
 * @Date: 2020-07-01 15:19:44
 * @LastEditors: zyk
 * @LastEditTime: 2020-07-25 10:24:40
 * @FilePath: /compiler/main.cpp
 */

#include "main.h"

int tuple_trigger = 1;
int gmerror;            // the errors in grammar analyzing
int smerror;

int semantic_check = 1; // semantic_check != 0时执行语义检查ScopeTrial

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
