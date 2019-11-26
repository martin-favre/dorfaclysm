#include "Font.h"

Font::Font(TTF_Font* font, int mSize) : mFont(font), msize(size){}

const TTF_Font* Font::getSdlFont()
{
    return mFont;
}