#include "StringBuilder.h"
#include <sstream>

namespace Common
{


    // this Ctor is all in the .h
    //    StringBuilder::StringBuilder(int forecastedSize) : ScratchSize(forecastedSize), main(""), scratch("")
    //    {// all of the Ctor's duties are in the initialization list.
    //    }// END Ctor.

    // metodo che aggiunge stringhe ulteriori, in coda ad una primaria.
    // restituisce una referenza all'intero apparato di classe, nel quale si possono consultare sia il buffer primario, che quello temporaneo.
    // il buffer primario non viene mai svuotato.
    // il buffer temporaneo accumula fino a raggiungimento della propria dimensione (i.e. ScratchSize) e quindi fa flush nel buffer primario. In tale circostanza
    // il buffer temporaneo viene svuotato.
    StringBuilder & StringBuilder::append(const std::string & str)
    {
        scratch.append(str);// aggiungere in coda al buffer temporaneo
        if (scratch.size() > ScratchSize) // se il buffer temporaneo ha saturato la propria capacita' ->
        {
            main.append(scratch);// -> allora il temporaneo viene flushato nel primario (i.e. main)
            scratch.resize(0); // quindi il temporaneo viene azzerato
        }// else continue appending to the temporary buffer.
        return *this;// return a reference to the whole StringBuilder class, which contains both the main and the scratch buffers, in form of std::string.
    }// end StringBuilder::append.


    // try append the ASCII symbol of the provided code.
    StringBuilder & StringBuilder::append(int i) // overload to append the ASCII symbol, which has code i.
    {
        char * tmp = new char[2];
        tmp[0] = (char)i;
        tmp[1] = (char)0;// termination code.
        scratch.append(tmp);// aggiungere in coda al buffer temporaneo
        if (scratch.size() > ScratchSize) // se il buffer temporaneo ha saturato la propria capacita' ->
        {
            main.append(scratch);// -> allora il temporaneo viene flushato nel primario (i.e. main)
            scratch.resize(0); // quindi il temporaneo viene azzerato
        }// else continue appending to the temporary buffer.
        delete[] tmp;
        return *this;// return a reference to the whole StringBuilder class, which contains both the main and the scratch buffers, in form of std::string.
    }// end StringBuilder::append.

// overload to append an unsigned long long (i.e. 64bit unsigned).
StringBuilder & StringBuilder::append(unsigned long long Ull)
{
    std::string * unsignedLongLong_str = StrManipul::uLongLongToString( Ull);
    this->append( *unsignedLongLong_str);
    delete unsignedLongLong_str;
    return *this;// return a reference to the whole StringBuilder class.
}// append(unsigned long long Ull)

StringBuilder & StringBuilder::append( long long Signed_ll) // overload to append a SIGNED long long (i.e. 64bit SIGNED).
{
    std::string * Signed_LongLong_str = StrManipul::signedLongLongToString( Signed_ll);
    this->append( *Signed_LongLong_str);
    delete Signed_LongLong_str;
    return *this;// return a reference to the whole StringBuilder class.
}// append Signed_LongLong

StringBuilder & StringBuilder::append( double dbl) // overload to append a double
{// append double
    std::string * double_str = StrManipul::doubleToString( dbl);
    this->append( *double_str);
    delete double_str;
    return *this;// return a reference to the whole StringBuilder class.
}// append double

StringBuilder & StringBuilder::append_bool( bool bl) // overload to append a bool
{// append bool
    std::string * bool_str = StrManipul::boolToString( bl);
    this->append( *bool_str);
    delete bool_str;
    return *this;// return a reference to the whole StringBuilder class.
}// append bool


    // metodo che fornisce la rattresentazione stringa dello StringBuilder:
    // tale rappresentazione consiste nel portare nel buffer primario tutto cio' che e' in attesa di accodamento (i.e. main.append(scratch)
    // a valle di tale copia, viene fatto lo svuotamento del buffer temporaneo.
    const std::string & StringBuilder::str() // NB. la stringa restituita e' in sola lettura.
    {
        if (scratch.size() > 0) // se c'e' qualcosa nel buffer temporaneo
        {
            main.append(scratch);// allora fai flush del buffer nel record principale (i.e. main)
            scratch.resize(0);// quindi svuota il buffer temporaneo, dopo aver trasferito il suo contenuto nel buffer primario.
        }
        return main;// restituisce il buffer primario( una reference &), costituito da una std::string.
    }// end StringBuilder::str method.




namespace StrManipul
{



    //---------------utility functions, to be used with std::string. They are outside classes; they are just functions in a namespace.


    int findFirstOccurenceOf( const std::string &tokenToBeFound, std::string original )
    {//NB. returns -1 on not found.
        int res = original.find( tokenToBeFound);
        // ready.
        return res;
    }// END replaceAllOccurencesOf.

