#pragma once
#include <SDL_ttf.h>
class Font {
 public:
  Font(TTF_Font* font, int size);
  TTF_Font* getSdlFont();
  int getSize();

 private:
  TTF_Font* mFont{nullptr};
  const int mSize{0};
};