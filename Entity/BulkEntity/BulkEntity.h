
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


    Bulk_Entity();
    ~Bulk_Entity();
    Bulk_Entity::myIterator myBegin();
    Bulk_Entity::myIterator myEnd();

private:
    int containerCapacity;
    int *vec;
};// class Bulk_Entity

}//nmsp Entity
