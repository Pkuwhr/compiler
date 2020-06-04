/*
 * @Author: lfq
 * @Date: 2020-05-28 15:50:17
 * @LastEditTime: 2020-05-29 17:01:07
 * @LastEditors: Please set LastEditors
 * @Description: Type define
 * @FilePath: \Rigel\ast_type.h
 */ 

 

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
    
    const int GetName() { 
      return typeKind;
    }

    /*
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
    friend ostream& operator<<(ostream& out, Type *t) { t->PrintToStream(out); return out; }*/
    

    static bool ValidityChecker(Hashtable<Decl *>* symbolTable, Type* type);
    
};


