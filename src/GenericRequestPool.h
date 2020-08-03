
#pragma once
#include <algorithm>
#include <cstddef>
#include <functional>
#include <list>
#include <memory>
#include <mutex>
#include <thread>

#include "Logging.h"
#include "Timer.h"
#include "Vector3DInt.h"
template <class T>
class GenericRequestPool {
 public:
  void addRequest(std::unique_ptr<T> &&job) {
    std::scoped_lock lock(mMutex);
    ASSERT(job.get(), "Received null request");
    LOGL("Adding request " << job.get(), Logging::info);

    auto comper = [&job](std::shared_ptr<T> &req) { return *job == *req; };

    auto it = std::find_if(mRequests.begin(), mRequests.end(), comper);

    if (it == mRequests.end()) {
      mRequests.emplace_back(std::move(job));
    } else {
      LOGL("request already exists", Logging::info);
    }
  }
  bool hasRequests() const {
    std::scoped_lock lock(mMutex);
    return mRequests.size();
  }
  std::shared_ptr<T> getClosestTo(const Vector3DInt &pos) {
    std::scoped_lock lock(mMutex);
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

  void returnRequest(std::shared_ptr<T> &&request) {
    ASSERT(request.get(), "Returned nullptr");
    std::function<void(std::shared_ptr<T>, size_t)> foo =
        [this](std::shared_ptr<T> a, size_t b)
    { this->returnRequestInternal(a, b); };
    std::thread(foo, std::move(request), 1000).detach();
  }

  void clearRequests() { mRequests.clear(); }

  const std::list<std::shared_ptr<T>> &getRequests() const { return mRequests; }

 private:
  std::list<std::shared_ptr<T>> mRequests;
  mutable std::mutex mMutex;

  void returnRequestInternal(std::shared_ptr<T> request, size_t sleepDuration) {
    std::this_thread::sleep_for(std::chrono::milliseconds(sleepDuration));
    std::scoped_lock lock(mMutex);
    mRequests.emplace_back(request);
  }
};

template <class T>
void to_json(SerializedObj &out, const GenericRequestPool<T> &vec) {
  (void)out;
  (void)vec;
}

template <class T>
void from_json(const SerializedObj &serObj, GenericRequestPool<T> &vec) {
  (void)serObj;
  (void)vec;
}
