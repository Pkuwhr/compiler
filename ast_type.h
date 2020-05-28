/*
 * @Author: lfq
 * @Date: 2020-05-28 15:50:17
 * @LastEditTime: 2020-05-28 21:47:47
 * @LastEditors: Please set LastEditors
 * @Description: Type define
 * @FilePath: \Rigel\ast_type.h
 */ 

 
#ifndef _H_ast_type
#define _H_ast_type

#include "ast.h"
#include "list.h"
#include <iostream>
#include "hashtable.h"
using namespace std;

class Decl;

/**
 * copy from 13.
 * Some member functions is unnecessary，I will delet them later
 * */

class Type : public Node 
{
  protected:
    int typeKind; //1:int 2:const 3:void

  public :

    Type(yyltype loc) : Node(loc) {}
    Type(int tk);
    

    virtual const char* GetName() { 
      switch (typeKind)
      {
      case 1:
        return "int";
      case 2:
        return "const int";
      default:
        return "void";
      }  
    }
    virtual void PrintToStream(ostream& out) { 
      switch (typeKind)
      {
      case 1:
        out<<"int";
        break;
      case 2:
        out<<"const int";
        break;
      default:
        out<<"void";
        break;
      }  
    }
    friend ostream& operator<<(ostream& out, Type *t) { t->PrintToStream(out); return out; }
    

    static bool ValidityChecker(Hashtable<Decl *>* symbolTable, Type* type);
    
};

class ArrayType : public Type 
{
  protected:
    Type *elemType;

  public:
    ArrayType(yyltype loc, Type *elemType);
    Type* BaseType() { return elemType; }
    void PrintToStream(ostream& out) { out << elemType << "[]"; }
    
    virtual const char* GetName() { return "Array"; }
};

 
#endif
