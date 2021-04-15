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
            where = original.find( " ");
            if(-1==(int)where){break;}
            if(32!=original[c]){break;}// blanks on the left are over. There might be blanks on the right, but we don't care here.
            original.replace( where, howManyCharToBeReplaced, "");
        }// END core
        while( -1 !=(int)where);
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
            where = original.rfind( " ");
            if(-1==(int)where){break;}
            original.replace( where, howManyCharToBeReplaced, "");
            if(32!=original[where-1]){break;}// blanks on the left are over. There might be blanks on the right, but we don't care here.
        }// END core
        while( -1 !=(int)where);
        // ready.
        return original;// NB. it has been modified.
    }// end trimRight

    std::string &trimBoth( std::string &original)
    {
        std::string &res = trimRight( original);
        res = trimLeft( res);
        return res;
    }// end trimBoth


    bool isInvisibleString( const std::string &analyzed)
    {
        bool res = true;// intit assuming it true.
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



    std::string * doubleToString( const double &par)
    {
        std::string * res = new std::string();
        std::stringstream localSstream;
        localSstream.width( 18);// 2 for sign and decimal point
        localSstream.precision(16);
        localSstream.setf( std::ios_base::scientific);
        localSstream << par;//put the double into the stringStream
        *res = localSstream.str();// get the string from the stringStream
        // N.B. the caller has to delete the return value.
        return res;
    }

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


    int stringToInt( const std::string &par)
    {
        int res;
        std::stringstream( par) >> res;
        return res;
    }

    int stringToUnsignedLong( const std::string &par)
    {// recover an unsigned long from a string.
        unsigned long res;
        std::stringstream( par) >> res;
        return res;
    }// recover an unsigned long from a string.

    double stringToDouble( const std::string &par)
    {
        double res;
        std::stringstream( par) >> res;
        return res;
    }


}// end nmsp StrManipul

}// end nmsp Common
