
#include <iostream>

//using namespace std;

namespace Entity {

class Bulk_Entity
{

public:
    class myIterator
    {// class Iterator-------------------------internal------------------------
    public:
        int *currentNode;
        int *first;
        int *after_last;


        int *operator++()
        {
            // dbg int stepSize = sizeof(int);
            // TODO manage the case of incorrectness of current position.
            this->currentNode += 1;// which means 1*stepSize;
            return this->currentNode;
        }// operator++

        int *operator++( int par)
        {
            // dbg int stepSize = sizeof(int);
            // TODO manage the case of incorrectness of current position.
            this->currentNode += 1;// which means 1*stepSize;
            return this->currentNode;
        }// operator++

        int operator*()
        {
            int res = -1;// invalid
            if( this->currentNode>=this->first
                && this->currentNode<this->after_last // NB. has to be strictly <
                )
            {
                res =  *(this->currentNode);
            }// else invalid
            return res;
        }// operator deference (*)

        bool operator!=(Bulk_Entity::myIterator other)
        {
            return this->currentNode != other.currentNode;
        }// operator !=

        bool operator==(Bulk_Entity::myIterator other)
        {
            return this->currentNode == other.currentNode;
        }// operator ==

        bool operator<(Bulk_Entity::myIterator other)
        {
            return this->currentNode < other.currentNode;
        }// operator <

    };// class Iterator-------------------------internal------------------------


    Bulk_Entity()
    {
        this->containerCapacity = +55;// TODO let it parametrized
        this->vec = new int[containerCapacity];
        for(int c=0; c<containerCapacity; c++)
        {
            this->vec[c] = c;
        }
    }// Ctor

    ~Bulk_Entity()
    {
        delete[] this->vec;
    }// Dtor


    Bulk_Entity::myIterator myBegin()
    {
        Bulk_Entity::myIterator theIterator;
        theIterator.first = (&vec[0])+0;
        theIterator.after_last = (&(vec[containerCapacity-1]))+1;// one after last
        theIterator.currentNode = theIterator.first;// init to first
        return theIterator;
    }

    Bulk_Entity::myIterator myEnd()
    {
        Bulk_Entity::myIterator theIterator;
        theIterator.first = (&vec[0])+0;
        theIterator.after_last = (&(vec[containerCapacity-1]))+1;// one after last
        theIterator.currentNode = theIterator.after_last;// init to one_after_last
        return theIterator;
    }// end

private:
    int containerCapacity;
    int *vec;
};// class Bulk_Entity


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

