#include "StartMenu.h"
#include "Collision.h"

SDL_Texture *name;
SDL_Rect namerect = {50, 100, 50, 50};

StartMenu::StartMenu()
{
    bgtex = Game::assets->GetTexture("bg");
    bgDest = {0, 0, 800, 640};
    bgSrc = {0, 0, 1200, 996};
    cursortex = Game::assets->GetTexture("cursor");
    cursorSrc = {0, 0, 128, 128};
    cursorDest = {0, 0, 232, 128};

    buttons.push_back(new Button("STARTBTN", START, 120, 350));
    buttons.push_back(new Button("LDBBTN", LEADERBOARD, 120, 400));
    buttons.push_back(new Button("EXITBTN", EXIT, 120, 450));

    activeBtn = START;
    openldb = false;
    ldb = new Leaderboard();
}
StartMenu::~StartMenu() {}

void StartMenu::update()
{
    for (auto &btn : buttons)
    {
        if (btn->tag == activeBtn)
        {
            if (btn->tag == START)
            {
                cursorDest = {btn->destRect.x, btn->destRect.y - 32, 232 - 64, cursorDest.h};
            }
            if (btn->tag == LEADERBOARD)
            {
                cursorDest = {btn->destRect.x - 32, btn->destRect.y - 30, 232 + 10, cursorDest.h};
            }
            if (btn->tag == EXIT)
            {
                cursorDest = {btn->destRect.x + 15, btn->destRect.y - 32, 232 - 96, cursorDest.h};
            }
        }
        if (!openldb && Game::event.button.button == SDL_BUTTON_LEFT)
        {
            int x, y;
            SDL_GetMouseState(&x, &y);
            if (Collision::intersect(x, y, btn->destRect))
            {
                switch (btn->tag)
                {
                case START:
                    Game::isStart = true;
                    activeBtn = START;
                    cursorDest = {btn->destRect.x, btn->destRect.y - 32, 232 - 64, cursorDest.h};
                    Mix_PlayMusic(Game::assets->GetBGM("stage1"), -1);
                    break;
                case LEADERBOARD:
                    openldb = true;
                    activeBtn = LEADERBOARD;
                    cursorDest = {btn->destRect.x - 32, btn->destRect.y - 30, 232 + 10, cursorDest.h};
                    break;
                case EXIT:
                    Game::isRunning = false;
                    activeBtn = EXIT;
                    cursorDest = {btn->destRect.x + 15, btn->destRect.y - 32, 232 - 96, cursorDest.h};
                    break;
                }
            }
        }
    }
    if (!openldb)
    {
        if (Game::event.type == SDL_KEYDOWN)
        {
            if (Game::event.key.keysym.sym == SDLK_DOWN)
            {
                activeBtn++;
                if (activeBtn > 3)
                {
                    activeBtn = START;
                }
            }
            if (Game::event.key.keysym.sym == SDLK_UP)
            {
                activeBtn--;
                if (activeBtn < 1)
                {
                    activeBtn = EXIT;
                }
            }
            if (Game::event.key.keysym.sym == SDLK_RETURN && activeBtn == START)
            {
                Game::isStart = true;
            }
            if (Game::event.key.keysym.sym == SDLK_RETURN && activeBtn == EXIT)
            {
                Game::isRunning = false;
            }
            if (Game::event.key.keysym.sym == SDLK_RETURN && activeBtn == LEADERBOARD)
            {
                openldb = true;
            }
        }
    }
    else if (openldb)
    {
        if (Game::event.type == SDL_KEYDOWN)
        {
            openldb = false;
        }
        if (Game::event.type == SDL_MOUSEBUTTONDOWN)
        {
            openldb = false;
        }
        ldb->update();
    }
    {
        SDL_Surface *surf = TTF_RenderText_Blended(Game::assets->GetFont("arcadefont"), "NARAWIT TRIPROM \n 61010556", {135, 50, 96, 255});
        name = SDL_CreateTextureFromSurface(Game::renderer, surf);
        SDL_FreeSurface(surf);
        SDL_QueryTexture(name, nullptr, nullptr, &namerect.w, &namerect.h);
    }
    SDL_Delay(100);
}

void StartMenu::draw()
{
    TextureManager::Draw(bgtex, bgSrc, bgDest, SDL_FLIP_NONE);
    for (auto &btn : buttons)
    {
        btn->draw();
    }
    TextureManager::Draw(cursortex, cursorSrc, cursorDest, SDL_FLIP_NONE);
    SDL_RenderCopy(Game::renderer, name, NULL, &namerect);
    if (openldb)
    {
        ldb->draw();
    }
}
