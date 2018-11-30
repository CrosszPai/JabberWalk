#pragma once
#include "SDL2/SDL.h"
#include "Game.h"
#include <vector>
#include "Button.h"
#include "Leaderboard.h"

class Button;
class Leaderboard;

class StartMenu
{
public:
  StartMenu();
  ~StartMenu();
  void update();
  void draw();
  enum
  {
    START=1,
    LEADERBOARD,
    EXIT
  };

private:
  SDL_Texture *bgtex;
  SDL_Rect bgDest, bgSrc;
  SDL_Texture *cursortex;
  SDL_Texture *info;
  SDL_Rect infosrc,infodest;
  SDL_Rect cursorDest, cursorSrc;
  Leaderboard *ldb;
  std::vector<Button *> buttons;
  int activeBtn;
  bool openldb;
};