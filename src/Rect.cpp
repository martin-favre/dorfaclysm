#include "Rect.h"

Rect::Rect(const SDL_Rect& sdlRect): mRect(sdlRect)
{}

const SDL_Rect& Rect::getRect()const
{
    return mRect;
}