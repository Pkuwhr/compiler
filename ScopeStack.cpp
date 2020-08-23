#include "ScopeStack.h"
#include "parser.tab.h"

void InitScopeStack(ScopeStack *stack) { stack->clear(); }

void DestroyScopeStack(ScopeStack *stack) { stack->clear(); }

int ScopeStackLength(ScopeStack *stack) { return stack->size(); }

void ScopeStackPush(ScopeStack *stack, Scope scope) { stack->push_back(scope); }

Scope &ScopeStackPop(ScopeStack *stack) {
  Scope &top = stack->at(stack->size() - 1);
  stack->pop_back();
  return top;
}

Scope &GetStackTop(ScopeStack *stack) { return stack->at(stack->size() - 1); }

ScopeEntry TraverseScopeStack(ScopeStack *stack, char *name) {
  // traverse the stack and find certain name
  ScopeEntry result;
  LocalScope *local_symtable;
  FormalScope *formal_symtable;
  GlobalScope *global_symtable;

  for (ScopeStack::iterator it = stack->end(); it != stack->begin(); it--) {
    switch (it->type) {
    case ScopeLocal:
      local_symtable = it->local;
      for (LocalScope::iterator i = local_symtable->begin();
           i != local_symtable->end(); i++) {
        if (!strcmp((*i)->name, name)) {
          result.type = ScopeLocal;
          result.local_entry = *i;
          return result;
        }
      }
      break;
    case ScopeFormal:
      formal_symtable = it->formal;
      for (FormalScope::iterator i = formal_symtable->begin();
           i != formal_symtable->end(); i++) {
        if (!strcmp((*i)->name, name)) {
          result.type = ScopeFormal;
          result.formal_entry = *i;
          return result;
        }
      }
      break;
    case ScopeGlobal:
      global_symtable = it->global;
      for (GlobalScope::iterator i = global_symtable->begin();
           i != global_symtable->end(); i++) {
        if (!strcmp((*i)->name, name)) {
          result.type = ScopeGlobal;
          result.global_entry = *i;
          return result;
        }
      }
      break;

    default:
      break;
    }
  }
  result.type = ScopeNone;
  result.local_entry = nullptr;
  return result;
}

// check if given root is a constant expr
void CheckExprValue(GrammarTree tree) {
  // 判断 tree 是否为空
  if (!tree) return;

  if (tree->type != Exp) {
    puts("Invalid Expr!");
    exit(-1);
  }

  // * Exp 'op' Exp 形式
  if (tree->lchild->type == Exp) {
    // 检查两个子 Exp 节点
    if (!tree->lchild->isVisited)
      CheckExprValue(tree->lchild);
    if (!tree->lchild->rchild->rchild->isVisited)
      CheckExprValue(tree->lchild->rchild->rchild);

    if (tree->lchild->is_constant_expr &&
        tree->lchild->rchild->rchild->is_constant_expr) {
      // 两个操作数为常量 故 tree 也是常量
      tree->is_constant_expr = true;

      switch (tree->lchild->rchild->type) { // 处理不同操作符
      case '*':
        tree->expr_value =
            tree->lchild->expr_value * tree->lchild->rchild->rchild->expr_value;
        break;
      case '/':
        tree->expr_value =
            tree->lchild->expr_value / tree->lchild->rchild->rchild->expr_value;
        break;
      case '%':
        tree->expr_value =
            tree->lchild->expr_value % tree->lchild->rchild->rchild->expr_value;
        break;
      case '+':
        tree->expr_value =
            tree->lchild->expr_value + tree->lchild->rchild->rchild->expr_value;
        break;
      case '-':
        tree->expr_value =
            tree->lchild->expr_value - tree->lchild->rchild->rchild->expr_value;
        break;
      case '<':
        tree->expr_value = (tree->lchild->expr_value <
                            tree->lchild->rchild->rchild->expr_value)
                               ? 1
                               : 0;
        break;
      case '>':
        tree->expr_value = (tree->lchild->expr_value >
                            tree->lchild->rchild->rchild->expr_value)
                               ? 1
                               : 0;
        break;
      case T_LessEqual:
        tree->expr_value = (tree->lchild->expr_value <=
                            tree->lchild->rchild->rchild->expr_value)
                               ? 1
                               : 0;
        break;
      case T_GreaterEqual:
        tree->expr_value = (tree->lchild->expr_value >=
                            tree->lchild->rchild->rchild->expr_value)
                               ? 1
                               : 0;
        break;
      case T_Equal:
        tree->expr_value = (tree->lchild->expr_value ==
                            tree->lchild->rchild->rchild->expr_value)
                               ? 1
                               : 0;
        break;
      case T_NotEqual:
        tree->expr_value = (tree->lchild->expr_value !=
                            tree->lchild->rchild->rchild->expr_value)
                               ? 1
                               : 0;
        break;
      case T_And:
        tree->expr_value = (tree->lchild->expr_value &&
                            tree->lchild->rchild->rchild->expr_value)
                               ? 1
                               : 0;
        break;
      case T_Or:
        tree->expr_value = (tree->lchild->expr_value ||
                            tree->lchild->rchild->rchild->expr_value)
                               ? 1
                               : 0;
        break;

      default:
        break;
      }
    } else {
      tree->is_constant_expr = false;
    }
    tree->isVisited = true;
  
  
  // * ( Exp ) 形式
  } else if (tree->lchild->type == '(') {
    // '(' Exp ')'
    if (!tree->lchild->rchild->isVisited)
      CheckExprValue(tree->lchild->rchild);

    if (tree->lchild->rchild->is_constant_expr) {
      tree->expr_value = tree->lchild->rchild->expr_value;
      tree->is_constant_expr = true;
    } else {
      tree->is_constant_expr = false;
    }
    tree->isVisited = true;
  
  
  // * 'op' Exp 形式
  } else if (tree->lchild->rchild && tree->lchild->rchild->type == Exp) {
    // '-' Exp
    if (!tree->lchild->rchild->isVisited)
      CheckExprValue(tree->lchild->rchild);

    if (tree->lchild->rchild->is_constant_expr) {
      tree->is_constant_expr = true;

      switch (tree->lchild->type) {
      case '-':
        tree->expr_value = -(tree->lchild->rchild->expr_value);
        break;
      case '+':
        tree->expr_value = +(tree->lchild->rchild->expr_value);
        break;
      case '!':
        // ! 操作数必须为0或1
        assert(tree->lchild->rchild->expr_value == 0 ||
               tree->lchild->rchild->expr_value == 1);
        tree->expr_value = (tree->lchild->rchild->expr_value) ^ 1;
        break;

      default:
        break;
      }
    } else {
      tree->is_constant_expr = false;
    }
    tree->isVisited = true;
  
  
  // * 整型常量
  } else if (tree->lchild->type == T_IntConstant) {
    tree->isVisited = true;
    tree->is_constant_expr = true;
    tree->expr_value = tree->lchild->int_value;
  
  
  // * 字符常量 / 左值 / 函数调用
  } else {
    tree->isVisited = true;
    tree->is_constant_expr = false;
  }
  
  return;
}

void ScopeTrial(GrammarTree tree, Scope scope, ScopeStack *stack) {
  // TODO: traverse the tree and calculate semantic fault
  string PASS = "[Pass]";
  if (!tree)
    return;
  if (tree->type == Program) {
    ScopeStackPush(stack, scope);
  } else if (tree->type == CompUnit) {
    cout << "Check if main function exists: ";
    cout << "Check main's signature: ";
  } else if (tree->type == ConstExp) {
    cout << "Check if ConstExp evaluates to const: ";
  }
}