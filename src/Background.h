#pragma once

#include "Game.h"

class Background
{
  public:
    Background(std::string id,int xpos,int ypos,int w,int h,int speed);
    ~Background();
    SDL_Rect destRect,srcRect;
    SDL_Texture *tex;

    void update();
    void draw();

  private:
    int speed;
    std::string type;
    uint32_t time;
};