#pragma once
#include <memory>
#include <queue>
#include <stack>

#include "Timer.h"
#include "Vector3DInt.h"

class GridMap;
class AStarNode;
class Pimple;

class IncrementalAstar {
 public:
  IncrementalAstar(const Vector3DInt& start, const Vector3DInt& end,
                   const GridMap& map, int64_t stepTimeMs = 3);
  ~IncrementalAstar();
  void calculatePath();
  bool isDone();
  bool foundPath();
  void getPath(std::stack<Vector3DInt>& path);

 private:
  const Vector3DInt mStart;
  const Vector3DInt mEnd;
  const GridMap& mMap;
  const int64_t mStepTimeMs;
  bool mIsDone{false};
  bool mFoundPath{false};
  Timer mTimer;
  AStarNode* mFinishNode{nullptr};
  std::unique_ptr<Pimple> mInternals;
  std::vector<AStarNode*> mOldNodes;
};