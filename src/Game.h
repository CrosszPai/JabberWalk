#pragma once

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
#include <iostream>
#include <vector>
#include "AssetManager.h"
#include "Background.h"
#include "HUD.h"
#include <cstdlib>
#include <ctime>
#include "StartMenu.h"
#include "PauseWindow.h"
#include "Lostscreen.h"

class AssetManager;
class ColliderComponent;
class HUD;
class Background;
class StartMenu;
class PauseWindow;
class Lostscreen;

class Game
{
  public:
	Game();
	~Game();

	void init(const char *title, int width, int height, bool fullscreen);

	void handleEvents();
	void update();
	bool running() { return isRunning; }
	void render();
	void clean();

	std::vector<Background *> backgrounds;
	StartMenu *menu;
	PauseWindow *pWindow;
	Lostscreen *lScreen;
	HUD *hud;

	static void levelup();
	static void Reset();
	static SDL_Renderer *renderer;
	static SDL_Event event;
	static bool isRunning, isStart,isSpecial;
	static AssetManager *assets;
	static float hp, mp, maxhp, maxmp, shield;
	static bool isdef;
	static SDL_Rect camera;
	static uint32_t time,specialTime;
	static uint32_t timebuffer;
	static uint32_t combotime;
	static uint_fast16_t combomultiply;
	static long score;
	static bool pause;
	static bool lost;
	static int combo;
	static int exp;
	static int level;
	static float attack;

	enum groupLabels : std::size_t
	{
		groupPlayers,
		groupColliders,
		groupProjectiles,
		groupEnemies,
		ParticleType1,
		ParticleType2,
		ParticleType3,
		Effect,
		Defend,
		Range,
		Melee,
		Elite
	};

	enum combograde : unsigned short
	{
		D,
		C,
		B,
		A,
		S,
		SS
	};

	private : SDL_Window *window;
};