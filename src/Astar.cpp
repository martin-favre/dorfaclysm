#include "Astar.h"

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
    // Squared Euclidean Distance
    // const int d = Helpers::roundToInt(
    //     Helpers::getSquaredPositionBetweenPositions(goal, position));
    const Vector3DInt diff = goal - position;
    const int d = diff.squared_magnitude();
    // const int xd = goal.x - position.x;
    // const int yd = goal.y - position.y;
    // const int zd = goal.z - position.z;
    // Manhattan distance
    // const int d = abs(xd) + abs(yd) + abs(zd);

    // Chebyshev distance
    // const int d=std::max(abs(xd), abs(yd));

    return (d);
  }
};

class Compare {
 public:
  bool operator()(const AStarNode* a, const AStarNode* b) {
    return a->getPriority() > b->getPriority();
  }
};

static bool isStepValid(const GridMap& map, const Vector3DInt& newPos,
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

bool Astar::getPath(const Vector3DInt& start, const Vector3DInt& finish,
                    const GridMap& map, std::stack<Vector3DInt>& path) {
  if (!map.isPosInMap(start)) {
    LOG("Could not find path, the start was outside map");
    return false;
  }
  if (!map.isPosInMap(finish)) {
    LOG("Could not find path, the target position outside map");
    return false;
  }
  if (!map.isPosFree(finish)) {
    LOG("Could not find path, the target position was occupied");
    return false;
  }
  std::priority_queue<AStarNode*, std::vector<AStarNode*>, Compare> node_queue;
  AStarNode* current_node = new AStarNode(start, 0, 0, nullptr);
  current_node->updatePriority(finish);
  node_queue.push(current_node);

  std::unordered_map<Vector3DInt, int, Vector3DIntHash> map_weights;
  map_weights[start] = current_node->getPriority();
  int steps = 0;
  std::vector<AStarNode*> old_nodes;
  while (!node_queue.empty()) {
    ++steps;
    if (steps > 1000) {
      LOG("Could not find path, over max number of steps");
      return false;
    }
    current_node = node_queue.top();
    node_queue.pop();

    if (current_node->get_pos() == finish) {
      AStarNode const* curr = current_node;
      while (curr->get_parent() != nullptr) {
        Vector3DInt pos = curr->get_pos();
        path.push(pos);
        curr = curr->get_parent();
      }
      while (!node_queue.empty()) {
        delete node_queue.top();
        node_queue.pop();
      }
      for (auto it = old_nodes.begin(); it != old_nodes.end(); ++it) {
        delete *it;
      }
      delete current_node;
      return true;
      /* Generate output path and cleanup*/
    } else {
      for (const Vector3DInt& delta : DELTA_POSITIONS) {
        const Vector3DInt currentPos = current_node->get_pos();
        const Vector3DInt newPos{currentPos + delta};

        /*if x and y are within boundaries and
        the tile is not impassable and
        then it's a valid step
        */
        const bool valid_step = isStepValid(map, newPos, currentPos, delta);
        if (valid_step) {
          AStarNode* next_node =
              new AStarNode(newPos, current_node->getLevel(),
                            current_node->getPriority(), current_node);
          next_node->nextLevel();
          next_node->updatePriority(finish);

          /* if my priority is better, mark it down and continue */
          if (map_weights.count(newPos) == 0) {
            map_weights[newPos] = std::numeric_limits<int>::max();
          }

          if (next_node->getPriority() < map_weights[newPos]) {
            map_weights[newPos] = next_node->getPriority();
            node_queue.push(next_node);
          } else {
            delete next_node;
          }
        }
      }
      old_nodes.push_back(current_node);
    }
  }
  LOG("Could not find path, no possible path");
  return false;
}
