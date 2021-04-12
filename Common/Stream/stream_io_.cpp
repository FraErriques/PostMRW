#include <vector>
#include "stream_io_.h"
#include "../StringBuilder/StringBuilder.h"

namespace Common
{

namespace Stream
{

bool instreamOpener( std::string &fullPath, std::fstream &theInStream)
{
	bool result = false;
	//std::ios_base::openmode x = 1;// read mode.

	try
	{
		theInStream.open( fullPath.c_str() , std::ios::in );
		if( theInStream.is_open() )
		{
			result = true;
			// do something
		}
		else
		{
			result = false;
		}
	}
	catch(...)
	{
	}
	//finally
	//{
	//}
	// return
	return result;
}


bool instreamCloser( std::fstream &theInStream)
{
	bool result = false;
	//
	try
	{
		if( theInStream.is_open() )
		{
			theInStream.close();
			result = true;
		}
		else
		{
			result = false;
		}
	}
	catch(...)
	{
		result = false;
	}
	//finally
	//{
	//}
	// return
	return result;
}


bool instream_getToken(  std::string &fullPath,  std::fstream &theInStream)
{
    bool result = instreamOpener( fullPath , theInStream );
    std::vector<std::string> * tokenArray = new std::vector<std::string>;
    //
    for( int c=0; !theInStream.eof();  )
    {
        std::string buf;// tmp for current token.
        theInStream >> buf;// cosi' va per WORD separate da blank o TAB
        std::cout << buf << std::endl;
        tokenArray->push_back( buf );
        ++c;
    }
    int c=0;
    for(std::vector<std::string>::iterator curToken=tokenArray->begin(); curToken!=tokenArray->end(); curToken++)
    {
        std::cout << "Token # " << c+1 << " = " << *curToken << std::endl;
        c++;
    }
    result =instreamCloser( theInStream );
    // final usage of bool_result
    result = (c>0);
    delete tokenArray;
    return result;
}// end  instream_getToken()


bool outstreamOpener( std::string &fullPath, std::fstream &theOutStream)
{
	bool result = false;
	//std::ios_base::openmode x = 2;// write mode. std::ios::out

	try
	{
		theOutStream.open( fullPath.c_str() , std::ios::out | std::ios::app);//----NB. create if does not exist or append if exists.
		if( theOutStream.is_open() )
		{
			result = true;
			// do something
		}
		else
		{
			result = false;
		}
	}
	catch(...)
	{
	    result = false;
	}
	//finally
	//{
	//}
	// return
	return result;
}


bool outstreamCloser( std::fstream &theOutStream)
{
	bool result = false;
	//
	try
	{
		if( theOutStream.is_open() )
		{
			theOutStream.flush();
			theOutStream.close();
			result = true;
		}
		else
		{
			result = false;
		}
	}
	catch(...)
	{
	}
	//finally
	//{
	//}
	// return
	return result;
}


bool putline( const std::string &theLine,  std::fstream &theOutStream )
{
	bool result = false;
	//
	try
	{
		if( theOutStream.is_open() )
		{
			theOutStream << theLine.c_str() << std::endl;
			theOutStream.flush();
			result = true;
		}
		else
		{
			result = false;
		}
	}
	catch(...)
	{
	}
	//finally
	//{
	//}
	// return
	return result;
}


    // an utility.
    void showASCII()
    {
        for( int c=32; c<127; c++)
        {
            std::cout<< " ASCII code " << c
            << " corresponds to symbol : "
            << (char)c
            << std::endl;
        }// for
    }// showASCII()



int stream_io_exercise( )
{
	std::string thePath("./out20171230_.txt");
	std::fstream theStream;
	//
	bool result = outstreamOpener( thePath , theStream );
	if( ! result)
    {
        return +1;
    }// else continue.
    //
	std::string s("Nel mezzo del cammin di nostra vita, mi ritrovai per una selva oscura, che la diritta via era smarrita...");
	for( int c=0; c<10; c++)
	{
		putline( s, theStream);
	}
	result = outstreamCloser( theStream );

	result = instreamOpener( thePath , theStream );
	const int stringBufLen = 144;
	char * buf = new char[stringBufLen];
	for( int c=0; !theStream.eof();  )
	{
		// theStream >> buf;// cosi' va per WORD separate da blank o TAB
		 theStream.getline( buf, stringBufLen);
		 std::cout <<"Line # "<< ++c<<" = "<< buf << std::endl;
	}
	result =instreamCloser( theStream );

	//
	//std::string s("Hallo");
	//double d = 3.1415;
	//std::cout << s.c_str() << std::endl;// N.B. convert to const char*,using c_str() method.
	//
	std::cout << "\r\tStrike Enter to leave\t" << std::endl;
	int key = getchar( );
	putchar(key);
	return 0;
}

}// nmsp Stream
}// nmsp Common
