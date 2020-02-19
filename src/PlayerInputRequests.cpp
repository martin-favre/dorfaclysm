#include "PlayerInputRequests.h"

std::queue<int> PlayerInputRequests::mKeyPresses;
std::queue<MouseClickInfo> PlayerInputRequests::mMouseClicks;
std::mutex PlayerInputRequests::mLock;

MouseClickInfo::MouseClickInfo(int button, const Vector3DInt& pos)
    : mButton(button), mPos(pos) {}
const Vector3DInt& MouseClickInfo::getPos() const { return mPos; }
int MouseClickInfo::getButton() const { return mButton; }

void PlayerInputRequests::enqueueMouseClick(const MouseClickInfo& info) {
  std::scoped_lock lock(mLock);
  mMouseClicks.emplace(info);
}
MouseClickInfo PlayerInputRequests::dequeueMouseClick() {
  std::scoped_lock lock(mLock);
  MouseClickInfo out = mMouseClicks.front();
  mMouseClicks.pop();
  return out;
}

bool PlayerInputRequests::hasMouseClicks() {
  std::scoped_lock lock(mLock);
  return !mMouseClicks.empty();
}

void PlayerInputRequests::enqueueKeyPress(int key) {
  std::scoped_lock lock(mLock);
  mKeyPresses.emplace(key);
}
int PlayerInputRequests::dequeueKeyPress() {
  std::scoped_lock lock(mLock);
  int out = mKeyPresses.front();
  mKeyPresses.pop();
  return out;
}

bool PlayerInputRequests::hasKeyPresses() {
  std::scoped_lock lock(mLock);
  return !mKeyPresses.empty();
}
