project(compiler)

cmake_minimum_required(VERSION 3.13)

add_executable(compiler lex.yy.c y.tab.c GrammarTree.c Nonterminals.c)
