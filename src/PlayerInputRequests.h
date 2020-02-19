#pragma once
#include <queue>
#include <mutex>
#include "Vector3DInt.h"

class MouseClickInfo {
 public:
  MouseClickInfo(int button, const Vector3DInt& pos);
  const Vector3DInt& getPos() const;
  int getButton() const;

 private:
  const int mButton;
  const Vector3DInt mPos;
};

class PlayerInputRequests {
 public:
  static bool hasMouseClicks();
  static void enqueueMouseClick(const MouseClickInfo& info);
  static MouseClickInfo dequeueMouseClick();
  static bool hasKeyPresses();
  static void enqueueKeyPress(int key);
  static int dequeueKeyPress();

 private:
  static std::queue<int> mKeyPresses;
  static std::queue<MouseClickInfo> mMouseClicks;
  static std::mutex mLock;
};
