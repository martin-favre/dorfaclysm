#include "Font.h"

Font::Font(TTF_Font* font, int size) : mFont(font), mSize(size) {}

TTF_Font* Font::getSdlFont() { return mFont; }

int Font::getSize() { return mSize; }