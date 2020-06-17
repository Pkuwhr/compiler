project(compiler)

cmake_minimum_required(VERSION 3.13)

add_executable(compiler lex.yy.c y.tab.c GrammarTree.c Nonterminals.c)

# TODO: fix this so that you don't need to run make to generate lex.yy.c and y.tab.c first