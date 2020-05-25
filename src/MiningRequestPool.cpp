#include "MiningRequestPool.h"

#include <algorithm>
#include <functional>
#include <limits>

#include "Block.h"
#include "DeltaPositions.h"
#include "GridMap.h"

MiningRequestPool MiningRequestPool::mInstance;
MiningRequestPool &MiningRequestPool::getInstance() { return mInstance; }
