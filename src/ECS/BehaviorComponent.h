#pragma once

#include "../Game.h"
#include "Components.h"

class BehaviorComponent : public Component
{
  private:
    TransformComponent *transform;
    ColliderComponent *coll;
    int hitpoint, rattack;
    int tempspeed;
    int temppos;
    bool isTackle;
    std::string rangetype;

  public:
    BehaviorComponent(int hp, int atk, int ratk, std::string rtype) : hitpoint(hp), attack(atk), rattack(ratk), rangetype(rtype)
    {
    }
    ~BehaviorComponent() {}
    void init() override
    {
        transform = &entity->getComponent<TransformComponent>();
        coll = &entity->getComponent<ColliderComponent>();
    }
    void update() override
    {
        if (transform->position.x - temppos >= 200 && isTackle)
        {
            transform->velocity.x = tempspeed;
            isTackle = false;
        }
        if (transform->position.x >= 800)
        {
            if (coll->tag == "elite")
            {
                Game::score += 100 * 8 * Game::level * Game::combomultiply * 1.5;
                Game::exp += 30;
            }
            else if (coll->tag == "range")
            {
                Game::score += 100 * 2 * Game::level * Game::combomultiply * 1.5;
                Game::exp += 30;
            }
            else
            {
                Game::score += 100 * Game::level * Game::combomultiply * 1.5;
                Game::exp += 10;
            }
            entity->destroy();
        }
        if (hitpoint <= 0)
        {
            if (coll->tag == "elite")
            {
                Game::score += 100 * 8 * Game::level * Game::combomultiply;
                Game::exp += 30;
            }
            else if (coll->tag == "range")
            {
                Game::score += 100 * 2 * Game::level * Game::combomultiply;
                Game::exp += 20;
            }
            else
            {
                Game::score += 100 * Game::level * Game::combomultiply;
                Game::exp += 10;
            }
            entity->destroy();
            Game::combo++;
        }
        if (std::rand() % 1000 < 1 && rattack != 0)
        {
            shoot();
        }
    }
    void tackle(float dealerdmg)
    {
        if (dealerdmg > attack)
        {
            isTackle = true;
            temppos = transform->position.x;
            tempspeed = transform->velocity.x;
            transform->velocity.x = 10;
        }
    }
    void Attacked(float dmg)
    {
        hitpoint -= dmg;
        if (hitpoint > 0)
        {
            tackle(dmg);
        }
    }
    void shoot()
    {
        Game::assets->CreateProjectile(
            transform->position,
            Vector2D(
                transform->velocity.x * 5 * Game::level,
                0),
            800,
            transform->velocity.x * 5,
            coll->tag == "elite" ? (std::rand() % 2 == 1 ? rattack : 0.1 * Game::maxhp) : rattack,
            coll->tag + rangetype);
    }
    int attack;
};
