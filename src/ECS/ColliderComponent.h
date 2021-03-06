#pragma once
#include <string>
#include <SDL2/SDL.h>
#include "ECS.h"
#include "TransformComponent.h"
#include "../Game.h"
#include "../TextureManager.h"

class ColliderComponent : public Component
{
  public:
	SDL_Rect collider;
	std::string tag;
	float damage;	

	SDL_Texture *tex;

	TransformComponent *transform;

	ColliderComponent(std::string t)
	{
		tag = t;
	}
	ColliderComponent(std::string t,int dmg)
	{
		tag = t;
		damage= dmg;
	}
	ColliderComponent(std::string t, int xpos, int ypos, int size)
	{
		tag = t;
		collider.x = xpos;
		collider.y = ypos;
		collider.h = collider.w = size;
	}
	ColliderComponent(std::string t, int xpos, int ypos, int w, int h)
	{
		tag = t;
		collider.x = xpos;
		collider.y = ypos;
		collider.w = w;
		collider.h = h;
	}

	void init() override
	{
		if (!entity->hasComponent<TransformComponent>())
		{
			entity->addComponent<TransformComponent>();
		}

		transform = &entity->getComponent<TransformComponent>();
	}

	void update() override
	{
		collider.x = static_cast<int>(transform->position.x);
		collider.y = static_cast<int>(transform->position.y);
		collider.w = transform->width * transform->scale;
		collider.h = transform->height * transform->scale;
	}

  private:
};