#include "../../Common/BinaryTree/BinaryTree.h"

// call Common:: from Process:: which is a C interface.
int BinTree_wrapper_C_( char *dataFilePath )
{
    int result = TreeManagement( dataFilePath );
    // ready
    return result;
}// end BinTree_wrapper_C_
