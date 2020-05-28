/*
 * @Author: lfq
 * @Date: 2020-05-28 10:16:09
 * @LastEditTime: 2020-05-28 14:41:47
 * @Description: Declear some classes of ast
 * @FilePath: \Rigel\ast.h
 */ 

#include <stdlib.h>   // for NULL
#include "location.h"
#include <iostream>
#include "list.h"
#include "hashtable.h"

using namespace std;

class Decl;

class Node  {
  protected:
    yyltype *location;
    Node *parent;

  public:
    Node(yyltype loc);
    Node();
    virtual ~Node() {}
    
    yyltype *GetLocation()   { return location; }
    void SetParent(Node *p)  { parent = p; }
    Node *GetParent()        { return parent; }
};
   

class Identifier : public Node 
{
  protected:
    char *name;
    
  public:
    Identifier(yyltype loc, const char *name);
    friend ostream& operator<<(ostream& out, Identifier *id) { return out << id->name; }
    const char* GetName() { return name; }
};