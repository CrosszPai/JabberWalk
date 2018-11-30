#include "Leaderboard.h"

void to_json(json &j, const data *d)
{
    j = json{
        {"name", d->name},
        {"score", d->score}};
}
void from_json(const json &j, data *d)
{
    j.at("name").get_to(d->name);
    j.at("score").get_to(d->score);
}

Leaderboard::Leaderboard()
{
    boardtex = Game::assets->GetTexture("leaderboard");
    SDL_QueryTexture(boardtex, nullptr, nullptr, &boardRect.w, &boardRect.h);
    boardRect = {(800 - boardRect.w) / 2, (640 - boardRect.h) / 2, boardRect.w, boardRect.h};
    SDL_Color white = {250, 250, 250, 250};
    SDL_Surface *hs = TTF_RenderText_Blended(Game::assets->GetFont("arcadefont"), "LEADERBOARD", white);
    headertex = SDL_CreateTextureFromSurface(Game::renderer, hs);
    SDL_FreeSurface(hs);
    SDL_QueryTexture(headertex, nullptr, nullptr, &headerRect.w, &headerRect.h);
    headerRect.x = boardRect.x + boardRect.w / 2 - headerRect.w / 2;
    headerRect.y = boardRect.y + 32;

    std::ifstream i("record.json");
    json j;
    i >> j;

    for (auto &elm : j["data"])
    {
        datas.push_back(new data(elm["name"], elm["score"]));
    }
    std::sort(datas.begin(), datas.end(), [](const auto &ldata, const auto &rdata) {
        return ldata->score > rdata->score;
    });
    float u = 1;
    for (auto &item : datas)
    {
        std::cout << "name :" << item->name << " score :" << item->score << std::endl;
        char buffer[200];
        std::snprintf(buffer, 200, "%-10s%35s", item->name.c_str(), std::to_string(item->score).c_str());
        std::cout << buffer << "\n";
        SDL_Surface *surf = TTF_RenderText_Solid(Game::assets->GetFont("arcadefont"), buffer, white);
        SDL_Texture *t = SDL_CreateTextureFromSurface(Game::renderer, surf);
        SDL_FreeSurface(surf);
        list.push_back(new listRender(t, u, boardRect.x + 64, boardRect.y + 100));
        u += 0.5;
    }
    u = 1;
}
void Leaderboard::update()
{
    {
        std::ifstream i("record.json");
        json j;
        i >> j;
        SDL_Color white = {255, 255, 255, 255};
        datas.clear();
        list.clear();
        for (auto &elm : j["data"])
        {
            datas.push_back(new data(elm["name"], elm["score"]));
        }
        std::sort(datas.begin(), datas.end(), [](const auto &ldata, const auto &rdata) {
            return ldata->score > rdata->score;
        });
        float u = 1;
        for (auto &item : datas)
        {
            std::cout << "name :" << item->name << " score :" << item->score << std::endl;
            char buffer[200];
            std::snprintf(buffer, 200, "%-10s%35s", item->name.c_str(), std::to_string(item->score).c_str());
            std::cout << buffer << "\n";
            SDL_Surface *surf = TTF_RenderText_Solid(Game::assets->GetFont("arcadefont"), buffer, white);
            SDL_Texture *t = SDL_CreateTextureFromSurface(Game::renderer, surf);
            SDL_FreeSurface(surf);
            list.push_back(new listRender(t, u, boardRect.x + 64, boardRect.y + 100));
            u += 0.5;
        }
        u = 1;
    }
}

void Leaderboard::draw()
{
    SDL_RenderCopy(Game::renderer, boardtex, NULL, &boardRect);
    SDL_RenderCopy(Game::renderer, headertex, NULL, &headerRect);
    for (auto &item : list)
    {
        SDL_RenderCopy(Game::renderer, item->tex, NULL, &item->rec);
    }
}

Leaderboard::~Leaderboard()
{
}
