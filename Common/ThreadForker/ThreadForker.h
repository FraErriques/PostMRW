#ifndef THREADFORKER_H_INCLUDED
#define THREADFORKER_H_INCLUDED

#include <thread>         // std::thread
#include <mutex>          // std::mutex

namespace Common
{

    typedef void ( * FuncPtr ) (int);

    class ThreadForker
    {
    private:
        FuncPtr branchedThreadAction;
        //void (* branchedThreadAction) int;
        unsigned howManyForkingThreads;

    public:
        ThreadForker( FuncPtr branchedThreadActionPar , unsigned howManyForkingThreadsPar )
        :   branchedThreadAction(branchedThreadActionPar),
            howManyForkingThreads(howManyForkingThreadsPar)
        {
        }

        int theForkingPoint( );

    };// end class ThreadForker

}// namespace Common

#endif // THREADFORKER_H_INCLUDED
