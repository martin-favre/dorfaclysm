#include "Rect.h"

Rect::Rect(const SDL_Rect& sdlRect): mRect(sdlRect)
{}

Rect::Rect(int x, int y, int w, int h): mRect{x,y,w,h} {}

const SDL_Rect& Rect::getSdlRect()const
{
    return mRect;
}