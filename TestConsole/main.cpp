#include <iostream>
//--------------------#include <boost/lambda/lambda.hpp>
//#include "../Common/DbConnectionService/dbCall.h"
//#include "../Common/DbConnectionService/mysql_connection.h"
//#include "../Common/DbConnectionService/DbConnectionService.h"
#include "../Common/StringBuilder/StringBuilder.h"
#include "../Common/Stream/stream_io_.h"
#include "../Process/MonteCarlo_wrap/MonteCarlo_wrap.h"
#include "../Common/MonteCarlo/BaseConverter.h"
#include "../Common/MonteCarlo/ClassicalDiscreteGenerator.h"
#include "../Common/MonteCarlo/ClassicalContinuousGenerator.h"
#include "../Common/Stream/stream_io_.h"
#include "../Entity/Integration/Integration.h"
#include "../Entity/Integration/Integrate.h"
#include "../Entity/PrimesFinder/Primes.h"
#include "../Entity/Complex/Complex.h"

/// l'integrando (-z)^s/(Exp[z]-1)dz/z
Numerics::Complex * IcoChain( Numerics::Complex s, Numerics::Complex z)
{
    Numerics::Complex numerator(-z);
    numerator ^=s;
    Numerics::Complex denominator((z.ExpC()-1.0)*z);
    Numerics::Complex * res = new Numerics::Complex( numerator / denominator);
    return res;
}// IcoChain


Numerics::Complex * pointFromOriginAnulus( double delta, double Theta )
{// this parametrization is: (delta*Cos[t] + I*delta*Sin[t])
    // take the desired point, on the unit ball: i.e. the versor.
    Numerics::Complex * res = new Numerics::Complex( cos(Theta) , sin(Theta) );//NB. caller has to delete.
    *res *= delta;// then take it on the appropriate ball, by omothetia.
    return res;
}// pointFromOriginAnulus


Numerics::Complex * originAnulus( double stepSize, int stepOrdinal, double delta)
{// this parametrization is: (delta*Cos[t] + I*delta*Sin[t])
    Domain domain;//to be thrown
    int stepCardinality = +2.0*PI/stepSize;
    if(stepOrdinal>stepCardinality){throw domain;}
    double Theta = +2.0*PI*(double)stepOrdinal/(double)stepCardinality;
    Numerics::Complex * res = new Numerics::Complex( cos(Theta) , sin(Theta) );//NB. caller has to delete.
    *res *= delta;
    return res;
}// originAnulus

Numerics::Complex * integralStepIntoOriginAnulus( double stepSize, double radius, Numerics::Complex s)
{// this parametrization is: (radius*Cos[t] + I*radius*Sin[t])
    Numerics::Complex * accumulator = new Numerics::Complex( 0.0 , 0.0);
    int stepCardinality = +2.0*PI/stepSize;
    double Theta =0.0;
    for(int curStep=0; curStep<stepCardinality; curStep++)
    {
        Theta = +2.0*PI*(double)curStep/(double)stepCardinality;
        Numerics::Complex * pointOnCircularChain = pointFromOriginAnulus( radius, Theta );
        Numerics::Complex * pointOn_COchain = IcoChain( s, *pointOnCircularChain);
        *accumulator += *pointOn_COchain;
        delete pointOnCircularChain;
        delete pointOn_COchain;
    }
    return accumulator;// caller has to delete.
}// originAnulus

