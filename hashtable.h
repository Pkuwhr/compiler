/*
 * @Author: lfq
 * @Date: 2020-05-28 10:26:05
 * @LastEditTime: 2020-05-28 19:30:46
 * @Description: Declaration of symbol table
 * @FilePath: \compiler\hashtable.h
 */ 

#include<map>
#include <string.h>
using namespace std;

/**
 * This is Function Class which will be used to create the hashtable
 * The comparation rule
 */
struct ltstr
{
    bool operator()(const char *s1, const char* s2) const
    {return strcmp(s1,s2)<0;}
};

template <class Value> class Iterator;

/**
 * this is the symbol table 
 * which use multimap STL to implement Hashtable.
 * member function and Iterator will be implemented by myself.
 */

template <class Value> class Hashtable{

    private:
        multimap<const char*, Value, ltstr> mmap;
    public:
        Hashtable() {}
        int NumEntries() const; //return pairs number
        void Enter(const char *key, Value value, 
                bool overwriteInsteadOfShadow = true); //insert a new pairs 
        void Remove(const char *key, Value value);//remove a pairs
        Value Lookup(const char *key);//return the latest value or NULL
        Iterator<Value> GetIterator();
        Hashtable<Value>* MakeCopy();//Copy the symbol table
};

/**
 * Hashtable Iterator
 * */

template <class Value> 
class Iterator {

  friend class Hashtable<Value>;

  private:
    typename multimap<const char*, Value , ltstr>::iterator cur, end;
    Iterator(multimap<const char*, Value, ltstr>& t)
	: cur(t.begin()), end(t.end()) {}
	 
  public:
         // Returns current value and advances iterator to next.
         // Returns NULL when there are no more values in table
         // Visits every value, even those that are shadowed.
    Value GetNextValue();
};


#include "hashtable.cc" // icky, but allows implicit template instantiation
