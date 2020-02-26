
#pragma once
#include <algorithm>
#include <list>
#include <memory>
#include "Logging.h"
#include "Vector3DInt.h"
template <class T>
class GenericRequestPool {
 public:
  void addRequest(std::unique_ptr<T> && job) {
    ASSERT(job.get(), "Received null request");
    LOG("Adding request " << job.get());
    
    auto comper = [&job](std::shared_ptr<T> &req) { return *job == *req; };

    auto it = std::find_if(mRequests.begin(), mRequests.end(), comper);

    if (it == mRequests.end()) {
      mRequests.emplace_back(std::move(job));
    }
    else{
      LOG("request already exists ");
    }
  }
  bool hasRequests() const { return mRequests.size(); }
  std::shared_ptr<T> getClosestTo(const Vector3DInt &pos) {
    double smallestDistance = DBL_MAX;
    auto smallestIndx = mRequests.begin();
    for (auto it = mRequests.begin(); it != mRequests.end(); ++it) {
      Vector3DInt diff = pos - (*it)->getPos();
      double dist = diff.squared_magnitude();
      if (dist < smallestDistance) {
        smallestIndx = it;
        smallestDistance = dist;
      }
    }
    ASSERT(smallestDistance != DBL_MAX, "Call hasRequests first");
    std::shared_ptr<T> yourCopy = *smallestIndx;
    mRequests.erase(smallestIndx);
    return yourCopy;
  }
  const std::list<std::shared_ptr<T>> &getRequests() const { return mRequests; }
  void returnRequest(std::shared_ptr<T> &&request) {
    ASSERT(request.get(), "Returned nullptr");
    mRequests.emplace_back(std::move(request));
  }

 private:
  std::list<std::shared_ptr<T>> mRequests;
};
