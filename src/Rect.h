#pragma once
#include <SDL_image.h>

class Rect
{
    public:
    Rect()=default;
    Rect(const SDL_Rect& sdlRect);
    const SDL_Rect& getRect()const;
    private:
    SDL_Rect mRect{0,0,0,0};
}