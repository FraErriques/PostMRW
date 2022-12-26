
class Throwable
{
    public:
    std::string reasonForThrowing;
    bool canResume;
    //Ctor
    Throwable( std::string reason){this->reasonForThrowing=reason;}
};

void someFunc()
{
    try
    {
        int i = 0;
        ++i;
        Throwable theCurrentExc( std::string("that's the reason for unwinding the Stack."));
        throw theCurrentExc;
    }
    catch( Throwable &theException)
    {
        int c = -1;
        c++;
        //std::cout<<"\n\t from inside: catch( Throwable &theException) "<<"bool_canResume=="<<theException->canResume<<std::endl;
        std::cout<<"\n\t from inside: catch( Throwable &theException) "<<std::endl;//NB. cannot use ostream in a catch.
    }
    catch(...)
    {
        int c = -1;
        c++;
        std::cout<<"\n\t from inside: catch(...) "<<std::endl;
    }
}// someFunc()

