#pragma once
#include "Game.h"
class HUD
{
  public:
    HUD();
    ~HUD();
    void draw();
    void update();

  private:
    SDL_Texture *bartex, *hptex, *mptex;
    SDL_Rect hpdestRect, hpsrcRect;
    SDL_Rect mpdestRect, mpsrcRect;
    SDL_Rect barsrcR, bardestR;
};