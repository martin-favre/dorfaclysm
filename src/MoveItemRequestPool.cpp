#include "MoveItemRequestPool.h"

MoveItemRequestPool MoveItemRequestPool::mInstance;
MoveItemRequestPool& MoveItemRequestPool::getInstance() { return mInstance; }
