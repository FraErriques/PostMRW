#include <vector>
#include "Test_Unit_selectInterval.h"

Test_Unit_selectInterval::Test_Unit_selectInterval()
{//ctor
}//ctor

Test_Unit_selectInterval::~Test_Unit_selectInterval()
{
    //dtor
}

void Test_Unit_selectInterval::managementMethod()
{
    int interval_one = selectInterval( 5);
    int interval_two = selectInterval( 17);
    int interval_invalidLeft = selectInterval( -1);
    int interval_invalidRight = selectInterval( +33);
    //---act on variables after test, to avoid misleading warnings. Only setting the value is not enough.
    interval_one++;
    interval_two++;
    interval_invalidLeft++;
    interval_invalidRight++;
}// managementMethod


// distributionFunction_fromExistingMesh :must fill up th global::vector<BoundaryCumulateMesh>
int Test_Unit_selectInterval::selectInterval(int candidate)
{// TODO move into getNearestIntegral : it's the vector reader and interval selector
    std::vector<int> intervalBoundaries;
    intervalBoundaries.push_back(  0);
    intervalBoundaries.push_back( 10);
    intervalBoundaries.push_back( 20);
    size_t boundaryCardinality = intervalBoundaries.size();
    //
    size_t c=0;
    size_t selectedInterval=-1;// init to invalid
    for( ; c<boundaryCardinality-1; c++)// NB. if c+1<n then c<n-1
    {
        if(
            candidate > intervalBoundaries[c]
            && candidate <= intervalBoundaries[c+1]
           )
        {
            selectedInterval = c;
            break;
        }// else continue : to select Interval
    }// for
    //ready.
    return selectedInterval;
}// selectInterval
