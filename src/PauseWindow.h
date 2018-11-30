#pragma once
#include "Game.h"
#include "Button.h"

class Button;
class PauseWindow
{
  public:
    PauseWindow();
    ~PauseWindow();
    void update();
    void draw();
    enum
    {
        RESUME,
        RESTART,
        BackToMenu,
        EXIT
    };

  private:
    SDL_Rect winsize;
    std::vector<Button *> buttons;
};

