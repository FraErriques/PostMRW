#ifndef SINGLETON_H_INCLUDED
#define SINGLETON_H_INCLUDED

#include <iostream>
#include <thread>         // std::thread
#include <mutex>          // std::mutex
#include <sstream>
#include <vector>

std::mutex Sng_instance_mtx;    // mutex for critical section has to be compilation-unit global.
std::mutex Sng_unsubscribe_mtx;    // mutex for critical section has to be compilation-unit global.
//    Singleton_mtx.lock();// critical section (exclusive access to std::cout signaled by locking mtx):
//    do something in between : std::cout << "thread #" << id << '\n';
//    Singleton_mtx.unlock();// end of critical section.

namespace Common
{

    template<typename T>
    class TSingleton
    {
        private:
            static  int reference_counter;
            static  T * handle;
            TSingleton();//  Ctor
            static void TryMakeDestruction();// a private tool to annihilate the instance, when no more clients are active.

        public:
            ~TSingleton();// Dtor
            static  T * instance();
            static  void unsubscribe_one_();
            static  void unsubscribe_all_();
            static  int ClientCardinality();
    };// end  class TSingleton<T>




//template<typename T>    NB. cosi' si inizializza un dato statico in una template, altrimenti ld->undefined reference !!
//int myTemplate<T>::someValue = 100; le init a membri statici di template devono essere globali all'unita' di
// compilazione, i.e. fuori dalla classe.

     template<typename T>
      T * Common::TSingleton<T>::handle = nullptr;// NB. ! mandatory init !

     template<typename T>
      int Common::TSingleton<T>::reference_counter = 0;// NB. ! mandatory init !



// NB. le implementazioni dei metodi di template vanno tutte nel .h , altrimenti il linker richiede un'istanza
// fittizia per non fare unresolved external.

    template <typename T>
    TSingleton<T>::TSingleton()
    {
    }// end private Ctor

    template <typename T>
    TSingleton<T>::~TSingleton()
    {
        std::cout<<" \n\n\t Singleton::Destructor\n\n";
        TSingleton<T>::unsubscribe_all_();
        TSingleton<T>::TryMakeDestruction(); // try kill.
    }// end Dtor


    template <typename T>
     T * TSingleton<T>::instance()
    {
        Sng_instance_mtx.lock();// critical section (exclusive access to std::cout signaled by locking mtx):
        {// start critical section
            try
            {
                if (NULL == TSingleton<T>::handle)
                {
                    TSingleton<T>::handle = new T();// default(i.e. no params) Ctor is required fo T typename.
                }// else:  Singleton<content> gia' istanziata
                // in entrambi i casi iscrivo il nuovo client
                TSingleton<T>::reference_counter++;// one client more.
            }
            catch (...)// default C++ exception.
            {
                TSingleton<T>::handle = NULL;// without incrementing reference counter
            }
        }// end critical section
        Sng_instance_mtx.unlock();// end of critical section.
        return TSingleton<T>::handle;
    }// unique public access point



        /// <summary>
        /// an utility, to notify a client less, to the server.
        /// </summary>
        template <typename T>
         void TSingleton<T>::unsubscribe_one_()
        {
            Sng_unsubscribe_mtx.lock();// critical section (exclusive access to std::cout signaled by locking mtx):
            {// start critical section
                --TSingleton<T>::reference_counter;// one client less :NB. that's the only difference with unsubscribe_all_
                TSingleton<T>::TryMakeDestruction(); // try kill.
            }// end critical section
            Sng_unsubscribe_mtx.unlock();// end of critical section.
        }// end unsubscribe_one_


        template <typename T>
        void TSingleton<T>::TryMakeDestruction()
        {// NB. it's not under critical section, since it's in the caller(s).
                if (0 == TSingleton<T>::reference_counter)// try make destruction:
                {// kill
                    if (NULL != TSingleton<T>::handle)// case of no more clients, but instance still to be destroied.
                    {
                        delete TSingleton<T>::handle;
                        TSingleton<T>::handle = NULL;
                    }// else it's already a NULL handle.
                }// END try make destruction: else there are still other active clients-> leave the instance on.
        }// END



