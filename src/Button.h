#pragma once

#include "Game.h"

class Button
{
public:
  Button(std::string type, int t, int xpos, int ypos);
  Button(int t, std::string text, int xpos, int ypos);
  ~Button();

  void draw();
  void update();
  int tag;
  SDL_Rect destRect;

private:
  SDL_Texture *tex;
};