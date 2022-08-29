#ifndef mapNode_H_INCLUDED
#define mapNode_H_INCLUDED

#include <string>
#include <iostream>

namespace Common{
    namespace Dictionary{

class PhoneBookRecord
{
  private:
    std::string * name;
    std::string * email;
    std::string * internal;
    std::string * cellPhone;
    //
    // methods
  public:
    PhoneBookRecord(){}
    PhoneBookRecord(
        std::string &name,
        std::string &email,
        std::string &internal,
        std::string &cellPhone
                    );// Ctor
    ~PhoneBookRecord();// Dtor
    void internalPrint();
}; //  PhoneBookRecord

    }// nmsp
}// nmsp

#endif // mapNode_H_INCLUDED
