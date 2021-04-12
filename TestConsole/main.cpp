#include <iostream>
//#include <boost/lambda/lambda.hpp>
#include "../Common/DbConnectionService/dbCall.h"
#include "../Common/StringBuilder/StringBuilder.h"
#include "../Common/Stream/stream_io_.h"
#include "../Common/Stream/RWtxtfile.h"
#include "../Process/MonteCarlo_wrap/MonteCarlo_wrap.h"
#include "../Common/MonteCarlo/BaseConverter.h"
#include "../Common/MonteCarlo/ClassicalDiscreteGenerator.h"
#include "../Common/MonteCarlo/ClassicalContinuousGenerator.h"
#include "../Entity/Integration/Integration.h"
#include "../Entity/Integration/Integrate.h"



double * VectorFieldImage( double *ApplicationPoint, int hm)
{// NB. remember to specify of which scalar field, the present vector field is Nabla.
    if(nullptr==ApplicationPoint)
    {
        return nullptr;
    }// else continue
    double * FreeBound = new double[2];
    double Ximage = -2.0 * ApplicationPoint[0];// Nabla of scalar field f(x,y)==-x^2-y^2+1.0
    FreeBound[0] = ApplicationPoint[0] + Ximage;// remember to add FreeBound to the respective ApplicationPoint so to obtain the affine image vector.
    double Yimage = -2.0 * ApplicationPoint[1];
    FreeBound[1] = ApplicationPoint[1] + Yimage;
    return FreeBound;
}


void LoggerSinkFS_example( unsigned long inf, unsigned long sup)
{
    Common::LogWrappers::SectionOpen("TestConsole::LoggerSinkFS_example()", 0);
    bool isStillPrime = true;
    double realQuotient;
    unsigned long intQuotient;
    std::string * realQuotientStr = nullptr;
    std::string * intQuotientStr = nullptr;
    for( unsigned long cursor=inf; cursor<=sup; cursor++)
    {
        double soglia = sqrt( cursor);// division is a two-operand operator: the bisection of dividend is Sqrt[dividend]
        // when dividend/Sqrt[dividend]==Sqrt[dividend] and when dividend/(Sqrt[dividend]+eps)<Sqrt[dividend]
        // so the stepping into divisor>Sqrt[dividend] leads to divisors<Sqrt[dividend] which have already been explored.
        unsigned long divisor=+2;
        for( ; divisor<=soglia; divisor++)
        {
            realQuotient = (double)cursor/(double)divisor;
            intQuotient = cursor/divisor;
            realQuotientStr = Common::StrManipul::doubleToString(realQuotient);
            intQuotientStr = Common::StrManipul::uLongToString(intQuotient);
            if( realQuotient-intQuotient <+1.0E-80 )
            {// divisione diofantea
                Common::StringBuilder strBuild( 200);// on the stack
                strBuild.append("divisione diofantea:");
                strBuild.append( *Common::StrManipul::uLongToString( cursor) );
                strBuild.append("/");
                strBuild.append( *Common::StrManipul::uLongToString( divisor) );
                std::string logBuf = strBuild.str();
                const char* buf = logBuf.c_str();
                Common::LogWrappers::SectionContent( buf, 0);
                isStillPrime = false;// NB. #################
                break;// NB. #################
            }
            else
            {// continue searching for primality

                Common::StringBuilder strBuild( 200);// on the stack
                strBuild.append("searching-primality:");
                strBuild.append( *Common::StrManipul::uLongToString( cursor) );
                strBuild.append("/");
                strBuild.append( *Common::StrManipul::uLongToString( divisor) );
                strBuild.append("=");
                strBuild.append( *realQuotientStr );
                std::string logBuf = strBuild.str();
                const char* buf = logBuf.c_str();
                Common::LogWrappers::SectionContent( buf, 0);
            }// else : // continue searching for primality
            Common::StringBuilder strBuild( 200);// on the stack
            strBuild.append("fine interno-divisori:");
            strBuild.append( *Common::StrManipul::uLongToString( cursor) );
            strBuild.append("/");
            strBuild.append( *Common::StrManipul::uLongToString( divisor) );
            std::string logBuf = strBuild.str();
            const char* buf = logBuf.c_str();
            Common::LogWrappers::SectionContent( buf, 0);
            delete realQuotientStr;// a new step allocates new memory, for each of those pointers.
            delete intQuotientStr;// a new step allocates new memory, for each of those pointers.
        }// the internal for : the one from [+2, cursor]
        Common::StringBuilder strBuild( 200);// on the stack
        strBuild.append("fine esterno-dividendi:");
        strBuild.append( *Common::StrManipul::uLongToString( cursor) );
        strBuild.append("/");
        strBuild.append( *Common::StrManipul::uLongToString( divisor) );
        if(isStillPrime)
        {
            strBuild.append("  ## Primo individuato ## : ");
            strBuild.append( *Common::StrManipul::uLongToString( cursor) );
            strBuild.append("  ###");
        }// else ripristino.
        else
        {// ripristino della primalita', dopo un composto(i.e. non primo).
            isStillPrime = true;
        }// ripristino della primalita', dopo un composto(i.e. non primo).
        std::string logBuf = strBuild.str();
        const char* buf = logBuf.c_str();
        Common::LogWrappers::SectionContent( buf, 0);
    }// external for : the one where cursor cicles from inf to sup, on dividends.
    // ready.
    Common::LogWrappers::SectionClose();
}// LoggerSinkFS_example()



