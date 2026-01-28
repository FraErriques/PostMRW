#include "TheNode.h"
#include "../StringBuilder/StringBuilder.h"

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
        // DBG std::cout<<"\n\t Entering the Destructor \n";
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
        std::string emptyField("");
        std::cout<<"\n--------------Recapiti:";
        //--------------------------
        if( nullptr!=this->name
             && emptyField.compare( Common::StrManipul::trimBoth( *(this->name) ))
            )
        {
            std::cout<<"\n\t Name: "<< *(this->name);
        }
        else
        {
            std::cout<<"\n\t Field Name is missing";
        }
        //--------------------------
        if( nullptr!=this->email
             && ""!= Common::StrManipul::trimBoth( *(this->email) )
            )
        {
            std::cout<<"\n\t Email: "<< *(this->email);
        }
        else
        {
            std::cout<<"\n\t Field Email is missing";
        }
        //--------------------------
        if( nullptr!=this->internal
             && ""!= Common::StrManipul::trimBoth( *(this->internal) )
            )
        {
            std::cout<<"\n\t Internal-Phone: "<< *(this->internal);
        }
        else
        {
            std::cout<<"\n\t Field Internal-Phone is missing";
        }
        //--------------------------
        if( nullptr!=this->cellPhone
             && ""!= Common::StrManipul::trimBoth( *(this->cellPhone) )
           )
        {
            std::cout<<"\n\t Cellular-Phone: "<< *(this->cellPhone);
        }
        else
        {
            std::cout<<"\n\t Field Cellular-Phone is missing";
        }
        std::cout<<"\n----------------------------------------------\n";
    }// internalPrint()
