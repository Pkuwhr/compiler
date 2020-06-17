/*
 * @Date: 2020-06-13 17:07:18
 * @LastEditors: zyk
 * @LastEditTime: 2020-06-15 21:01:43
 * @FilePath: \compiler\GrammarTree.c
 */


#include"GrammarTree.h"
#include"Nonterminals.h"
#include"y.tab.h"

extern int tuple_trigger;

GrammarTree CreateGrammarTree(int type, int num, ...) {
    char *value_buffer; // 存放标识符名称/常量的值
    va_list variables; // 可变参数列表
    GrammarTree current = (GrammarTree) malloc(
            sizeof(GrammarTreeNode));  // 当前输入所有节点的父节点
    current->type = type; // 标识父节点类型
    va_start(variables, num);  // 初始化可变参数列表
    if (num > 0) // 将num后面的参数设置为current的子节点
    {
        GrammarTree tmp = va_arg(variables, GrammarTree); // 获取列表中的第一个节点
        current->lchild = tmp; // 将第一个节点设置为current的右子节点（即孩子节点）
        current->line = tmp->line;      // Current grammar unit's line number is equal to its left child's
        for (int i = 0; i < num - 1; i++) // 处理剩下的(num - 1)个节点，将他们设置为第一个节点的兄弟
        {                               // Brothers
            tmp->rchild = va_arg(variables, GrammarTree);
            tmp = tmp->rchild; // 可以视为将指针移动到链表尾
        }
        tmp->rchild = NULL; // make sure the end flag is set correctly
        // Modify the line number when reduce using rule "Stmt->SimpleStmt" and SimpleStmt is Epsilon
         if ((current->lchild->type == ConstInitVal || current->lchild->type == Block) && current->lchild->line == -1)
             current->line = current->lchild->rchild->line;
    } else {   // 处理终结符或空规则
        current->line = va_arg(variables,
        int); // 行号

        switch (current->type) // 此变量正是bison中定义的token和type
        {
            case T_Identifier:
            case T_StringConstant:
                // 将标识符名称放入string_value
                value_buffer = (char *) malloc(sizeof(char) * strlen(yytext));
                strcpy(value_buffer, yytext);
                current->string_value = value_buffer;
                break;
            case T_IntConstant:
                // 计算不同进制的常量值
                if (yytext[0] == '0' && (yytext[1] == 'x' || yytext[1] == 'X')) {
                    current->int_value = (int) strtol(yytext, NULL, 16);
                } else if (yytext[0] == '0') {
                    current->int_value = (int) strtol(yytext, NULL, 8);
                } else {
                    current->int_value = (int) strtol(yytext, NULL, 10);
                }
                break;
            default:
                break;
        }
    }
    return current;
}

void TraverseGrammarTree(GrammarTree gmtree, int level) {
    if (gmtree != NULL) {
        for (int i = 0; i < level; i++)
            printf("    ");
        if (gmtree->line != -1) {
            // Not void rule
            printf("%s", NameOf(gmtree->type));
            if (gmtree->type == T_Identifier || gmtree->type == T_StringConstant)
                printf(": %s\n", gmtree->string_value);
            else if (gmtree->type == T_IntConstant)
                printf(": %d\n", gmtree->int_value);
            else
                printf(" (%d)\n", gmtree->line);
        } else
            printf("%s (Epsilon)\n", NameOf(gmtree->type));
        TraverseGrammarTree(gmtree->lchild, level + 1);
        TraverseGrammarTree(gmtree->rchild, level);
    }
}

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
