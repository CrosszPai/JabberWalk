#include "Button.h"

Button::Button(std::string type, int t, int xpos, int ypos)
{
    tex = Game::assets->GetTexture(type);
    destRect = {xpos, ypos, 168, 72};
    tag = t;
}

Button::Button(int t, std::string text, int xpos, int ypos)
{
    SDL_Color white = {255, 255, 255, 255};
    SDL_Surface *surf = TTF_RenderText_Blended(Game::assets->GetFont("arcadefont"), text.c_str(), white);
    tex = SDL_CreateTextureFromSurface(Game::renderer, surf);
    SDL_FreeSurface(surf);
    SDL_QueryTexture(tex, nullptr, nullptr, &destRect.w, &destRect.h);
    destRect.x = xpos - destRect.w / 2;
    destRect.y = ypos;
    tag = t;
}

Button::~Button() {}

void Button::update()
{
    
}
void Button::draw()
{
    SDL_RenderCopy(Game::renderer, tex, NULL, &destRect);
}