#include "Block.h"

#include "GridActor.h"
Block::Block(const BlockIdentifier& identifier) : mIdentifier(identifier) {}
bool Block::isExplored() const { return mExplored; }
void Block::setExplored() { mExplored = true; }
const BlockIdentifier& Block::getIdentifier() const { return mIdentifier; }