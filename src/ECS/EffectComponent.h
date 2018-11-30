#pragma once

#include "ECS.h"
#include "TransformComponent.h"
#include "SDL2/SDL.H"
#include "../TextureManager.h"
#include "Animation.h"

class EffectComponent : public Component
{
  private:
    TransformComponent *transform;
    SDL_Texture *texture;

    bool animated = false;
    int frames = 0;
    int speed = 100;
    uint32_t animTime = 0;

  public:
    SDL_Rect srcRect, destRect;
    int animIndex = 0;
    int animFrame = 0;

    std::map<std::string, Animation> animations;

    SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

    EffectComponent() = default;
    EffectComponent(std::string id)
    {
        setTex(id);
    }

    EffectComponent(std::string id, bool isAnimated, int idf)
    {
        animated = isAnimated;

        Animation idle = Animation(0, idf, 600 / idf);
        animations.emplace("Idle", idle);

        Play("Idle");

        setTex(id);
    }

    ~EffectComponent()
    {
    }

    void setTex(std::string id)
    {
        texture = Game::assets->GetTexture(id);
    }

    void init() override
    {

        transform = &entity->getComponent<TransformComponent>();

        srcRect.x = srcRect.y = 0;
        srcRect.w = transform->width;
        srcRect.h = transform->height;
    }

    void update() override
    {

        if (animated)
        {
            if (SDL_GetTicks() - animTime >= speed)
            {
                animFrame++;
                animTime = SDL_GetTicks();
            }
            if (srcRect.x > srcRect.w * frames)
            {
                entity->destroy();
            }
            srcRect.x = srcRect.w * animFrame;
        }
        srcRect.y = animIndex * transform->height;

        destRect.x = static_cast<int>(transform->position.x);
        destRect.y = static_cast<int>(transform->position.y);
        destRect.w = transform->width * transform->scale;
        destRect.h = transform->height * transform->scale;
    }

    void draw() override
    {
        TextureManager::Draw(texture, srcRect, destRect, spriteFlip);
    }

    void Play(std::string animName)
    {
        frames = animations[animName].frames;
        animIndex = animations[animName].index;
        speed = animations[animName].speed;
    }
};