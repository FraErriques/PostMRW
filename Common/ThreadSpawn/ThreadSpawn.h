#ifndef THREAD_SPAWN_H_INCLUDED
#define THREAD_SPAWN_H_INCLUDED

#include <thread>         // std::thread
#include <mutex>          // std::mutex

namespace Common
{

    typedef void ( * FuncPtr ) (int);

    class ThreadSpawn
    {
    private:
        FuncPtr branchedThreadAction;
        //void (* branchedThreadAction) int;
        unsigned howMany_Spawn_Threads;

    public:
        ThreadSpawn( FuncPtr branchedThreadActionPar , unsigned howMany_Spawn_Threads_Par )
        :   branchedThreadAction(branchedThreadActionPar),
            howMany_Spawn_Threads(howMany_Spawn_Threads_Par)
        {
        }

        int theSpawnPoint( );

    };// end class ThreadSpawn

}// namespace Common

#endif // THREAD_SPAWN_H_INCLUDED