int main()
{
    //RWtxtfile_demo_();
    //LoggerSinkFS_example( +2, +100);

    fstream bidirStream( "./log/cpp_LogStream_Fri Apr 09 2021_.log", ios::in | ios::out);
    if(bidirStream)
    {

        int length = bidirStream.tellp();
          // allocate memory for file content
        bidirStream.seekp( length/2, bidirStream.end);
        std::string buf( "some text, IN THE MIDDLE__________________");
        bidirStream.write( buf.c_str(), buf.length() );
        //
        bidirStream.seekp( 0, bidirStream.end);
        buf = "some text, at the end.";
        bidirStream.write( buf.c_str(), buf.length() );
        //
        bidirStream.close();
    }

  std::ifstream inputStream ("./log/cpp_LogStream_Fri Apr 09 2021_.log", std::ifstream::binary);
  if (inputStream)
  {
    // get length of file:
    inputStream.seekg( 0, inputStream.end);
    int length = inputStream.tellg();
    inputStream.seekg (0, inputStream.beg);
    // allocate memory:
    char * buffer = new char [length];
    // read data as a block:
    inputStream.read (buffer,length);
    inputStream.close();
    //
    // print content:
    std::cout.write (buffer,length);
    delete[] buffer;
    //
    //
  }// test seek.


//    std::fstream testPrimeTokenStream;
//    std::string fullPath("./log/testPrimeTokenStream_.txt");
//    bool isOpened = Common::Stream::outstreamOpener( fullPath, testPrimeTokenStream);
//    if( isOpened)
//    {
//        testPrimeTokenStream.seekg( off64_t, ios_base);
//        Common::Stream::putline(
//            "ordinalExample_PrimeExample"
//            ,testPrimeTokenStream
//                                );
//    }// else skip.
//    //anyway close:
//    bool isAppropriatelyClosed = Common::Stream::outstreamCloser( testPrimeTokenStream);



    //
    std::cout<<"\n\t Strike Enter to leave\t";
    getchar();
    return 0;
}



