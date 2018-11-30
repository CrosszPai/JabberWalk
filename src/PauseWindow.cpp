#include "PauseWindow.h"
#include "Collision.h"

PauseWindow::PauseWindow()
{
    winsize = {0, 0, 800, 640};
    float order = 1;
    buttons.push_back(new Button(RESUME, "RESUME", winsize.w / 2, 200 * order));
    order += 0.5;
    buttons.push_back(new Button(RESTART, "RESTART", winsize.w / 2, 200 * order));
    order += 0.5;
    buttons.push_back(new Button(BackToMenu, "Back to menu", winsize.w / 2, 200 * order));
    order += 0.5;
    buttons.push_back(new Button(EXIT, "EXIT", winsize.w / 2, 200 * order));
}

PauseWindow::~PauseWindow()
{
}

void PauseWindow::update()
{
    if (Game::event.type == SDL_KEYDOWN && Game::pause)
    {
        if (Game::event.key.keysym.sym == SDLK_ESCAPE)
        {
            Game::pause = false;
        }
    }
    if (Game::event.type == SDL_MOUSEBUTTONDOWN)
    {
        if (Game::event.button.button == SDL_BUTTON_LEFT)
        {
            for (auto &b : buttons)
            {
                b->update();
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (Collision::intersect(x, y, b->destRect) && Game::pause)
                {
                    switch (b->tag)
                    {
                    case RESUME:
                        Game::pause = false;
                        break;
                    case RESTART:
                        Game::Reset();
                        Game::pause = false;
                        Mix_PlayMusic(Game::assets->GetBGM("stage1"), -1);
                        break;
                    case BackToMenu:
                        Game::Reset();
                        Game::pause = false;
                        Game::isStart = false;
                        Mix_PlayMusic(Game::assets->GetBGM("menu"), -1);
                        break;
                    case EXIT:
                        Game::isRunning = false;
                        break;
                    }
                }
            }
        }
    }
    SDL_Delay(200);
}

void PauseWindow::draw()
{
    SDL_SetRenderDrawBlendMode(Game::renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(Game::renderer, 0, 0, 0, 200);
    SDL_RenderFillRect(Game::renderer, &winsize);
    for (auto &b : buttons)
    {
        b->draw();
    }
}