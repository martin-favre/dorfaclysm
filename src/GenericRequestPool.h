
#pragma once
#include <memory>
#include <algorithm>
#include "Vector3DInt.h"
#include <list>
template <class T>
class GenericRequestPool {
 public:
  void addRequest(std::unique_ptr<T> &&job) {
    ASSERT(job.get(), "Received null request");
    cleanHandedoutRequests();
    auto comper = [&job](std::shared_ptr<T> &req) { return *job == *req; };

    auto comperHandOut = [&job](std::weak_ptr<T> &req) {
      return *job == *req.lock();
    };

    auto it = std::find_if(mRequests.begin(), mRequests.end(), comper);

    if (it == mRequests.end()) {
      auto it = std::find_if(mHandedOutRequests.begin(),
                             mHandedOutRequests.end(), comperHandOut);
      if (it == mHandedOutRequests.end()) {
        mRequests.emplace_back(std::move(job));
      }
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
    mHandedOutRequests.emplace_back(*smallestIndx);
    std::shared_ptr<T> yourCopy = *smallestIndx;
    mRequests.erase(smallestIndx);
    return yourCopy;
  }
  const std::list<std::shared_ptr<T>> &getRequests() const { return mRequests; }
  const std::list<std::weak_ptr<T>> &getClaimedRequests() {
    cleanHandedoutRequests();
    return mHandedOutRequests;
  }
  void returnRequest(std::shared_ptr<T> &&request) {
    cleanHandedoutRequests();
    for (auto it = mHandedOutRequests.begin(); it != mHandedOutRequests.end();
         ++it) {
      {
        if (*it->lock() == *request) {
          mHandedOutRequests.erase(it);
          break;
        }
      }
    }
    mRequests.emplace_back(std::move(request));
  }

 private:
  void cleanHandedoutRequests() {
    for (auto it = mHandedOutRequests.begin();
         it != mHandedOutRequests.end();) {
      if (it->expired()) {
        it = mHandedOutRequests.erase(it);
      } else {
        ++it;
      }
    }
  }
  std::list<std::shared_ptr<T>> mRequests;
  std::list<std::weak_ptr<T>> mHandedOutRequests;
};
