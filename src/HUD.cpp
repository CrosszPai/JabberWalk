#include "HUD.h"
#include "TextureManager.h"

HUD::HUD()
{
    hpsrcRect = {0, 0, 61, 2};
    mpsrcRect = {0, 0, 42, 1};
    bartex = Game::assets->GetTexture("statBar");
    hptex = Game::assets->GetTexture("hpBar");
    mptex = Game::assets->GetTexture("mpBar");
    barsrcR = {0, 0, 64, 9};
    bardestR = {0, 20, 64 * 6, 9 * 6};
    hpdestRect.x = bardestR.x;
    hpdestRect.y = bardestR.y + 12;
    hpdestRect.w = hpsrcRect.w * 5.75;
    hpdestRect.h = hpsrcRect.h * 6;
    mpdestRect.x = bardestR.x;
    mpdestRect.y = bardestR.y + 40;
    mpdestRect.w = mpsrcRect.w * 5.75;
    mpdestRect.h = mpsrcRect.h * 6;
}
HUD::~HUD() {}

void HUD::update()
{
    hpdestRect.w = hpsrcRect.w * 5.75 * (Game::hp / Game::maxhp);
    mpdestRect.w = mpsrcRect.w * 5.75 * (Game::mp / Game::maxmp);
}
void HUD::draw()
{
    TextureManager::Draw(bartex, barsrcR, bardestR, SDL_FLIP_NONE);
    TextureManager::Draw(hptex, hpsrcRect, hpdestRect, SDL_FLIP_NONE);
    TextureManager::Draw(mptex, mpsrcRect, mpdestRect, SDL_FLIP_NONE);
}