        /// <summary>
        /// an utility, to make destruction, completely.
        /// </summary>
        template <typename T>
         void TSingleton<T>::unsubscribe_all_()
        {
            Sng_unsubscribe_mtx.lock();// critical section (exclusive access to std::cout signaled by locking mtx):
            {// start critical section
                TSingleton<T>::reference_counter = 0;// unsubscribe_all_ ! NB. that's the only difference with unsubscribe_one_
                TSingleton<T>::TryMakeDestruction(); // try kill.
            }// end critical section
            Sng_unsubscribe_mtx.unlock();// end of critical section.
        }// end unsubscribe_all_


        template <typename T>
         int TSingleton<T>::ClientCardinality()
        {
            return TSingleton<T>::reference_counter;// publish #(client)
        }// end ClientCardinality

}// end nmsp Common



/***********************************************************************************************
*
*
*
*
*/
/////////// a good prototype of the class( or struct to let unique, by the Singleton).


   void threadAction( int par)
   {
       // TODO sostituire il corpo con EntryPointThreadSpawnTraceOnly, sostituendo le chiamate a Process:SinkFsTrace
       // con Singleton<Common::SinkFs>.Trace()
   }

struct HandleProto
{
     int *  field;
public:

   // MemberFuncPtr _memberFuncPtr;
     double (HandleProto::*_memberFuncPtr)(double) ; // <- declare by saying what class it is a pointer to
     void (HandleProto::*_threadActionsPtr)(int) ; // <- declare by saying what class it is a pointer to
    // NB. instantiation technique for such member functions pointers is:

    HandleProto( )  // default Ctor
    {
        this->field = new int;
        *(this->field) = 0;// default Ctor
        //
        this->_threadActionsPtr = &HandleProto::threadActions;// init method ptr.
    }
    ~HandleProto( )  // Dtor
    {
        if(nullptr!=(this->field))
        {
            delete (this->field);
        }// else already NULL.
    }
    HandleProto( int par)
    {
        this->field = new int;
        *(this->field) = par;
    }

     void writeSthg(  int parNumb)
    {
        std::cout<<" scrittura con par=="<<parNumb<<std::endl;
    }

     void threadActions( int id )
    {
        std::cout<<"\n threadActions : parameter==  "<<id<<" row 1"<<std::endl;
        std::cout<<"\n threadActions : parameter==  "<<id<<" row 2"<<std::endl;
        std::cout<<"\n threadActions : parameter==  "<<id<<" row 3"<<std::endl;
    }// end threadActions


