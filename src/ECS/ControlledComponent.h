#pragma once

#include "ECS.h"
#include "TransformComponent.h"
#include "SDL2/SDL.H"
#include "../TextureManager.h"
#include "Animation.h"
#include <map>
#include <iostream>
#include "../AssetManager.h"

class ControlledComponent : public Component
{
  private:
    TransformComponent *transform;
    SDL_Texture *texture;

    bool animated = false;
    int frames = 0;
    int speed = 100;

  public:
    SDL_Rect srcRect, destRect;
    int animIndex = 0;
    bool isActioning = false;
    int AttackMode = ATTACK_MODE1;
    int ActionState = IDLE;
    int animFrame = 0;
    uint32_t animTime = 0;
    bool playing = false;
    bool fallen = false;
    Vector2D temppos;
    enum
    {
        IDLE,
        WALK,
        ATTACK,
        ATTACK_MODE1,
        ATTACK_MODE2,
        ATTACK_MODE3,
        JUMP,
        FALL,
        SPECIAL,
        CHARGE,
        DEF,
        DASH

    };

    std::map<std::string, Animation> animations;

    SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

    ControlledComponent() = default;
    ControlledComponent(std::string id)
    {
        setTex(id);
    }

    ControlledComponent(std::string id, bool isAnimated)
    {
        animated = isAnimated;

        Animation idle = Animation(0, 3, 100);
        Animation walk = Animation(1, 5, 100);
        Animation attack = Animation(2, 5, 100);
        Animation attack2 = Animation(3, 5, 100);
        Animation attack3 = Animation(4, 5, 100);
        Animation jump = Animation(5, 4, 50);
        Animation fall = Animation(6, 1, 300);
        Animation dash = Animation(7, 3, 100);
        Animation trigger_break = Animation(8, 2, 100);
        Animation air_attack = Animation(9, 2, 200);
        Animation air_break = Animation(10, 4, 100);
        Animation charge = Animation(11, 5, 100);
        Animation dash_break = Animation(12, 4, 200);

        animations.emplace("Idle", idle);
        animations.emplace("Walk", walk);
        animations.emplace("Attack", attack);
        animations.emplace("Attack2", attack2);
        animations.emplace("Attack3", attack3);
        animations.emplace("Dash", dash);
        animations.emplace("Jump", jump);
        animations.emplace("Fall", fall);
        animations.emplace("TriggerBreak", trigger_break);
        animations.emplace("AirAttack", air_attack);
        animations.emplace("Charge", charge);
        animations.emplace("DashBreak", dash_break);
        animations.emplace("AirBreak", air_break);

        Play("Walk");
        ActionState = WALK;

        setTex(id);
    }

    ~ControlledComponent()
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
        temppos.x = transform->position.x;
        temppos.y = transform->position.y;
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
            if (animFrame > frames - 1)
            {
                if (ActionState == ATTACK)
                {
                    switch (AttackMode)
                    {
                    case ATTACK_MODE1:
                        AttackMode = ATTACK_MODE2;
                        break;
                    case ATTACK_MODE2:
                        AttackMode = ATTACK_MODE3;
                        break;
                    case ATTACK_MODE3:
                        AttackMode = ATTACK_MODE1;
                        break;
                    }
                    Play("Walk");
                    ActionState = WALK;
                }
                playing = false;
                animFrame = 0;
            }
            srcRect.x = srcRect.w * animFrame;
        }
        srcRect.y = animIndex * transform->height;

        destRect.x = static_cast<int>(transform->position.x);
        destRect.y = static_cast<int>(transform->position.y);
        destRect.w = transform->width * transform->scale;
        destRect.h = transform->height * transform->scale;

        if (transform->position.y == temppos.y && fallen)
        {
            transform->velocity.y = 0;
            fallen = false;
            transform->position.y = temppos.y;
            ActionState = WALK;
        }
        if (transform->position.y + 3.5 * transform->height <= temppos.y && !fallen)
        {
            transform->velocity.y = 15;
            if (ActionState != ATTACK)
            {

                ActionState = FALL;
                Play("Fall");
                playing = true;
            }
            fallen = true;
        }
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