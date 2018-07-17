#ifndef STREAM_IO__H_INCLUDED
#define STREAM_IO__H_INCLUDED

#include <iostream>
#include <fstream>

namespace Common
{

namespace Stream
{


bool instreamOpener( std::string &fullPath, std::fstream &theInStream);
bool instreamCloser( std::fstream &theInStream);
bool outstreamOpener( std::string &fullPath, std::fstream &theOutStream);
bool outstreamCloser( std::fstream &theOutStream);
bool putline( const std::string &theLine,  std::fstream &theOutStream );
bool instream_getToken(  std::string &fullPath,  std::fstream &theInStream);
// an evergreen utility:
void showASCII();
//
int stream_io_exercise(); // a test caller().

}// nmsp Stream
}// nmsp Common


#endif // STREAM_IO__H_INCLUDED
