#pragma once
#include "SDL2/SDL.h"
#include <vector>
#include <iterator>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <iostream>
#include "json/json.hpp"
#include "Game.h"

using json = nlohmann::json;

class data;
class listRender;
class Leaderboard
{
public:
  Leaderboard();
  ~Leaderboard();
  void update();
  void draw();

private:
  SDL_Texture *boardtex,*headertex;
  SDL_Rect boardRect,headerRect;
  std::vector<int> scores;
  std::vector<std::string> names;
  std::vector<data *> datas;
  std::vector<listRender *> list;
  int min_score;
};

class data
{
public:
  data(std::string n, uint64_t sc) : name(n), score(sc) {}
  ~data() {}
  std::string name;
  uint64_t score;
};

class listRender
{
public:
  listRender(SDL_Texture *t,float o,int posx,int posy)
  {
    tex = t;
    SDL_QueryTexture(tex, nullptr, nullptr, &rec.w, &rec.h);
    rec.x = posx;
    rec.y = posy * o;
  }
  ~listRender() {}
  SDL_Rect rec;
  SDL_Texture *tex;
};