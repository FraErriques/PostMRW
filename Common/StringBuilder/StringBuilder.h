#ifndef STRINGBUILDER_H_INCLUDED
#define STRINGBUILDER_H_INCLUDED

#include <string>
#include <vector>


namespace Common
{

class StringBuilder {

private:
    const std::string::size_type ScratchSize;
    std::string main;
    std::string scratch;

public:
    StringBuilder(int forecastedSize) : ScratchSize(forecastedSize), main(""), scratch(""){};
    // metodo che aggiunge stringhe ulteriori, in coda ad una primaria.
    // restituisce una referenza all'intero apparato di classe, nel quale si possono consultare sia il buffer primario, che quello temporaneo.
    // il buffer primario non viene mai svuotato.
    // il buffer temporaneo accumula fino a raggiungimento della propria dimensione (i.e. ScratchSize) e quindi fa flush nel buffer primario. In tale circostanza
    // il buffer temporaneo viene svuotato.
    StringBuilder & append(const std::string & str);
    StringBuilder & append(int i); // overload to append the ASCII symbol, which has code i.
    StringBuilder & append(unsigned long long Ull); // overload to append an unsigned long long (i.e. 64bit unsigned).
    StringBuilder & append( long long Ull); // overload to append a SIGNED long long (i.e. 64bit SIGNED).

    // metodo che fornisce la rappresentazione stringa dello StringBuilder:
    // tale rappresentazione consiste nel portare nel buffer primario tutto cio' che e' in attesa
    // di accodamento (i.e. main.append(scratch) ).
    // A valle di tale copia, viene fatto lo svuotamento del buffer temporaneo. ?std::move?
    const std::string & str();// NB. la stringa restituita e' in sola lettura.

};// end class StringBuilder.


namespace StrManipul
{


//---------------utility functions, to be used with std::string. They are outside classes; they are just functions in a namespace.

int findFirstOccurenceOf( const std::string &tokenToBeFound, std::string original );
std::string * replaceAllOccurencesOf( const std::string &tokenToBeReplaced, std::string original, const std::string &replacementToken );// NB. original passed by value, to be preserved.
std::vector<std::string> * stringSplit( const std::string &tokenToSplitOn, std::string original, bool removeEmptyEntries=false );// NB. original passed by value, to be preserved.
std::string &trimLeft( std::string &original);
std::string &trimRight( std::string &original);
std::string &trimBoth( std::string &original);
bool isInvisibleString( const std::string &analyzed);
std::vector<std::string> * removeEmptyEntries( std::vector<std::string> * original);
// follow the xToString() and stringToX() functions:
std::string * doubleToString( const double &par);
std::string * longDoubleToString( const long double &par);
std::string * intToString( const int &par);
std::string * uLongToString( const unsigned long &par);
std::string * uLongLongToString( const unsigned long long &par);//long-long i.e. 8 byte integral type(may be signed or not).
std::string * signedLongLongToString( const long long &par);//long-long i.e. 8 byte integral type(Signed here).
std::string * boolToString( const bool &par);
std::string * charToString( const char par);// no byRef, but byVal, since it's one byte.
int stringToInt( const std::string &par);
unsigned long stringToUnsignedLong( const std::string &par);
unsigned long long stringToUnsignedLongLong( const std::string &par);
double stringToDouble( const std::string &par);
long double stringToLongDouble( const std::string &par);


} // END namespacesp StrManipul

} // END namespacesp Common

#endif // STRINGBUILDER_H_INCLUDED
