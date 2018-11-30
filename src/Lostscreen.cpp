#include "Lostscreen.h"
#include "Collision.h"
#include "SDL2/SDL_ttf.h"

void to_json(json &j, const wdata *d)
{
    j = json{
        {"name", d->name},
        {"score", d->score}};
}
void from_json(const json &j, wdata *d)
{
    j.at("name").get_to(d->name);
    j.at("score").get_to(d->score);
}

Lostscreen::Lostscreen()
{
    winsize = {0, 0, 800, 640};
    float order = 1.5;
    buttons.push_back(new Button(OK, "Record", winsize.w / 2, 200 * order));
    order += 0.5;
    buttons.push_back(new Button(BackToMenu, "Back to Menu", winsize.w / 2, 200 * order));
    order += 0.5;
    buttons.push_back(new Button(Exit, "Exit Without saving score", winsize.w / 2, 200 * order));
    order += 0.5;
    inputRect.x = winsize.w / 2;
    inputRect.y = 200;
    std::ifstream i("record.json");
    json j;
    i >> j;
    name = "Input your name";
    for (auto &elm : j["data"])
    {
        datas.push_back(new wdata(elm["name"], elm["score"]));
    }
    SDL_Color red = {255, 0, 0, 255};
    SDL_Surface *surf = TTF_RenderText_Blended(TTF_OpenFont("assets/font/ARCADECLASSIC.TTF", 60), "You Lost!", red);
    headertex = SDL_CreateTextureFromSurface(Game::renderer, surf);
    SDL_FreeSurface(surf);
    SDL_QueryTexture(headertex, nullptr, nullptr, &headerRect.w, &headerRect.h);
    headerRect.x = winsize.w / 2 - headerRect.w / 2;
    headerRect.y = 100;

    SDL_StartTextInput();
}

Lostscreen::~Lostscreen()
{
}

void Lostscreen::update()
{

    SDL_Color white = {255, 255, 255, 255};
    SDL_Surface *surf = TTF_RenderText_Blended(Game::assets->GetFont("arcadefont"), name.c_str(), white);
    inputtex = SDL_CreateTextureFromSurface(Game::renderer, surf);
    SDL_FreeSurface(surf);
    SDL_QueryTexture(inputtex, nullptr, nullptr, &inputRect.w, &inputRect.h);
    inputRect.x = winsize.w / 2 - inputRect.w / 2;
    if (Game::event.type == SDL_KEYDOWN)
    {
        if (Game::event.key.keysym.sym == SDLK_BACKSPACE && name.length() > 0)
        {
            name.erase(name.length() - 1);
        }
    }
    if (Game::event.type == SDL_TEXTINPUT)
    {
        if (name == "Input your name")
        {
            name.clear();
        }
        name += Game::event.text.text;
        std::cout << Game::event.text.text << std::endl;
    }
    if (Game::event.type == SDL_MOUSEBUTTONDOWN)
    {
        if (Game::event.button.button == SDL_BUTTON_LEFT)
        {
            for (auto &b : buttons)
            {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (Collision::intersect(x, y, b->destRect) && Game::lost)
                {
                    switch (b->tag)
                    {
                    case OK:
                    {
                        datas.push_back(new wdata(name, Game::score));
                        std::sort(datas.begin(), datas.end(), [](const auto &ldata, const auto &rdata) {
                            return ldata->score > rdata->score;
                        });
                        if (datas.size() > 5)
                        {
                            datas.pop_back();
                        }
                        std::ofstream o("record.json");
                        o << std::setw(4) << "{\"data\" :[";
                        for (auto &item : datas)
                        {
                            json k(item);
                            if (datas.back() == item)
                            {
                                o << k.dump(4);
                            }
                            else
                            {
                                o << k.dump(4) << ",";
                            }
                        }
                        o << "] }" << std::endl;
                        name.clear();
                        name = "Input your name";
                        Game::Reset();
                        Mix_PlayMusic(Game::assets->GetBGM("menu"),-1);
                        Game::isStart = false;
                    }
                    break;
                    case BackToMenu:
                        name = "Input your name";
                        Game::Reset();
                        Mix_PlayMusic(Game::assets->GetBGM("menu"), -1);
                        Game::isStart = false;
                        break;
                    case Exit:
                        Game::isRunning = false;
                        break;
                    }
                }
            }
        }
    }
    SDL_Delay(50);
}

void Lostscreen::draw()
{
    SDL_SetRenderDrawBlendMode(Game::renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(Game::renderer, 0, 0, 0, 230);
    SDL_RenderFillRect(Game::renderer, &winsize);
    SDL_RenderCopy(Game::renderer, inputtex, NULL, &inputRect);
    SDL_RenderCopy(Game::renderer, headertex, NULL, &headerRect);
    for (auto &b : buttons)
    {
        b->draw();
    }
}