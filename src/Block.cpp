#include "Block.h"

#include "WorldItem.h"
void Block::assignJob(JobType type) { mAssignedJob = type; }

void Block::unassignJob() { mAssignedJob = jobTypeUnset; }

JobType Block::getJob() const { return mAssignedJob; }
