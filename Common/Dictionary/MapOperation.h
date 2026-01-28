#ifndef mapOperation_H_INCLUDED
#define mapOperation_H_INCLUDED

#include <map>
#include <string>
#include <iostream>
#include "../StringBuilder/StringBuilder.h"
#include "TheNode.h"

namespace Common{
    namespace Dictionary{

class MapOperation
{
  private:
      std::map<std::string, PhoneBookRecord * > * dictionary;
  public:
    MapOperation();// Ctor
    ~MapOperation();// Dtor
    void readFileByLines(std::string &where);
    bool prune_RecordLayout( std::vector<std::string> * tokenizedLine);
    void mapTraverseForward();
    void mapTraverseReverse();
    void mapNodeDestructorCaller();
    void nodeFinder( std::string requiredkey);
    void mapListener();
};//class

    }}//2 nmsp
#endif // mapOperation_H_INCLUDED
