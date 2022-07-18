#include "TheNode.h"

    Common::Dictionary::PhoneBookRecord::PhoneBookRecord(
        std::string &name,
        std::string &email,
        std::string &internal,
        std::string &cellPhone
                    )
                    {
                        this->name = new std::string( name);
                        this->email = new std::string( email);
                        this->internal = new std::string( internal);
                        this->cellPhone = new std::string( cellPhone);
                    }// Ctor


    Common::Dictionary::PhoneBookRecord::~PhoneBookRecord()// Dtor
    {
        std::cout<<"\n\t Entering the Destructor \n";
        delete this->name;
        delete this->email;
        delete this->internal;
        delete this->cellPhone;
        this->name = nullptr;
        this->email = nullptr;
        this->internal = nullptr;
        this->cellPhone = nullptr;
    }// Dtor


    void Common::Dictionary::PhoneBookRecord::internalPrint()
    {
        if( nullptr!=this->name
        && nullptr!=this->email
        && nullptr!=this->internal
        && nullptr!=this->cellPhone
        )
        {
            std::cout<<"\n\n\tRecapiti:"
            <<"\n\t"<< *(this->name)
            <<"\n\t"<< *(this->email)
            <<"\n\t"<< *(this->internal)
            <<"\n\t"<< *(this->cellPhone)
            <<"\n\n";
        }
        else
        {
            std::cout<<"\n\t The node fields are empty \n";
        }
    }// internalPrint()
