#include "Block.h"

#include "GridActor.h"

bool Block::isExplored() const { return mExplored; }
void Block::setExplored() { mExplored = true; }
