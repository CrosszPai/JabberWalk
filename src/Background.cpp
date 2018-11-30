#include "Background.h"

Background::Background(std::string id, int xpos, int ypos, int w, int h, int sp) : type(id), speed(sp)
{
    destRect = {xpos, ypos, w, h};
    srcRect = {0, 0, w, h};
}

Background::~Background() {}

void Background::update()
{
    tex = Game::assets->GetTexture(type + std::to_string(Game::level));
    if (destRect.x == -Game::camera.w)
    {
        destRect.x = Game::camera.w;
    }

    destRect.x += speed;
    //speed *= Game::level;
}

void Background::draw()
{
    TextureManager::Draw(tex, srcRect, destRect, SDL_FLIP_NONE);
}