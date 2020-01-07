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

  void nextLevel() {
    level += level_weight;
  }

  // Estimation function for the remaining distance to the goal.
  int estimate(const Vector3DInt& goal) const {
    // Squared Euclidean Distance
    const int d = Helpers::roundToInt(
        Helpers::getSquaredPositionBetweenPositions(goal, position));

    // const int xd = goal.x - position.x;
    // const int yd = goal.x - position.y;
    // Manhattan distance
    // const int d=abs(xd)+abs(yd);

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

bool isStepValid(const GridMap& map, const Vector3DInt& newPos,
                 const Vector3DInt& dir) {
  if (!map.isPosInMap(newPos)) return false;
  if (!map.isPosFree(newPos)) return false;
  const Block& block = map.getBlockAt(newPos);
  const BlockMovementType type = block.getMovementType();
  if (dir.z > 0) {
    if (type != movementUpBlock && type != movementUpAndDownBlock) return false;
  } else if (dir.z < 0) {
    if (type != movementDownBlock && type != movementUpAndDownBlock)
      return false;
  }
  return true;
}

bool Astar::getPath(const Vector3DInt& start, const Vector3DInt& finish,
                    const GridMap& map, std::stack<Vector3DInt>& path) {
  if (!map.isPosInMap(start)) {
    Logging::log("Could not find path, the start was outside map");
    return false;
  }
  if (!map.isPosInMap(finish)) {
    Logging::log("Could not find path, the target position outside map");
    return false;
  }
  if (!map.isPosFree(finish)) {
    // bool success = false;
    // for (int dir = 0; dir < DIRECTIONS; ++dir) {
    //   const Vector3DInt newPos(finish.x + DX[dir], finish.y + DY[dir]);
    //   success = getPath(start, newPos, map, path, retryDepth + 1);
    //   if(success) return success;
    // }
    Logging::log("Could not find path, the target position was occupied");
    return false;
  }
  std::priority_queue<AStarNode*, std::vector<AStarNode*>, Compare> node_queue;
  AStarNode* current_node = new AStarNode(start, 0, 0, nullptr);
  current_node->updatePriority(finish);
  node_queue.push(current_node);
  std::map<Vector3DInt, int> map_weights;
  map_weights[start] = current_node->getPriority();
  int steps = 0;
  std::vector<AStarNode*> old_nodes;
  while (!node_queue.empty()) {
    ++steps;
    if (steps > 100000) {
      Logging::log("Could not find path, over max number of steps");
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
        const bool valid_step = isStepValid(map, newPos, delta);
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
  Logging::log("Could not find path, no possible path");
  return false;
}