/* --------------cantina----------------------------

// VectorField example
//    double * ApplicationPoint = new double[2];
//    double * FreeBound = nullptr;
//    for( double x=0.0; x<+1.0; x+=+0.3)
//    {
//        for( double y=0.0; y<+1.0; y+=+0.3)
//        {
//            ApplicationPoint[0] = x;
//            ApplicationPoint[1] = y;
//            FreeBound = VectorFieldImage( ApplicationPoint, 2);
//            bool res = writeVectorFieldR2Affine( ApplicationPoint, FreeBound, 2);
//        }//y
//    }//x
//    delete[] ApplicationPoint;
//    delete[] FreeBound;


   Numerics::Integrate * intgVoghera = new Numerics::Integrate( asdrubale, 2,4,2, -9.8E+99 );
   double vogheraTrapezi = intgVoghera->equi_trapezium( );
   delete intgVoghera;
   double trapeziCorrettivoControrno = Entity::Integration::trapezi( 2,4,2, asdrubale );
   double errore = vogheraTrapezi - trapeziCorrettivoControrno;

 // MonteCarlo
    double left  = -92.809;
    double right = +12.209;

    int populationCardinality = 9000;
    //
    Common::MonteCarlo::ClassicalDiscreteGenerator * monteCGen =
    new Common::MonteCarlo::ClassicalDiscreteGenerator();// Ctor
    monteCGen->resetExtractionInterval( left, right);// NB. compulsory
    for( int c=0; c<populationCardinality;c++)
    {
        monteCGen->nextIntegerInInterval();
    }
    //monteCGen->showDiscretePopulation();
    monteCGen->buildOmega( left, right );
    monteCGen->buildDiscreteFrequencyDistribution();
    std::cout<<"\n\n\t -------- START discrete model;"<<std::endl;
    monteCGen->showFrequencyDistribution();
    monteCGen->showCumulatedFrequency();
    delete monteCGen;

    std::cout<<"\n\t -------- END discrete model; START continuous model \n"<<std::endl;

    Common::MonteCarlo::ClassicalContinuousGenerator * monteCGen_D =
    new Common::MonteCarlo::ClassicalContinuousGenerator();// Ctor
    monteCGen_D->resetExtractionInterval( left, right);// NB. compulsory
    for( int c=0; c<populationCardinality;c++)
    {
        monteCGen_D->nextDoubleInInterval();
    }
    //monteCGen_D->showContinuousPopulation();
    monteCGen_D->buildOmega( left, right );
    monteCGen_D->buildContinuousFrequencyDistribution();
    monteCGen_D->showFrequencyDistribution();
    monteCGen_D->showCumulatedFrequency();

    delete monteCGen_D;
 // MonteCarlo


  std::string s("Nel mezzo#@# del cammin#@# di nostra vita,#@# mi ritrovai#@#  #@# #@# #@# per una selva oscura#@#.");
  std::vector<std::string> * tokens = Common::StrManipul::stringSplit("#@#", s, false);
  std::vector<std::string> * cleanTokens = Common::StrManipul::removeEmptyEntries( tokens);

  if( NULL != tokens)
  {
  for( std::vector<std::string>::const_iterator readerTk = tokens->begin();
        readerTk != tokens->end();
        readerTk++
     )
    {
        std::cout<< *readerTk<<std::endl;
    }
  }// else skip on NULL.
  //
  if( NULL != cleanTokens)
  {
  for( std::vector<std::string>::const_iterator readerCleanTk = cleanTokens->begin();
        readerCleanTk != cleanTokens->end();
        readerCleanTk++
     )
    {
        std::cout<< *readerCleanTk<<std::endl;
    }
  }// else skip on NULL.
  //
  delete tokens;
  delete cleanTokens;


    Common::MonteCarlo::BaseConverter b(false);
    const unsigned long threshold = +1.5E6;

    for(unsigned long c=0; c<threshold; c++)
    {
        for( int base=2; base<=64; base++)
        {
            std::string * converted = b.baseOtherConverterTo( c, base);
            unsigned long reConverted = b.base10ConverterTo( *converted, base);
            if( reConverted != c)
            {
                std::cout<<"\n\t Allarme ! c:"<<c<<" base:"<<base<<std::endl;
            }
            else
            {
//                std::cout<<"\n\t OK ! converted: "<<*converted
//                <<" reConverted:"<<reConverted
//                <<" original:"<<c
//                <<" base:"<<base<<std::endl;
            }
            delete converted;
        }
    }

void boost_test()
{
    using namespace boost::lambda;
    typedef std::istream_iterator<int> in;

    std::for_each(
                    in(std::cin)
                    , in()
                    , std::cout << (_1 * 3) << " "
                  );
}// end boost_test



double f(double x)
{
    //return exp(x)*cos(x);
    return x*x;
}


//
//    std::string * charToString( const char par)
//    {
//        std::string * res = new std::string();
//        std::stringstream localSstream;
//        localSstream << par;//put the char into the stringStream
//        *res = localSstream.str();// get the string from the stringStream
//        // N.B. the caller has to delete the return value.
//        return res;
//    }
//


//typedef struct _GUID {
//    unsigned long  Data1;
//    unsigned short Data2;
//    unsigned short Data3;
//    unsigned char  Data4[8];
//} GUID;
//std::string * getGUID( int randInitializer)
//{
//    Common::MonteCarlo * mCarlo = new Common::MonteCarlo( randInitializer);// user suggested seed.
//    Common::StringBuilder * sb = new Common::StringBuilder(17) ;
//    for( int c=0; c<16; c++)
//    {
//        int tmp = -1;
//        while( 33>tmp || 126<tmp)
//        {
//            tmp = mCarlo->nextProbabilityMeasure() * 127;// selected code
//        }
//        sb->append( tmp);
//    }
//    delete mCarlo;
//    std::string * res = new std::string( sb->str() );// caller has to delete.
//    delete sb;
//    return res;
//}// getGUID()





namespace Common{
namespace MonteCarloNamespace{
namespace Development{

// DeltaOmega is the DeltaAscissa in a Riemann partition of the Omega Event-space.
class DeltaOmega
{// an instance of DeltaOmega is a single step in the Riemann partition. The friend function "buildOmega" builds a vector, which contains the whole population of
 // DeltaOmega, which constitutes the whole Omega event space.
private:
    friend class MonteCarloGenerator;
    double mediana;
    double eta;
    double categoryFrequency;
public:
    DeltaOmega( double medianaPoint, double etaMeasure)
    {
        this->mediana = medianaPoint;
        this->eta = etaMeasure;
        this->categoryFrequency = 0.0;// init and then add on.
    }// Ctor
    double getMediana(){return this->mediana;}
    bool belongsToDeltaOmega( double candidate)
    {
        bool res = fabs( candidate-this->mediana)<this->eta;
        return res;
    }
};// class DeltaOmega


    class MonteCarloGenerator
    {
    private:
        friend class DeltaOmega;
        bool ConstructorCanContinue;
        std::string * ReasonForAbortingConstructor;
        double theIntervalMeasure;
        double Min, Sup;
        double omothetia;
        bool leftClosed, rightClosed;
        std::vector<double> * population;
        std::vector<DeltaOmega> * frequencyDistribution;

    public:
     MonteCarloGenerator( double left, bool isLeftIncluded
                         ,double right, bool isRightIncluded
                         ,bool wantContinuousModel ):ConstructorCanContinue(true)
                         ,ReasonForAbortingConstructor(new std::string("NOT YET.") )
                         ,theIntervalMeasure(right-left)
                         ,Min(left), Sup(right)
                         ,leftClosed( isLeftIncluded), rightClosed( isRightIncluded)
                         ,population( new std::vector<double>() )
                         ,frequencyDistribution(  new std::vector<DeltaOmega>() )
         {
             //theIntervalMeasure = right-left;// init for both models; correct only for discrete ones.
             // default model is [min,sup) on [0,RAND_MAX)==[0,32767)
             if( ! wantContinuousModel)//---------------------- discrete -------------------------------------------------
             {
                 if( fabs(left-(int)left)>+1E-80
                     || fabs(right-(int)right)>+1E-80 )
                 {
                     ConstructorCanContinue = false;
                     delete this->ReasonForAbortingConstructor;//free memory of default Ctor message.
                     this->ReasonForAbortingConstructor = new std::string("Invocation of a discrete model, with non Diophantine boundaries. Aborting.");
                 }
                 else // estremi interi
                 {// non default boundary conditions.
                     if( !isLeftIncluded)
                     {
                         left += +1.0;
                         theIntervalMeasure -= +1.0;// NB. such corrections only affect the discrete case, since they have zero measure on continuous models.
                     }// else the default is already appropriate
                     if( isRightIncluded)
                     {
                         right += +1.0;
                         theIntervalMeasure += +1.0;
                     }// else the default is already appropriate
                 }// estremi interi
             }////-------END--------------- discrete -------------------------------------------------
             this->omothetia = this->theIntervalMeasure/((double)RAND_MAX);
             //
         }// Ctor

        void nextIntegerInInterval() const
        {
            double temp =  rand()*this->omothetia+this->Min;
            this->population->push_back( (int)temp);
        }// END nextIntegerInInterval

        void nextDoubleInInterval() const
        {
            double temp = rand()*this->omothetia+this->Min;
            if( this->rightClosed)
            {
                while( temp>this->Sup)
                {
                    temp = rand()*this->omothetia+this->Min;
                }
            }
            this->population->push_back( temp);
        }// END nextIntegerInInterval

        void showPopulation() const
        {
            for( std::vector<double>::const_iterator it=this->population->begin();
                 it!=this->population->end();
                 it++
                )
                {
                    std::cout<< *it<<std::endl;
                }
        }// END showPopulation() const

        void showFrequencyDistribution() const
        {
            for( std::vector<DeltaOmega>::const_iterator it=this->frequencyDistribution->begin();
                 it!=this->frequencyDistribution->end();
                 it++
                )
                {

                        double mediana;
                        double eta;
                        double categoryFrequency;

                    std::cout<<" mediana "<< *Common::StrManipul::doubleToString(it->mediana)<<" eta "
                    << *Common::StrManipul::doubleToString(it->eta)
                    <<" categoryFrequency "<<*Common::StrManipul::doubleToString(it->categoryFrequency) <<std::endl;
                }
        }// END showFrequencyDistribution() const

        void showCumulatedFrequency() const
        {
           double CumulatedFrequency = 0.0;
           for( std::vector<DeltaOmega>::iterator frequencyWriter=this->frequencyDistribution->begin();
                frequencyWriter != this->frequencyDistribution->end();
                frequencyWriter++
              )
              {
                 CumulatedFrequency += frequencyWriter->categoryFrequency;
              }// for frequencyWriter
              std::cout<< "\n\n\t CumulatedFrequency : " << CumulatedFrequency <<std::endl;
        }// END showCumulatedFrequency() const


        void buildFrequencyDistribution()
        {
            int populationCardinality = this->population->size();
            double elementPresenceWeight = +1.0/populationCardinality;
            for( std::vector<double>::const_iterator populationReader=this->population->begin();
                 populationReader != this->population->end();
                 populationReader++
               )
               {
                   for( std::vector<DeltaOmega>::iterator frequencyWriter=this->frequencyDistribution->begin();
                        frequencyWriter != this->frequencyDistribution->end();
                        frequencyWriter++
                      )
                      {
                          if( frequencyWriter->belongsToDeltaOmega( *populationReader) )
                          {
                              frequencyWriter->categoryFrequency += elementPresenceWeight;
                              break;// belonging class found.
                          }
                      }// for frequencyWriter

               }// for populationReader
        }// buildFrequencyDistribution()



        void buildOmega( double partizioneLeft
                         ,double partizioneRight
                         ,double eta
                       )
        {//this->frequencyDistribution has been built by Ctor.
            for( double position=partizioneLeft-eta; position<=partizioneRight+eta; position+=2*eta)
            {
                DeltaOmega * curDeltaOmega = new DeltaOmega( position+eta, eta);// TODO verify
                this->frequencyDistribution->push_back( *curDeltaOmega );
            }//for
        }//buildOmega



    };// class MonteCarloGenerator


std::string * exadecimalConverterTo( unsigned int par )
{
    const std::string exaFigures("0123456789ABCDEF");
    Common::StringBuilder sb(20);
    sb.append("0x");//start with Hex prefix.
    int abacusLeftMostPosition = log(par)/log(16);
    double dividendo = (double)par;
    for( ;abacusLeftMostPosition>=0; abacusLeftMostPosition--)
    {
        double quotient = dividendo/ pow(+16.0,(double)abacusLeftMostPosition);
        double resto = dividendo - ((int)quotient)*pow(+16.0,(double)abacusLeftMostPosition);
        // update
        sb.append( exaFigures[(int)quotient]);
        dividendo = resto;
    }
    //ready.
    std::string * res = new std::string( sb.str());
    return res;// caller has to delete.
}

std::string * base64ConverterTo( unsigned int par )
{

        [48,57]
        [65,90]
        [97,126]

    Common::StringBuilder sbB4symbols( 66);
    for( int c=48;c<=57;c++)// figures
    {//NB. append(int) appende alla stringa il simbolo corrispondente al codice int.
        sbB4symbols.append(c);
    }
    for( int c=65;c<=90;c++)
    {// upper case letters
        sbB4symbols.append(c);
    }
    for( int c=97;c<=126;c++)
    {// lower case letters + some symbols
        sbB4symbols.append(c);
    }

    std::string base64Figures( sbB4symbols.str() );
    std::cout<< base64Figures<<std::endl;



    Common::StringBuilder sb( 100);
    sb.append("b64_");//start with base64 prefix.
    int abacusLeftMostPosition = log(par)/log(64);
    double dividendo = (double)par;
    for( ;abacusLeftMostPosition>=0; abacusLeftMostPosition--)
    {
        double quotient = dividendo/ pow(+64.0,(double)abacusLeftMostPosition);
        double resto = dividendo - ((int)quotient)*pow(+64.0,(double)abacusLeftMostPosition);
        // update
        sb.append( base64Figures[(int)quotient]);
        dividendo = resto;
    }
    //ready.
    std::string * res = new std::string( sb.str());
    return res;// caller has to delete.
}

unsigned long base10ConverterTo( const std::string & base64par )
{

        [48,57]
        [65,90]
        [97,126]

    Common::StringBuilder sbB4symbols( 66);
    for( int c=48;c<=57;c++)// figures
    {//NB. append(int) appende alla stringa il simbolo corrispondente al codice int.
        sbB4symbols.append(c);
    }
    for( int c=65;c<=90;c++)
    {// upper case letters
        sbB4symbols.append(c);
    }
    for( int c=97;c<=126;c++)
    {// lower case letters + some symbols
        sbB4symbols.append(c);
    }

    std::string base64Figures( sbB4symbols.str() );
    std::map<char,int> * base64Map = new std::map<char,int>();
    for( int c=0; c<base64Figures.size(); c++)
    {
        (*base64Map)[base64Figures[c]]= c;
    }

    for( std::map<char,int>::const_iterator reader=base64Map->begin();
         reader != base64Map->end();
         reader++ )
         {
             std::cout<< (*reader).first<<"  "<< (*reader).second<<std::endl;
         }

    unsigned long decimalQuantity = 0;// init and sum on it.
    std::string * cleanVersion = Common::StrManipul::replaceAllOccurencesOf("b64_",base64par,"");
    int abacusLeftMostPosition = cleanVersion->size()-1;// zero based
    for( int c=0 ;abacusLeftMostPosition>=0; abacusLeftMostPosition--,c++)
    {
        unsigned long figureQuantity = (*base64Map)[ (*cleanVersion)[c]];// value of the symbol.
        double abacusPositionQuantity = pow( +64.0 , (double)abacusLeftMostPosition );
        figureQuantity *= abacusPositionQuantity;
        decimalQuantity += figureQuantity;
    }
    delete cleanVersion;
    //ready.
    return decimalQuantity;
}

}// Development
}// MonteCarloNamespace
}// Common


    std::string * converted =
    Common::MonteCarloNamespace::Development::base64ConverterTo( 4294967295);// max(unsigned long)
    std::cout<<"dec 64 == B64 "<< *converted<<std::endl;

    unsigned long backFromB64 =// NB. avoid backSlashes due to escape sequences.
     Common::MonteCarloNamespace::Development::base10ConverterTo( *converted);
    delete converted;

    converted =
    Common::MonteCarloNamespace::Development::base64ConverterTo( backFromB64);
    std::cout<<"dec 64 == B64 "<< *converted<<std::endl;

    converted =
     Common::MonteCarloNamespace::Development::exadecimalConverterTo( 9898798);
    std::cout<<"dec 9898798 == B16 "<< *converted<<std::endl;
    delete converted;

    std::cout<<"sizeof(unsigned long) : "<<sizeof(unsigned long)<<std::endl;
    std::cout<<"sizeof(unsigned short) : "<<sizeof(unsigned short)<<std::endl;
    std::cout<<"sizeof(unsigned char) : "<<sizeof(unsigned char)<<std::endl;
//    unsigned long  Data1;  4 bytes == 32bit
//    unsigned short Data2;  2 == 16bit
//    unsigned short Data3;  2
//    unsigned char  Data4[8]; 1x8 == 64bit

//    int input =0;
//    for(;;)
//    {
//        std::cout << "Enter decimal number: " ;
//        std::cin >> input ;
//        if( 0 > input) {break;}
//        std::cout << "0x" << std::hex << input << '\n' ;
//    }



    double left  = +2.0;
    double right = +5.0;
    int populationCardinality = 4;
    double eta = +0.5;
    //
    Common::MonteCarloNamespace::Development::MonteCarloGenerator * monteCGen =
    new Common::MonteCarloNamespace::Development::MonteCarloGenerator(
                        left,true
                        ,right,true
                        ,true // is_continuous
                         );
    for( int c=0; c<populationCardinality;c++)
    {
        //monteCGen->nextIntegerInInterval();
        monteCGen->nextDoubleInInterval();
    }
    monteCGen->showPopulation();
    monteCGen->buildOmega( left, right, eta);
    monteCGen->buildFrequencyDistribution();
    monteCGen->showFrequencyDistribution();
    monteCGen->showCumulatedFrequency();

    delete monteCGen;

    //Common::Stream::showASCII();

//    for( int c=32; c<127; c++)
//    {
//        std::string * strASCIIcode = Common::StrManipul::intToString( c);
//        std::string * strASCIIsymb = Common::StrManipul::charToString( c);
//        //
//        std::cout<< " ASCII code "+ *strASCIIcode
//        + " corresponds to symbol : "
//        + *strASCIIsymb
//        << std::endl;
//        //
//        delete strASCIIcode;// free at each step, the allocated memory.
//        delete strASCIIsymb;
//    }// for


//    Common::dbCall::cantiere_numericTable_INSERT_SINGLE( -5.67005 );
//    Common::dbCall::cantiere_sede_INSERT_SINGLE();
//    Common::dbCall::cantiere_sede_LOAD_MULTI_();


//    boost_test();

    // Common::command_PREPARE(); tested : hangs with noexcept.

//    Entity::Integration::rettangoli( +100.0,
//                                     +120.0,
//                                     28,
//                                     f
//                                     , false // i.e. not called from inside "trapezi"
//                                     );
//
//    Entity::Integration::trapezi( +100.0,
//                                     +120.0,
//                                     28,
//                                     f
//                                     );

    // sqlParameters();
    //preparedStatement();
//    Process::testCpp11_MonteCarlo();



//    Common::MonteCarlo * mcInitializer = new Common::MonteCarlo();// time based seed.
//    int min = +2;
//    int sup = +4;
//    std::string * minStr = Common::StrManipul::intToString( min);
//    std::string * maxStr = Common::StrManipul::intToString( sup-1);
//    for( int c=0; c<20; c++)
//    {
//        std::string * tmp = Common::StrManipul::intToString(mcInitializer->nextIntegerInInterval( min, sup));
//        std::cout<< "int in ["+ *minStr +", "+ *maxStr +"] : " + *tmp <<std::endl;
//        delete tmp;
//    }
//    delete minStr;
//    delete maxStr;
//    delete mcInitializer;

*/
