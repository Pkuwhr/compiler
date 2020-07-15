/*
 * @Date: 2020-07-01 15:19:44
 * @LastEditors: zyk
 * @LastEditTime: 2020-07-15 15:33:08
 * @FilePath: /compiler/main.c
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
    printf("\nNow print the two-tuples of \"Lexical Analyzing\":\n");
    printf("__________________________________________________\n\n");
  }
  yyparse();
  return 0;
}