    std::string * replaceAllOccurencesOf( const std::string &tokenToBeReplaced, std::string original, const std::string &replacementToken )// NB. original passed by value, to be preserved.
    {
        std::string * returnValue = new std::string( original);// NB. work on a copy.
        size_t where = 0;
        size_t howManyCharToBeReplaced = std::string(tokenToBeReplaced).size();// if a different value is specified, the token will not be replaced entirely xor more of it will be.
        // core
        do// core
        {
            where = returnValue->find( tokenToBeReplaced);
            if(-1==(int)where){break;}
            returnValue->replace( where, howManyCharToBeReplaced, replacementToken);
        }// END core
        while( -1 !=(int)where);
        // ready.
        return returnValue;// NB. the original string has not been modified.
    }// END replaceAllOccurencesOf.


    std::vector<std::string> * stringSplit( const std::string &tokenToSplitOn, std::string original, bool removeEmptyEntries )// NB. original passed by value, to be preserved.
    {
        size_t where = 0;
        const size_t whereWeWere = 0;
        if(0==tokenToSplitOn.size() ) {return nullptr;}
        // the return value is a vector, which contains as many strings, as there are in "original" between the occurences of "tokenToSplitOn".
        std::vector<std::string> * returnValue = new std::vector<std::string>();// caller has to delete.
        // core
        do// core
        {
            where = original.find( tokenToSplitOn);
            std::string tmp = original.substr( whereWeWere, where );
            if( true==removeEmptyEntries)
            {
                if( false==isInvisibleString( tmp) )
                {
                    returnValue->push_back( tmp );
                }// END if( false==isInvisibleString( tmp) ) : else : skip an emptyEntry.
            }// END if( true==removeEmptyEntries) : else : do not check for emptyEntries : just push them back along with the other entries.
            else// else : do not check for emptyEntries : just push them back along with the other entries.
            {// else : do not check for emptyEntries : just push them back along with the other entries.
                returnValue->push_back( tmp );
            }// END else : do not check for emptyEntries : just push them back along with the other entries.
            original.replace( whereWeWere, where+tokenToSplitOn.size(), "");// eliminate the token that has been detached: do this anyway( i.e. removeEmptyEntries true or false).
        }// END core
        while( -1 != (int)where);
        // ready.
        return returnValue;// caller has to delete.
    }// END stringSplit


    std::string &trimLeft( std::string &original)
    {
        size_t where = 0;
        const size_t howManyCharToBeReplaced = 1;// 1 blank at a time. Left only.
        int c=0;
        // core
        do// core
        {
            where = original.find( 32);
            if(std::string::npos==(int)where){break;}
            if(32!=original[c]){break;}// blanks at string-start(i.e. at [0]) are over. There might be blanks inside or at string-end, but we don't care here.
//original.replace( where, howManyCharToBeReplaced, "");
            original.erase(where,howManyCharToBeReplaced);
        }// END core
        while( std::string::npos != where);
        // ready.
        return original;// NB. it has been modified.
    }// end trimLeft


    std::string &trimRight( std::string &original)
    {
        size_t where = 0;
        const size_t howManyCharToBeReplaced = 1;// 1 blank at a time. Left only.
        // core
        do// core
        {
            where = original.rfind( 32);
            if(std::string::npos==(int)where){break;}
//original.replace( where, howManyCharToBeReplaced, "");
            original.erase(where,howManyCharToBeReplaced);
            if(32!=original[where-1]){break;}// blanks at end-of-string are over. There might be termination nulls after, but we don't care about.
        }// END core
        while( std::string::npos != where);
        // ready.
        return original;// NB. it has been modified.
    }// end trimRight

    std::string &trimBoth( std::string &original)
    {
        size_t where = 0;
        const size_t howManyCharToBeReplaced = 1;// 1 blank at a time. -------Left only-------
        int c=0;
        // std::string::npos  === -1
        // core
        do// core  ---LEFT---
        {
            where = original.find( 32);// i.e. char blank==' '
            if(std::string::npos==where){break;}//-1 means not-found
            if(32!=original[c]){break;}// blanks at string-start(i.e. at [0]) are over. There might be blanks inside or at string-end, but we don't care here.
            original.erase(where,howManyCharToBeReplaced);//shorten the string, of the current char.// evaluate which of the two lines: this one shortens the string.
        }// END core
        while( std::string::npos !=where);
        //
        do// core   ---RIGHT------
        {
            where = original.rfind( ' ');// i.e. 32
            if(std::string::npos==where){break;}
            original.erase(where,howManyCharToBeReplaced);//shorten the string, of the current char.// evaluate which of the two lines: this one shortens the string.
            if(32!=original[where-1]){break;}// blanks at end-of-string are over. There might be termination nulls after, but we don't care about.
        }// END core
        while( std::string::npos !=where);
        // ready.
        return original;// NB. it has been modified.
    }// trimBoth


    bool isInvisibleString( const std::string &analyzed)
    {
        bool res = true;// init assuming it true.
        for(size_t c=0; c<analyzed.size(); c++)
        {
            res &= (       32==analyzed[c]      // blank
                        || 7==analyzed[c]       // TAB
                        || '\r'==analyzed[c]    // various newline
                        || '\n'==analyzed[c]
                   );
            if( false==res){break;}
        }// end for each char, until variation.
        // ready.
        return res;
    }// END isInvisibleString


