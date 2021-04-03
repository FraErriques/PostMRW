#ifndef BASECONVERTER_H_INCLUDED
#define BASECONVERTER_H_INCLUDED


#include <map>
#include <string>


namespace Common
{

namespace MonteCarlo
{


class BaseConverter
{
private:
    std::string * base64Figures;
    std::map<char,int> * base64Map;

public:
BaseConverter( bool wantShowDbg);// Ctor
~BaseConverter();                // Dtor
std::string * baseOtherConverterTo( unsigned long decimalIntegerToBeConverted, unsigned short destinationBase );
unsigned long base10ConverterTo( const std::string & baseOtherPar, unsigned short originBase );

};// class BaseConverter

} // namespace MonteCarlo

} // namespace Common

#endif // BASECONVERTER_H_INCLUDED
