#include "ThreadForker.h"

//        ThreadForker( FuncPtr branchedThreadActionPar , unsigned howManyForkingThreadsPar )
//        :   branchedThreadAction(branchedThreadActionPar),
//            howManyForkingThreads(howManyForkingThreadsPar)
//        {
//        }

int Common::ThreadForker::theForkeingPoint( )
{
    std::thread threads[howManyForkingThreads];
    // spawn nThreads threads:
    for (unsigned i=0; i<howManyForkingThreads; ++i)
    {
        threads[i] = std::thread( branchedThreadAction, i+1);
    }
    //
    for (auto& th : threads)
    {
        th.join();
    }
    // ready.
    return (int)(this->howManyForkingThreads);
}// end int Common::ThreadForker::theForkeingPoint( )