    std::vector<std::string> * removeEmptyEntries( std::vector<std::string> * original)
    {
        if( NULL==original || nullptr==original) {return nullptr;}
        std::vector<std::string> * workCopy = new std::vector<std::string>();// create a new version. Caller has to delete.
        for( std::vector<std::string>::const_iterator originalPtr=original->begin(); originalPtr!=original->end(); originalPtr++)
        {
            if( false==isInvisibleString( *originalPtr))
            {
                workCopy->push_back( *originalPtr);
            }// else skip invisible entry.
        }// end for each entry in the collection.
        // ready
        return workCopy;// Caller has to delete.
    }// end removeEmptyEntries



    std::string * doubleToString( const double &par, bool isScientific)
    {
        std::string * res = new std::string();
        std::stringstream localSstream;
        localSstream.width( 18);// 2 for sign and decimal point
        if( isScientific)
        {
            localSstream.precision(16);
            localSstream.setf( std::ios_base::scientific);
        }
        else
        {
            localSstream.precision(4);
            localSstream.setf( std::ios_base::fixed);
        }
        localSstream << par;//put the double into the stringStream
        *res = localSstream.str();// get the string from the stringStream
        // N.B. the caller has to delete the return value.
        return res;
    }//doubleToString

std::string * longDoubleToString( const long double &par)
    {
        std::string * res = new std::string();
        std::stringstream localSstream;
        localSstream.width( 30);// 2 for sign and decimal point
        localSstream.precision(30);
        localSstream.setf( std::ios_base::scientific);
        localSstream << par;//put the long double into the stringStream
        *res = localSstream.str();// get the string from the stringStream
        // N.B. the caller has to delete the return value.
        return res;
    }//longDoubleToString


    std::string * intToString( const int &par)
    {
        std::string * res = new std::string();
        std::stringstream localSstream;
        localSstream << par;//put the int into the stringStream
        *res = localSstream.str();// get the string from the stringStream
        // N.B. the caller has to delete the return value.
        return res;
    }

    std::string * uLongToString( const unsigned long &par)
    {
        std::string * res = new std::string();
        std::stringstream localSstream;
        localSstream << par;//put the unsigned long into the stringStream
        *res = localSstream.str();// get the string from the stringStream
        // N.B. the caller has to delete the return value.
        return res;
    }

    std::string * uLongLongToString( const unsigned long long &par)
    {//long-long i.e. 8 byte integral type(may be signed or not).
        std::string * res = new std::string();
        std::stringstream localSstream;
        localSstream << par;//put the unsigned long-long into the stringStream
        *res = localSstream.str();// get the string from the stringStream
        // N.B. the caller has to delete the return value.
        return res;
    }

    std::string * signedLongLongToString( const long long &par)
    {// signed long long
        std::string * res = new std::string();
        std::stringstream localSstream;
        localSstream << par;//put the signed long long into the stringStream
        *res = localSstream.str();// get the string from the stringStream
        // N.B. the caller has to delete the return value.
        return res;
    }// signed long long

    std::string * boolToString( const bool &par)
    {
        std::string * res = new std::string();
        std::stringstream localSstream;
        // set literal representation :  "true" xor "false".
        localSstream << std::boolalpha << par;//put the bool into the stringStream
        *res = localSstream.str();// get the string from the stringStream
        // N.B. the caller has to delete the return value.
        return res;
    }

    std::string * charToString( const char par)
    {
        std::string * res = new std::string();
        std::stringstream localSstream;
        localSstream << par;//put the char into the stringStream
        *res = localSstream.str();// get the string from the stringStream
        // N.B. the caller has to delete the return value.
        return res;
    }


// parameter not byRef, but byVal, since it's a temporary to be converted in a string-pointer, for logging purposes.
std::string * stringByValueToStringPtr( const std::string par)
{
    std::string * res = new std::string( par);// put the parameter in the heap.
    // N.B. the caller has to delete the return value.
    return res;// the logging method will delete, after usage.
}// stringByValueToStringPtr


    int stringToInt( const std::string &par)
    {
        int res;
        std::stringstream( par) >> res;
        return res;
    }

    unsigned long stringToUnsignedLong( const std::string &par)
    {// recover an unsigned long from a string.
        unsigned long res;
        std::stringstream( par) >> res;
        return res;
    }// recover an unsigned long from a string.

    unsigned long long stringToUnsignedLongLong( const std::string &par)
    {// recover an unsigned long long from a string.
        unsigned long long res;
        std::stringstream( par) >> res;
        return res;
    }// recover an unsigned long long from a string.

    double stringToDouble( const std::string &par)
    {
        double res;
        std::stringstream( par) >> res;
        return res;
    }// stringToDouble

    long double stringToLongDouble( const std::string &par)
    {
        long double res;
        std::stringstream( par) >> res;
        return res;
    }// stringToLongDouble


}// end nmsp StrManipul

}// end nmsp Common
