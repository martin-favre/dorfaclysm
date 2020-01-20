#include "Block.h"

#include "GridActor.h"
void Block::assignJob(JobType type) { mAssignedJob = type; }

void Block::unassignJob() { mAssignedJob = jobTypeUnset; }

JobType Block::getJob() const { return mAssignedJob; }

bool Block::isExplored() const { return mExplored; }
void Block::setExplored() { mExplored = true; }