    /*
std::mutex open_phase;
std::mutex trace_phase_1_;
std::mutex trace_phase_2_;
std::mutex trace_phase_3_;
std::mutex close_phase;

void EntryPointThreadSpawn( int par)
{
    open_phase.lock();
    {
        std::stringstream thread_number_open;
        thread_number_open << par;
        //
        std::vector<std::string> opener;
        opener.push_back("EntryPointThreadSpawn");
        opener.push_back(" thread_number= ");
        opener.push_back(thread_number_open.str());
        Process_LogWrappers::SectionOpen_multiMessage( opener, 0);
        opener.clear();
    }
    open_phase.unlock();
    //
    trace_phase_1_.lock();
    {
        std::stringstream thread_number_t1;
        thread_number_t1 << par;
        //
        std::vector<std::string> contentTracer_1;
        contentTracer_1.push_back("Nel mezzo del cammin di nostra vita");
        contentTracer_1.push_back(" thread_number= ");
        contentTracer_1.push_back(thread_number_t1.str());
        Process_LogWrappers::SectionContent_multiMessage( contentTracer_1 , 0);
        contentTracer_1.clear();
    }
    trace_phase_1_.unlock();
    //
    trace_phase_2_.lock();
    {
        std::stringstream thread_number_t2;
        thread_number_t2 << par;
        //
        std::vector<std::string>  contentTracer_2;
        contentTracer_2.push_back("Mi ritrovai per una selva oscura");
        contentTracer_2.push_back(" thread_number= ");
        contentTracer_2.push_back(thread_number_t2.str());
        Process_LogWrappers::SectionContent_multiMessage( contentTracer_2 , 0);
        contentTracer_2.clear();
    }
    trace_phase_2_.unlock();
    //
    trace_phase_3_.lock();
    {
        std::stringstream thread_number__t3;
        thread_number__t3 << par;
        //
        std::vector<std::string>  contentTracer_3;
        contentTracer_3.push_back("che la diritta via era smarrita");
        contentTracer_3.push_back(" thread_number= ");
        contentTracer_3.push_back(thread_number__t3.str());
        Process_LogWrappers::SectionContent_multiMessage( contentTracer_3 , 0);
        contentTracer_3.clear();
    }
    trace_phase_3_.unlock();
    //
    close_phase.lock();
    {
        Process_LogWrappers::SectionClose();
    }
    close_phase.unlock();
    // ready.
}// END EntryPointThreadSpawn


void EntryPointThreadSpawn_TraceOnly_( int par)
{
    //
    trace_phase_1_.lock();
    {
        std::stringstream thread_number_t1;
        thread_number_t1 << par;
        //
        std::vector<std::string> contentTracer_1;
        contentTracer_1.push_back("Nel mezzo del cammin di nostra vita");
        contentTracer_1.push_back(" thread_number= ");
        contentTracer_1.push_back(thread_number_t1.str());
        Process_LogWrappers::SectionContent_multiMessage( contentTracer_1 , 0);
        contentTracer_1.clear();
    }
    trace_phase_1_.unlock();
    //
    trace_phase_2_.lock();
    {
        std::stringstream thread_number_t2;
        thread_number_t2 << par;
        //
        std::vector<std::string>  contentTracer_2;
        contentTracer_2.push_back("Mi ritrovai per una selva oscura");
        contentTracer_2.push_back(" thread_number= ");
        contentTracer_2.push_back(thread_number_t2.str());
        Process_LogWrappers::SectionContent_multiMessage( contentTracer_2 , 0);
        contentTracer_2.clear();
    }
    trace_phase_2_.unlock();
    //
    trace_phase_3_.lock();
    {
        std::stringstream thread_number__t3;
        thread_number__t3 << par;
        //
        std::vector<std::string>  contentTracer_3;
        contentTracer_3.push_back("che la diritta via era smarrita");
        contentTracer_3.push_back(" thread_number= ");
        contentTracer_3.push_back(thread_number__t3.str());
        Process_LogWrappers::SectionContent_multiMessage( contentTracer_3 , 0);
        contentTracer_3.clear();
    }
    trace_phase_3_.unlock();
    // ready.
}// END EntryPointThreadSpawn_TraceOnly_
*/



    int threadForker( int nThreads)
    {
        //int nThreads = 3;
        std::thread threads[nThreads];
        // spawn nThreads threads:
        for (int i=0; i<nThreads; ++i)
        {
            threads[i] = std::thread( threadAction, i+1);
        }
        //
        for (auto& th : threads)
        {
            th.join();
        }
        return 0;// TODO
    }// END threadForker

    /*****
     *********** function pointer to member function : instantiation :
     *******
        (stkp.*(stkp._threadActionsPtr)) (7);  // stkp is an automatic instance of a class
        (foo->*(foo->_memberFuncPtr))(3.1415);//  foo is a dynamic (pointer) instance to a class
        (myUniqueObject->*(myUniqueObject->_threadActionsPtr))( par );//  myUniqueObject is a dynamic (pointer) instance to a class
      NB. tutte le parentesi utilizzate sono indispensabili !! L'omissione di ciascuna di esse porta ad
      errori di compilazione difficili da capire !
    */

};



#endif // SINGLETON_H_INCLUDED
