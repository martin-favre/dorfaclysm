#pragma once
#include <SDL_image.h>

class Rect {
 public:
  Rect() = default;
  constexpr Rect(int x, int y, int w, int h) : mRect{x, y, w, h} {}
  constexpr Rect(const SDL_Rect& sdlRect) : mRect(sdlRect) {}
  constexpr const SDL_Rect& getSdlRect() const {return mRect;};
  constexpr int x() const {return mRect.x;}
  constexpr int y() const {return mRect.y;}
  constexpr int w() const {return mRect.w;}
  constexpr int h() const {return mRect.h;}
 private:
  SDL_Rect mRect{0, 0, 0, 0};
};