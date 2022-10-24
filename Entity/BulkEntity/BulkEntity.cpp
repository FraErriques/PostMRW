#include <iostream>
#include "BulkEntity.h"

namespace Entity {


    Bulk_Entity::Bulk_Entity()
    {
        this->containerCapacity = +55;// TODO let it parametrized
        this->vec = new int[containerCapacity];
        for(int c=0; c<containerCapacity; c++)
        {
            this->vec[c] = c;
        }
    }// Ctor

    Bulk_Entity::~Bulk_Entity()
    {
        delete[] this->vec;
    }// Dtor


    Bulk_Entity::myIterator Bulk_Entity::myBegin()
    {
        Bulk_Entity::myIterator theIterator;
        theIterator.first = (&vec[0])+0;
        theIterator.after_last = (&(vec[containerCapacity-1]))+1;// one after last
        theIterator.currentNode = theIterator.first;// init to first
        return theIterator;
    }

    Bulk_Entity::myIterator Bulk_Entity::myEnd()
    {
        Bulk_Entity::myIterator theIterator;
        theIterator.first = (&vec[0])+0;
        theIterator.after_last = (&(vec[containerCapacity-1]))+1;// one after last
        theIterator.currentNode = theIterator.after_last;// init to one_after_last
        return theIterator;
    }// end



/*  -------- for test purposes -------------------------
----
int main( int argc, char** argv )
{
    Bulk_Entity be;// automatic instance
    for( Bulk_Entity::myIterator it=be.myBegin(); it!=be.myEnd(); it++)
    {
        std::cout<<"\t pointee=="<< *it <<"\n";
    }// stl style

    //
    cout << "\n\n\t Strike \"Enter\" to leave " << endl;
    getchar();
    return 0;
}
*/


} // namespace Entity

