﻿# include "Primes.h"
#include "../../Common/Config_wrap/Config_wrap.h"



        PrimesFinder::Primes::Primes()
        {
        }// Ctor

        const char * PrimesFinder::Primes::getDefaultPrimeDumpFullPath( const std::string & sectionNameInFile)
        {
            const char *  res = nullptr;
            Common::ConfigurationService * primeNamedConfig = new Common::ConfigurationService( "./PrimeConfig.txt");
            const std::string * desiderSectionContent = primeNamedConfig->getValue( sectionNameInFile);
            res = desiderSectionContent->c_str();
            return res;
            //ofstream createOrAppend()
        }// createOrAppend

        void PrimesFinder::Primes::createOrAppend( const std::string & fullPath)
        {
            ofstream createOrApp(fullPath, std::fstream::out | std::fstream::app);
            createOrApp.close();
        }//ofstream getDefaultPrimeDumpFullPath()

        const char *  PrimesFinder::Primes::lastRecordReader( const std::string & fullPath)
        {
            ifstream lastrecReader(fullPath, std::fstream::in );
            std::string * buf = new std::string();
            char curChar;
            lastrecReader.seekg( 0, lastrecReader.end);
            int streamSize = lastrecReader.tellg();
            bool ifstreamStatus;
            int readChars = 0;
            for(  lastrecReader.seekg( -1, lastrecReader.end);//########## NB. to do not get EOF on first read, it's necessary to seek(-1,end).
                  streamSize>=readChars;
               )
            {
                ifstreamStatus = lastrecReader.good();
                lastrecReader.get( curChar);
                readChars++;
                ifstreamStatus = lastrecReader.good();
                lastrecReader.seekg( -2, lastrecReader.cur);//####### NB. each stream.get() seeks(+1,cur) so to read backwards I need seek(-2,cur).
                ifstreamStatus = lastrecReader.good();
                buf->append( 1, curChar);// append one time, character curChar.
                if( !ifstreamStatus)
                {
                    break;
                }
            }
            lastrecReader.close();//######### NB. close the stream #################
            const char * revertedBuf =  buf->c_str();
            int revertedLength = buf->length();
            char * straightBuf = new char[revertedLength];
            for( int c=buf->length()-1; c>=0; c-- )
            {
                straightBuf[(buf->length()-1)-c] = revertedBuf[c];
            }
            return straightBuf;
        }// lastRecordReader
