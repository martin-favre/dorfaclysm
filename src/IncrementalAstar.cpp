#include "IncrementalAstar.h"

#include <limits>
#include <map>
#include <queue>

#include "Block.h"
#include "DeltaPositions.h"
#include "GridMap.h"
#include "Helpers.h"
#include "Logging.h"

/*
        Implementation stolen/inspired from here:
        http://code.activestate.com/recipes/577457-a-star-shortest-path-algorithm/
*/

class Vec3Hash {
 public:
  size_t operator()(const Vector3DInt& vec) const {
    constexpr int random_prime_1 = 7919;
    constexpr int random_prime_2 = 6599;
    return (vec.x * random_prime_1 + vec.y) * random_prime_2 + vec.z;
  }
};

class AStarNode {
  /* Tuning parameters
          Increase level weights to punish every extra level taken
          Increase estimation weight to punish the distance estimation.
  */
  static constexpr int level_weight = 40;
  static constexpr int estimation_weight = 10;

  const Vector3DInt position;
  // total distance already traveled to reach the node
  int level;
  // priority=level+remaining distance estimate
  int priority;  // smaller: higher priority

  const AStarNode* parent;

 public:
  std::string name;
  AStarNode(const Vector3DInt& vec, int d, int p, AStarNode* par)
      : position(vec), level(d), priority(p), parent(par) {}
  ~AStarNode() {}
  const Vector3DInt& get_pos() const { return position; }
  int getLevel() const { return level; }
  int getPriority() const { return priority; }
  const AStarNode* get_parent() const { return parent; }

  void updatePriority(const Vector3DInt& goal) {
    priority = level + estimate(goal) * estimation_weight;
  }

  void nextLevel() { level += level_weight; }

  // Estimation function for the remaining distance to the goal.
  int estimate(const Vector3DInt& goal) const {
    const Vector3DInt diff = goal - position;
    const int d = diff.squared_magnitude();

    return (d);
  }
};

class Compare {
 public:
  bool operator()(const AStarNode* a, const AStarNode* b) {
    return a->getPriority() > b->getPriority();
  }
};

class Pimple {
 public:
  std::priority_queue<AStarNode*, std::vector<AStarNode*>, Compare> mNodeQueue;
  std::unordered_map<Vector3DInt, int, Vec3Hash> mMapWeights;
};

bool isStepValid(const GridMap& map, const Vector3DInt& newPos,
                 const Vector3DInt& currentPos, const Vector3DInt& dir) {
  if (!map.isPosInMap(newPos)) return false;
  if (!map.isPosFree(newPos)) return false;
  /**
   * If I am moving up, I want to know if my current block allows it
   *     i.e. does it allow climbing?
   * If I am moving down, I want to know if the block below me allows it
   *     i.e. is it passable?
   * If I am moving horizontal, I want to know if the block below my next pos
   * allows it i.e. Does it allow be to step on top of it. AirBlock says no,
   * rest says yes
   */
  if (dir.z != 0) {
    if (dir.z > 0) {
      const Block& nextBlock = map.getBlockAt(currentPos);
      return nextBlock.mayClimbUpFrom();

    } else {
      return true;
    }
  } else {
    Vector3DInt posBelowNext = newPos + Vector3DInt{0, 0, -1};
    if (map.isPosInMap(posBelowNext)) {
      const Block& belowNextBlock = map.getBlockAt(posBelowNext);
      return belowNextBlock.mayWalkOnTop();
    } else {
      return true;  // You can walk on the bottom of the world
    }
  }
}

IncrementalAstar::IncrementalAstar(const Vector3DInt& start,
                                   const Vector3DInt& end, const GridMap& map,
                                   int64_t stepTimeMs)
    : mStart(start),
      mEnd(end),
      mMap(map),
      mStepTimeMs(stepTimeMs),
      mInternals(std::make_unique<Pimple>()) {
  if (!map.isPosInMap(start)) {
    LOG("Could not find path, the start was outside map");
    mIsDone = true;
  }
  if (!map.isPosInMap(end)) {
    LOG("Could not find path, the target position outside map");
    mIsDone = true;
  }
  if (!map.isPosFree(end)) {
    LOG("Could not find path, the target position was occupied");
    mIsDone = true;
  }

  AStarNode* currentNode = new AStarNode(mStart, 0, 0, nullptr);
  currentNode->updatePriority(mEnd);
  mInternals->mNodeQueue.push(currentNode);

  mInternals->mMapWeights[mStart] = currentNode->getPriority();
}

IncrementalAstar::~IncrementalAstar() {
  while (!mInternals->mNodeQueue.empty()) {
    delete mInternals->mNodeQueue.top();
    mInternals->mNodeQueue.pop();
  }
  for (auto& node : mOldNodes) {
    delete node;
  }
  if (mFinishNode) {
    delete mFinishNode;
  }
}

bool IncrementalAstar::isDone() { return mIsDone; }
bool IncrementalAstar::foundPath() { return mFoundPath; }
void IncrementalAstar::getPath(std::stack<Vector3DInt>& path) {
  if (!mFinishNode) return;
  const AStarNode* curr = mFinishNode;
  while (curr->get_parent() != nullptr) {
    const Vector3DInt& pos = curr->get_pos();
    path.push(pos);
    curr = curr->get_parent();
  }
}

void IncrementalAstar::calculatePath() {
  mTimer.start();
  while (!mInternals->mNodeQueue.empty()) {
    AStarNode* currentNode = mInternals->mNodeQueue.top();
    mInternals->mNodeQueue.pop();

    if (currentNode->get_pos() == mEnd) {
      mIsDone = true;
      mFoundPath = true;
      mFinishNode = currentNode;
      return;
    } else {
      for (const Vector3DInt& delta : DELTA_POSITIONS) {
        const Vector3DInt currentPos = currentNode->get_pos();
        const Vector3DInt newPos{currentPos + delta};

        /*if x and y are within boundaries and
        the tile is not impassable and
        then it's a valid step
        */
        const bool valid_step = isStepValid(mMap, newPos, currentPos, delta);
        if (valid_step) {
          AStarNode* next_node =
              new AStarNode(newPos, currentNode->getLevel(),
                            currentNode->getPriority(), currentNode);
          next_node->nextLevel();
          next_node->updatePriority(mEnd);

          /* if my priority is better, mark it down and continue */
          if (mInternals->mMapWeights.count(newPos) == 0) {
            mInternals->mMapWeights[newPos] = std::numeric_limits<int>::max();
          }

          if (next_node->getPriority() < mInternals->mMapWeights[newPos]) {
            mInternals->mMapWeights[newPos] = next_node->getPriority();
            mInternals->mNodeQueue.push(next_node);
          } else {
            delete next_node;
          }
        }
      }
      mOldNodes.push_back(currentNode);
    }

    // do at the end so we at least get one round
    if (mTimer.getElapsedMilliseconds() > mStepTimeMs) {
      return;
    }
  }
  mIsDone = true;
  LOG("Could not find path, no path available");
}
