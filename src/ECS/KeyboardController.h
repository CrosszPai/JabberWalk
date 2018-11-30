#pragma once

#include "../Game.h"
#include "ECS.h"
#include "TransformComponent.h"
#include "ControlledComponent.h"
#include "ColliderComponent.h"

class KeyboardController : public Component
{
  public:
	TransformComponent *transform;
	ControlledComponent *sprite;
	ColliderComponent *coll;
	int click;
	bool hold;

	const Uint8 *currentKeyStates = SDL_GetKeyboardState(NULL);

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		sprite = &entity->getComponent<ControlledComponent>();
		combotime = SDL_GetTicks();
	}

	void update() override
	{

		if (Game::event.type == SDL_MOUSEBUTTONDOWN && !hold)
		{
			hold = true;
		}
		if (Game::event.type == SDL_MOUSEBUTTONUP && hold)
		{
			hold = false;
			click++;
		}
		if (Game::event.type == SDL_KEYDOWN)
		{
			switch (Game::event.key.keysym.sym)
			{
			case SDLK_a:
				keycombo += "a";
				break;
			case SDLK_w:
				keycombo += "w";
				break;
			case SDLK_s:
				keycombo += "s";
				break;
			case SDLK_d:
				keycombo += "d";
				break;
			case SDLK_j:
				keycombo += "j";
				break;
			case SDLK_k:
				keycombo += "k";
				break;
			}
			std::cout << keycombo << std::endl;
		}
		if (Game::event.type == SDL_KEYDOWN && currentKeyStates[SDL_SCANCODE_J] && transform->position.y < sprite->temppos.y && sprite->ActionState != sprite->ATTACK)
		{
			sprite->Play("AirAttack");
			sprite->ActionState = sprite->ATTACK;
			sprite->playing = true;
			std::cout << "Air Attack" << std::endl;
			Game::assets->CreateEffect("slash3", transform->position, "slash");
		}
		if (Game::event.type == SDL_KEYDOWN && !sprite->playing)
		{

			if (currentKeyStates[SDL_SCANCODE_J] && transform->position.y == sprite->temppos.y && sprite->ActionState != sprite->CHARGE && sprite->ActionState != sprite->DEF)
			{
				sprite->animFrame = 0;
				if (!(keycombo == "ddj" || keycombo == "jwk" || keycombo == "jsk"))
				{
					if (sprite->AttackMode == sprite->ATTACK_MODE1)
					{
						sprite->Play("Attack");
						Mix_PlayChannel(-1, Game::assets->GetSFX("slash"), 0);
						Game::assets->CreateEffect("slash1", transform->position, "slash");
					}
					if (sprite->AttackMode == sprite->ATTACK_MODE2)
					{
						sprite->Play("Attack2");
						Mix_PlayChannel(-1, Game::assets->GetSFX("slash"), 0);
						Game::assets->CreateEffect("slash2", transform->position, "slash");
					}
					if (sprite->AttackMode == sprite->ATTACK_MODE3)
					{
						sprite->Play("Attack3");
						Mix_PlayChannel(-1, Game::assets->GetSFX("slash"), 0);
						Game::assets->CreateEffect("slash3", transform->position, "slash");
					}
				}
				sprite->playing = true;
				sprite->ActionState = sprite->ATTACK;
			}
			if (currentKeyStates[SDL_SCANCODE_D])
			{
				sprite->Play("Walk");
			}
			if (currentKeyStates[SDL_SCANCODE_ESCAPE] && Game::isStart)
			{
				Game::pause = true;
				SDL_Delay(300);
			}
			if (currentKeyStates[SDL_SCANCODE_W] && sprite->ActionState != sprite->JUMP && transform->position.y == sprite->temppos.y)
			{
				sprite->animFrame = 0;
				sprite->Play("Jump");
				transform->velocity.y = -15;
				sprite->playing = true;
				sprite->ActionState = sprite->JUMP;
			}
		}
		if (!currentKeyStates[SDL_SCANCODE_J] && !sprite->playing)
		{
			sprite->Play("Walk");
		}
		if (!currentKeyStates[SDL_SCANCODE_W] && !sprite->playing)
		{
		}
		if (currentKeyStates[SDL_SCANCODE_S])
		{
			transform->position.y = sprite->temppos.y;
			transform->velocity.Zero();
			sprite->Play("Dash");
			sprite->ActionState = sprite->DASH;
			sprite->playing = true;
		}
		if (keycombo == "ddj" && Game::mp >= 30)
		{
			Game::isSpecial = true;
			Game::specialTime = SDL_GetTicks();
			Game::assets->CreateExplosion("triggerbreak", transform->position, "TB");
			sprite->Play("TriggerBreak");
			transform->velocity.x = 30;
			Game::mp -= 30;
			keycombo.clear();
			sprite->ActionState = sprite->SPECIAL;
		}
		if (keycombo == "kwj" && Game::mp >= 10)
		{
			Game::assets->CreateExplosion("airbreak", transform->position, "AB");
			sprite->Play("AirBreak");
			Game::mp -= 10;
			keycombo.clear();
			sprite->ActionState = sprite->SPECIAL;
		}
		if (keycombo == "ksj" && Game::mp >= 40)
		{
			Game::isSpecial = true;
			Game::specialTime = SDL_GetTicks();
			Game::assets->CreateExplosion("dashbreak", transform->position, "DB");
			sprite->Play("DashBreak");
			Game::mp -= 40;
			keycombo.clear();
			sprite->ActionState = sprite->SPECIAL;
		}
		if (SDL_GetTicks() - combotime > 1000)
		{
			keycombo.clear();
			combotime = SDL_GetTicks();
			click = 0;
		}
		if (currentKeyStates[SDL_SCANCODE_J] && currentKeyStates[SDL_SCANCODE_K] && sprite->ActionState == sprite->WALK)
		{
			sprite->Play("Charge");
			sprite->ActionState = sprite->CHARGE;
		}
		if (sprite->ActionState == sprite->CHARGE)
		{

			Game::mp += 0.1;
		}
		if (currentKeyStates[SDL_SCANCODE_K] && sprite->ActionState == sprite->WALK)
		{
			sprite->ActionState = sprite->DEF;
			Game::isdef = true;
		}
		if (Game::event.type == SDL_KEYUP && Game::event.key.keysym.sym == SDLK_k)
		{
			Game::isdef = false;
			sprite->ActionState = sprite->WALK;
		}
		if (click > 4)
		{
			Game::mp += 5;
			click = 0;
		}
		if (click > 0)
		{
			sprite->Play("Charge");
		}
	}

  private:
	std::string keycombo;
	uint32_t combotime;
};