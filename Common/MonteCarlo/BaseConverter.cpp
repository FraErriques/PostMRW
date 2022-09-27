#include "BaseConverter.h"
#include "../StringBuilder/StringBuilder.h"
#include <iostream>
#include <map>
#include <cmath>



namespace Common
{

namespace MonteCarlo
{


BaseConverter::BaseConverter( bool wantShowDbg)
{
    /* ---- ASCII codes of the involved symbols: basically all the letters( upper & lower case) and figures(1,9), plus some symbols.
        [48,57]
        [65,90]
        [97,126]
    */
    Common::StringBuilder sbB4symbols( 66);// 64 should be enough.
    for( int c=48;c<=57;c++)// figures (1,9).
    {//NB. append(int) appende alla stringa il simbolo corrispondente al codice int.
        sbB4symbols.append(c);
    }
    for( int c=65;c<=90;c++)
    {// upper case letters [A,Z]
        sbB4symbols.append(c);
    }
    for( int c=97;c<=126;c++)
    {// lower case letters + some symbols [a,z]+some symbols.
        sbB4symbols.append(c);
    }
    // --- build data emmbers :  the string & the map.
    this->base64Figures = new std::string( sbB4symbols.str() );
    if( wantShowDbg)
    {
        std::cout<< *base64Figures<<std::endl;
    }// else skip.
    //
    this->base64Map = new std::map<char,int>();
    std::size_t stringLength = base64Figures->size();
    for( int c=0; c<(int)stringLength; c++)
    {// build the map.
        (*(this->base64Map))[(*(this->base64Figures))[c]]= c;// al carattere che occupa la posizione di indice "c" spetta il valore c.
    }// end for build map.
    if( wantShowDbg)
    {
        for( std::map<char,int>::const_iterator reader=base64Map->begin();
             reader != base64Map->end();
             reader++ )
             {
                 std::cout<< (*reader).first<<"  "<< (*reader).second<<std::endl;
             }
    }// if( wantShowDbg) else skip.
}// Ctor


BaseConverter::~BaseConverter() // Dtor
{
    delete this->base64Figures;
    delete this->base64Map;
} // Dtor


std::string * BaseConverter::baseOtherConverterTo( unsigned long decimalIntegerToBeConverted, unsigned short destinationBase )
{
    Common::StringBuilder sb( 100);// evaluate
    std::string * baseSuffix = Common::StrManipul::intToString( destinationBase);
    sb.append("b");//start with base prefix.
    sb.append(*baseSuffix);
    sb.append("_");//the underscore will be the prefix delimiter, since it's not involved in any base symbol set.
    int abacusLeftMostPosition = log((double)decimalIntegerToBeConverted)/log((double)destinationBase);
    double dividendo = (double)decimalIntegerToBeConverted;
    for( ;abacusLeftMostPosition>=0; abacusLeftMostPosition--)
    {
        double quotient = dividendo/ pow((double)destinationBase,(double)abacusLeftMostPosition);
        double resto = dividendo - ((int)quotient)*pow((double)destinationBase,(double)abacusLeftMostPosition);
        // update
        sb.append( (*(this->base64Figures))[(int)quotient] );
        dividendo = resto;
    }
    delete baseSuffix;
    //ready.
    std::string * res = new std::string( sb.str());
    return res;// caller has to delete.
}


unsigned long BaseConverter::base10ConverterTo( const std::string & baseOtherPar, unsigned short originBase )
{
    unsigned long decimalQuantity = 0;// init and sum on it.
    // DBG int inputLen = baseOtherPar.size();
    int endPrefixUnderscore = Common::StrManipul::findFirstOccurenceOf("_", baseOtherPar);
    std::string * cleanVersion = nullptr;
    if( -1 != endPrefixUnderscore)
    {
        cleanVersion = new std::string( baseOtherPar.substr(endPrefixUnderscore+1, baseOtherPar.size()-(endPrefixUnderscore+1) ));
    }
    else
    {// no prefix found -> take the whole string.
        cleanVersion = new std::string( 0, baseOtherPar.size() );
    }

    int abacusLeftMostPosition = cleanVersion->size()-1;// zero based
    for( int c=0 ;abacusLeftMostPosition>=0; abacusLeftMostPosition--,c++)
    {
        unsigned long figureQuantity = (*base64Map)[ (*cleanVersion)[c]];// value of the symbol.
        double abacusPositionQuantity = pow( (double)originBase , (double)abacusLeftMostPosition );
        figureQuantity *= abacusPositionQuantity;
        decimalQuantity += figureQuantity;
    }
    delete cleanVersion;
    //ready.
    return decimalQuantity;
}// base10ConverterTo



} // namespace MonteCarlo

} // namespace Common
