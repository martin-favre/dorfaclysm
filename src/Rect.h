#pragma once
#include <SDL_image.h>

class Rect
{
    public:
    Rect()=default;
    Rect(int x, int y = 0, int w = 0, int h = 0);
    Rect(const SDL_Rect& sdlRect);
    const SDL_Rect& getSdlRect()const;
    private:
    SDL_Rect mRect{0,0,0,0};
};