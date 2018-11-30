#pragma once
#include "Game.h"
#include "Button.h"

class Button;
class wdata;

class Lostscreen
{
  public:
    Lostscreen();
    ~Lostscreen();
    void update();
    void draw();
    enum
    {
        OK,
        BackToMenu,
        Exit
    };

  private:
    SDL_Rect winsize;
    std::vector<Button *> buttons;
    std::string name;
    SDL_Texture *inputtex;
    SDL_Texture *headertex;
    SDL_Rect inputRect,headerRect;
    std::vector<wdata*> datas;
};


class wdata
{
public:
  wdata(std::string n, uint64_t sc) : name(n), score(sc) {}
  ~wdata() {}
  std::string name;
  uint64_t score;
};