int main()
{
    Numerics::Complex z(+3.1 , 5.2);
    for( int n=0; n<6; n++)
    {
        Numerics::Complex  curExponent((double)n, 0.0);
        Numerics::Complex res = z^curExponent;
        std::cout<<"\n\t("<<z.Re()<<"+I*"<<z.Im()<<")^"<<n<<" == "<< res.Re()<<"+I*"<<res.Im();
    }




//	std::string thePath("./out20220219_.txt");
//	std::fstream theStream;
//	//
//	bool result = Common::Stream::outstreamOpener( thePath , theStream );
//    double radius = +3.7123E-09;
//    double stepSize = +2.0*PI/100.0;
//    Numerics::Complex s(+3.5, +20.15);
//    Numerics::Complex * integralOnOriginAnulus = integralStepIntoOriginAnulus( stepSize, radius, s);
//    std::string * curField = nullptr;
//    Common::StringBuilder sb(90);
//    //
//    curField = Common::StrManipul::doubleToString( integralOnOriginAnulus->Re() );
//    sb.append( *curField );
//    sb.append((int)'\t');
//    delete curField;
//    //
//    curField = Common::StrManipul::doubleToString( integralOnOriginAnulus->Im() );
//    sb.append( *curField );
//    sb.append((int)'\t');
//    delete curField;
//    //
//    //-----finally dump the line
//    Common::Stream::putline( sb.str() , theStream);
//    result = Common::Stream::outstreamCloser( theStream );
//    delete integralOnOriginAnulus;


//    for(int c=0; c<100; c++)
//    {
//        Numerics::Complex * res = originAnulus( stepSize, c, delta );
//        std::cout<<"originAnulus(stepSize,c,delta)=="<<stepSize<<c<<delta<<" == "<< res->Re()<<" +I* "<< res->Im()<<" length=="<< res->length() <<std::endl;
//        Common::StringBuilder sb(90);
//        std::string * curField = nullptr;
//        //
//        curField = Common::StrManipul::doubleToString( stepSize);
//        sb.append( *curField );
//        sb.append((int)'\t');
//        delete curField;
//        //
//        curField = Common::StrManipul::intToString( c);
//        sb.append( *curField );
//        sb.append((int)'\t');
//        delete curField;
//        //
//        curField = Common::StrManipul::doubleToString( delta);
//        sb.append( *curField );
//        sb.append((int)'\t');
//        delete curField;
//        //
//        curField = Common::StrManipul::doubleToString( res->Re());
//        sb.append( *curField );
//        sb.append((int)'\t');
//        delete curField;
//        //
//        curField = Common::StrManipul::doubleToString( res->Im());
//        sb.append( *curField );
//        sb.append((int)'\t');
//        delete curField;
//        //
//        curField = Common::StrManipul::doubleToString( res->length());
//        sb.append( *curField );
//        sb.append((int)'\t');
//        delete curField;
//        //-----finally dump the line
//        Common::Stream::putline( sb.str() , theStream);
//        delete res;
//    }
//	result = Common::Stream::outstreamCloser( theStream );

//    Numerics::Complex s(+0.5 , +13.7);
//    Numerics::Complex *z = new Numerics::Complex(1, 3);
//    Numerics::Complex icoChain( IcoChain( s, *z) );
//    delete z;

    //
    std::cout<<"\n\n\n\t Strike Enter to leave\t";
    getchar();
    return 0;
}// main



/* --------------cantina----------------------------

    //PrimesFinder::Primes * p = new PrimesFinder::Primes(100);
//    long desPrime = (*p)[p->lastOrdinal];
//    int bisectionConvergenceSteps = p->Bisection( 73, 100, false);
////for(int c=0; c<=100;c++)
////    {
////        std::cout << "\n\t converging in " << p->Bisection( c, 100, false) <<" steps."<<std::endl;
////    }
//    //int NsectionConvergenceSteps = p->NpartSection( 73, 100, false);
//    for(int c=73; c<=100;c++)
//    {
//        std::cout<<" steps for Bisection to converge to LandingPoint_"<<c<<" are:"<< p->Bisection( c, 100, false)<<std::endl;
//        std::cout<<" steps for NpartSection to converge to LandingPoint_"<<c<<" are:"<< p->NpartSection( c, 100, false)<<std::endl;
//    }
    //delete p;


//    p = new PrimesFinder::Primes(5000000); // re-use
//    p->Start_PrimeDump_FileSys();
//    lastOrdinal = p->getLastOrdinal();
//    lastPrime = p->getLastPrime();
//    presentLength = p->getActualLength();
//
//     unsigned long res = (*p)[25];// TODO bug [lastOrdinal] #######################################################
//     res = (*p)[1];
//     res = (*p)[2];
//     res = (*p)[3];
//     res = (*p)[lastOrdinal];
//     res = (*p)[lastOrdinal-1];
//     res = (*p)[lastOrdinal-2];
//     res = (*p)[lastOrdinal-3];
//     res = (*p)[lastOrdinal-100];
//     if(overflowFlag==res)
//     {
//         std::cout<<"\n\t invalid index fed to operator[] "<<std::endl;
//     }
//     for( int c=1; c<=15; c++)
//     {
//         std::cout<<"\n\t Prime["<<c<<"]=="<<(*p)[c];
//     }
//     for( int c=lastOrdinal-500; c<=lastOrdinal-50; c++)
//     {
//         std::cout<<"\n\t Prime["<<c<<"]=="<<(*p)[c];
//     }
//     const char * theDumpPath = p->getPrimeDumpFullPath( "primeDefaultFile");// Default Section Name.
//     if( nullptr != theDumpPath)
//      {}// else : TODO not-healthly built.
//     const char * straightContentOfDumpTail  = p->dumpTailReader( theDumpPath);
////PrimesFinder::Primes::DumpElement * dumpTail = p->recoverDumpTail( straightContentOfDumpTail);
//     PrimesFinder::Primes::SingleFactor * factorization = p->IntegerDecomposition( (*p)[30123]*7);

////
////     delete[] theDumpPath;
////     delete[] straightContentOfDumpTail;
//     delete[] factorization;
//    //delete[] dumpTail;
//     delete p;

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
