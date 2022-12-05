#include "ThreadSpawn.h"

int Common::ThreadSpawn::theSpawnPoint( )
{
    std::thread threads[howMany_Spawn_Threads];
    // spawn nThreads threads:
    for (unsigned i=0; i<howMany_Spawn_Threads; ++i)
    {
        threads[i] = std::thread( branchedThreadAction, i+1);
    }
    //
    for (auto& th : threads)
    {
        th.join();
    }
    // ready.
    return (int)(this->howMany_Spawn_Threads);
}// end int Common::ThreadSpawn::theSpawnPoint( )
