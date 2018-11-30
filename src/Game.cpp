#include "Game.h"
#include "TextureManager.h"
#include "ECS/Components.h"
#include "Vector2D.h"
#include "Collision.h"
#include <sstream>

Manager manager;

SDL_Rect barrierRect = {0, 0, 128, 128};
SDL_Rect specialRect = {800, 0, 400, 640};
SDL_Color white = {255, 255, 255, 255};
SDL_Color green = {0, 255, 0, 255};
SDL_Color blue = {0, 0, 255, 255};
SDL_Color red = {255, 0, 0, 255};
SDL_Color gold = {207, 181, 59, 255};
SDL_Color silver = {192, 192, 192, 255};

SDL_Renderer *Game::renderer = nullptr;
SDL_Event Game::event;

AssetManager *Game::assets = new AssetManager(&manager);

bool Game::isRunning = false;
bool Game::isStart = false;
float Game::hp = 0;
float Game::mp = 0;
float Game::maxhp = 0;
float Game::maxmp = 0;
float Game::shield = 0;
float Game::attack = 0;
int Game::combo = 0;
int Game::exp = 0;
long Game::score = 0;
bool Game::isdef = false;
bool Game::pause = false;
bool Game::lost = false;
uint_fast16_t Game::combomultiply = 1;
uint32_t Game::time = 0;
uint32_t Game::timebuffer = 0;
uint32_t Game::combotime = 0;
uint32_t Game::specialTime = 0;
bool Game::isSpecial = false;
SDL_Rect Game::camera = {0, 0, 400, 640};
int Game::level = 1;
SDL_Texture *combotex;
SDL_Rect combotexRect = {650, 40, 0, 0};
SDL_Texture *scoretex;
SDL_Rect scoretexRect = {450, 20, 0, 0};
auto &player(manager.addEntity());

Game::Game()
{
}

Game::~Game()
{
}

void Game::init(const char *title, int width, int height, bool fullscreen)
{
	int flags = 0;

	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer)
		{
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		}
		SDL_Surface *tmpsurf = IMG_Load("icon.ico");
		SDL_SetWindowIcon(window, tmpsurf);
		SDL_FreeSurface(tmpsurf);

		isRunning = true;
	}
	if (TTF_Init() == -1)
	{
		std::cout << "Error: SDL_TTF" << std::endl;
	}
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		std::cout << "Error: SDL_mixer" << std::endl;
	}

	assets->AddFont("arcadefont", "assets/font/ARCADECLASSIC.TTF", 24);
	assets->AddFont("combofont", "assets/font/ARCADECLASSIC.TTF", 100);

	assets->AddTexture("yuto", "assets/sprite/yuto.png");
	assets->AddTexture("statBar", "assets/hud/stat_bar.png");
	assets->AddTexture("hpBar", "assets/hud/hp.png");
	assets->AddTexture("mpBar", "assets/hud/mana.png");
	assets->AddTexture("land1", "assets/bg/land1.png");
	assets->AddTexture("land2", "assets/bg/land2.png");
	assets->AddTexture("land3", "assets/bg/land3.png");
	assets->AddTexture("sky1", "assets/bg/sky1.png");
	assets->AddTexture("sky2", "assets/bg/sky2.png");
	assets->AddTexture("sky3", "assets/bg/sky3.png");
	assets->AddTexture("tree1", "assets/bg/particle/tree1.png");
	assets->AddTexture("building2", "assets/bg/particle/building2.png");
	assets->AddTexture("grass2", "assets/bg/particle/grass2.png");
	assets->AddTexture("tree2", "assets/bg/particle/tree2.png");
	assets->AddTexture("building3", "assets/bg/particle/building3.png");
	assets->AddTexture("grass3", "assets/bg/particle/grass3.png");
	assets->AddTexture("tree3", "assets/bg/particle/tree3.png");
	assets->AddTexture("building1", "assets/bg/particle/building1.png");
	assets->AddTexture("grass1", "assets/bg/particle/grass1.png");
	assets->AddTexture("slash1", "assets/sprite/slash_1.png");
	assets->AddTexture("slash2", "assets/sprite/slash_2.png");
	assets->AddTexture("slash3", "assets/sprite/slash_3.png");
	assets->AddTexture("Barrier", "assets/sprite/Barrier.png");
	assets->AddTexture("triggerbreak", "assets/sprite/TriggerBreak.png");
	assets->AddTexture("dashbreak", "assets/sprite/DashBreak.png");
	assets->AddTexture("airbreak1", "assets/sprite/AirBreak1.png");
	assets->AddTexture("airbreak2", "assets/sprite/AirBreak2.png");
	assets->AddTexture("airbreak3", "assets/sprite/AirBreak3.png");
	assets->AddTexture("airbreak4", "assets/sprite/AirBreak4.png");
	assets->AddTexture("bg", "assets/bg/startmenu.png");
	assets->AddTexture("cursor", "assets/hud/cursor.png");
	assets->AddTexture("EXITBTN", "assets/hud/Exit.png");
	assets->AddTexture("LDBBTN", "assets/hud/Leaderboard.png");
	assets->AddTexture("STARTBTN", "assets/hud/start.png");
	assets->AddTexture("leaderboard", "assets/hud/Board.png");
	assets->AddTexture("plus", "assets/hud/plusbutton.png");
	assets->AddTexture("range", "assets/sprite/range_mon.png");
	assets->AddTexture("melee", "assets/sprite/melee_mon.png");
	assets->AddTexture("elite", "assets/sprite/elite_mon.png");
	assets->AddTexture("elite_arrow", "assets/sprite/elite_arrow.png");
	assets->AddTexture("range_ball", "assets/sprite/range_ball.png");
	assets->AddTexture("special", "assets/hud/special.png");

	assets->AddBGM("stage1", "assets/bgm/stage1.wav");
	assets->AddBGM("stage2", "assets/bgm/stage2.wav");
	assets->AddBGM("stage3", "assets/bgm/stage3.wav");
	assets->AddBGM("menu", "assets/bgm/menu.wav");
	assets->AddBGM("lost", "assets/bgm/lost.wav");

	assets->AddSFX("airbreak", "assets/bgm/airbreak.wav");
	assets->AddSFX("dashbreak", "assets/bgm/dashbreak.wav");
	assets->AddSFX("triggerbreak", "assets/bgm/triggerbreak.wav");
	assets->AddSFX("effect", "assets/bgm/effect.wav");
	assets->AddSFX("slash", "assets/bgm/slash.wav");

	SDL_SetWindowTitle(window, "JabberWalk");

	Background *gLand, *gLand1, *gSky, *gSky1;
	gLand = new Background("land", 0, 500, 900, 200, -10);
	gLand1 = new Background("land", Game::camera.w, 500, 900, 200, -10);
	gSky = new Background("sky", 0, 0, 900, 600, -5);
	gSky1 = new Background("sky", Game::camera.w, 0, 900, 600, -5);
	backgrounds.push_back(gSky);
	backgrounds.push_back(gSky1);
	backgrounds.push_back(gLand);
	backgrounds.push_back(gLand1);

	player.addComponent<TransformComponent>(100, 400, 64, 64, 2);
	player.addComponent<ControlledComponent>("yuto", true);
	player.addComponent<KeyboardController>();
	player.addComponent<ColliderComponent>("player");
	player.addGroup(groupPlayers);

	hp = maxhp = 500;
	mp = 50;
	maxmp = 50;
	shield = 20;
	attack = 30;

	hud = new HUD();
	menu = new StartMenu();
	pWindow = new PauseWindow();
	lScreen = new Lostscreen();
	Mix_VolumeMusic(60);

	Mix_PlayMusic(assets->GetBGM("menu"), -1);
	timebuffer = SDL_GetTicks();
	std::srand(std::time(nullptr));
}
auto &players(manager.getGroup(Game::groupPlayers));
auto &colliders(manager.getGroup(Game::groupColliders));
auto &projectiles(manager.getGroup(Game::groupProjectiles));
auto &trees(manager.getGroup(Game::ParticleType1));
auto &grasses(manager.getGroup(Game::ParticleType2));
auto &buildings(manager.getGroup(Game::ParticleType3));
auto &effects(manager.getGroup(Game::Effect));
auto &def(manager.getGroup(Game::Defend));
auto &enemies(manager.getGroup(Game::groupEnemies));

void Game::handleEvents()
{

	SDL_PollEvent(&event);

	switch (event.type)
	{
	case SDL_QUIT:
		isRunning = false;
		break;
	default:
		break;
	}
}

void Game::update()
{
	if (isStart && !pause && !lost && !isSpecial)
	{
		if (Game::mp > Game::maxmp)
		{
			Game::mp = Game::maxmp;
		}
		if (SDL_GetTicks() - timebuffer >= 1000)
		{

			time++;
			score += time * level;
			timebuffer = SDL_GetTicks();
			if (Game::mp < Game::maxmp)
			{
				Game::mp += 1;
			}
			exp += 10;
			std::string scoretext;
			scoretext = "score      " + std::to_string(score);
			SDL_Surface *ssurf = TTF_RenderText_Blended(assets->GetFont("arcadefont"), scoretext.c_str(), {255, 255, 255, 255});
			scoretex = SDL_CreateTextureFromSurface(renderer, ssurf);
			SDL_FreeSurface(ssurf);
			SDL_QueryTexture(scoretex, nullptr, nullptr, &scoretexRect.w, &scoretexRect.h);

			SDL_Surface *csurf;
			switch (combo)
			{
			case D:
				csurf = TTF_RenderText_Blended(assets->GetFont("combofont"), "D", white);
				break;
			case C:
				csurf = TTF_RenderText_Blended(assets->GetFont("combofont"), "C", green);
				break;
			case B:
				csurf = TTF_RenderText_Blended(assets->GetFont("combofont"), "B", blue);
				break;
			case A:
				csurf = TTF_RenderText_Blended(assets->GetFont("combofont"), "A", red);
				break;
			case S:
				csurf = TTF_RenderText_Blended(assets->GetFont("combofont"), "S", gold);
				break;
			case SS:
				csurf = TTF_RenderText_Blended(assets->GetFont("combofont"), "SS", silver);
				break;
			}
			combotex = SDL_CreateTextureFromSurface(renderer, csurf);
			SDL_FreeSurface(csurf);
			SDL_QueryTexture(combotex, nullptr, nullptr, &combotexRect.w, &combotexRect.h);
		}
		if (SDL_GetTicks() - combotime >= 3000)
		{
			combo--;
			SDL_SetWindowTitle(window, "JabberWalk control = j:attack k:defend w:jump s:dash j+k or L-Rmouse:charge");
			combotime = SDL_GetTicks();
		}
		if (score >= 300000 && level == 1)
		{
			levelup();
		}
		else if (score >= 1000000 && level == 2)
		{
			levelup();
		}
		assets->AutoGenerate(ParticleType1);
		assets->AutoGenerate(ParticleType2);
		assets->AutoGenerate(ParticleType3);
		assets->AutoGenerate(Range);
		assets->AutoGenerate(Melee);
		assets->AutoGenerate(Elite);

		for (auto &bg : backgrounds)
		{
			bg->update();
		}
		hud->update();

		manager.refresh();
		manager.update();

		SDL_Rect playerCol = player.getComponent<ColliderComponent>().collider;
		Vector2D playerPos = player.getComponent<TransformComponent>().position;
		Vector2D playerVel = player.getComponent<TransformComponent>().velocity;

		for (auto &b : buildings)
		{
			b->getComponent<TransformComponent>().velocity.x = b->getComponent<TransformComponent>().velocity.x;
			if (b->getComponent<TransformComponent>().position.x < camera.x - b->getComponent<TransformComponent>().width * b->getComponent<TransformComponent>().scale)
			{
				b->destroy();
			}
		}

		for (auto &t : trees)
		{
			t->getComponent<TransformComponent>().velocity.x = t->getComponent<TransformComponent>().velocity.x;
			if (t->getComponent<TransformComponent>().position.x < camera.x - t->getComponent<TransformComponent>().width * t->getComponent<TransformComponent>().scale)
			{
				t->destroy();
			}
		}
		for (auto &g : grasses)
		{
			g->getComponent<TransformComponent>().velocity.x = g->getComponent<TransformComponent>().velocity.x;
			if (g->getComponent<TransformComponent>().position.x < camera.x - g->getComponent<TransformComponent>().width * g->getComponent<TransformComponent>().scale)
			{
				g->destroy();
			}
		}
		for (auto &c : colliders)
		{
		}
		for (auto &p : projectiles)
		{
			if (Collision::AABB(player.getComponent<ColliderComponent>().collider, p->getComponent<ColliderComponent>().collider) && player.getComponent<ControlledComponent>().ActionState != player.getComponent<ControlledComponent>().DASH)
			{
				p->destroy();
				hp -= p->getComponent<ColliderComponent>().damage;
				if (isdef && p->getComponent<ColliderComponent>().damage != 0)
				{
					if (shield > p->getComponent<ColliderComponent>().damage)
					{
						hp += shield - p->getComponent<ColliderComponent>().damage;
					}
				}
				p->getComponent<ColliderComponent>().damage = 0;
			}
		}
		for (auto &e : effects)
		{
		}
		for (auto &e : enemies)
		{
			if (e->getComponent<TransformComponent>().position.x +
					e->getComponent<TransformComponent>().width <=
				0)
			{
				e->destroy();
			}
			if (Collision::AABB(player.getComponent<ColliderComponent>(), e->getComponent<ColliderComponent>()))
			{
				hp -= e->getComponent<BehaviorComponent>().attack;
				if (isdef && e->getComponent<BehaviorComponent>().attack != 0)
				{
					if (shield > e->getComponent<BehaviorComponent>().attack)
					{
						hp += shield - e->getComponent<BehaviorComponent>().attack;
					}
				}
				e->getComponent<BehaviorComponent>().attack = 0;
			}
			for (auto &eff : effects)
			{
				if (Collision::AABB(e->getComponent<ColliderComponent>(), eff->getComponent<ColliderComponent>()))
				{
					if (eff->getComponent<ColliderComponent>().tag == "slash")
					{
						e->getComponent<BehaviorComponent>().Attacked(attack);
					}
					if (eff->getComponent<ColliderComponent>().tag == "AB")
					{
						e->getComponent<BehaviorComponent>().Attacked(1.5 * attack);
					}
					if (eff->getComponent<ColliderComponent>().tag == "TB")
					{
						e->getComponent<BehaviorComponent>().Attacked(2.5 * attack);
					}
					if (eff->getComponent<ColliderComponent>().tag == "DB")
					{
						e->getComponent<BehaviorComponent>().Attacked(4.0 * attack);
					}
				}
			}
		}

		if (hp > 0)
		{
			if (playerPos.x >= camera.w)
			{
				player.getComponent<TransformComponent>().velocity.x = -30;
			}
			if (playerVel.x < 0 &&
				playerPos.x <= player.getComponent<ControlledComponent>().temppos.x)
			{
				playerPos.x =
					player.getComponent<ControlledComponent>().temppos.x;
				player.getComponent<TransformComponent>().velocity.Zero();
			}
		}
		if (hp <= 0)
		{
			player.getComponent<TransformComponent>().velocity.x = -10;
			if (playerPos.x < 0 - player.getComponent<TransformComponent>().width)
			{
				SDL_StartTextInput();
				lost = true;
				Mix_HaltMusic();
				Mix_PlayMusic(assets->GetBGM("lost"), -1);
			}
		}
		if (isdef)
		{
			barrierRect.x = playerPos.x + player.getComponent<TransformComponent>().width + 32;
			barrierRect.y = playerPos.y + 24;
			barrierRect.w = barrierRect.h = 96;
		}
		if (combo > 5)
		{
			combo = 5;
		}
		else if (combo < 0)
		{
			combo = 0;
		}
		if (exp >= 500)
		{
			exp = 0;

			attack += 5;
			maxhp += 10;
			maxmp += 5;
			shield += 3;
			hp += 0.4 * maxhp;
			mp += 0.2 * maxhp;
			SDL_SetWindowTitle(window, "Level UP!");
		}
		if (hp > maxhp)
		{
			hp = maxhp;
		}
		if (mp > maxmp)
		{
			mp = maxmp;
		}
		switch (combo)
		{
		case D:
			combomultiply = 1;
			break;
		case C:
			combomultiply = 2;
			break;
		case B:
			combomultiply = 3;
			break;
		case A:
			combomultiply = 4;
			break;
		case S:
			combomultiply = 5;
			break;
		case SS:
			combomultiply = 8;
			break;
		}
	}
	else if (!isStart)
	{
		menu->update();
	}
	else if (pause)
	{
		pWindow->update();
	}
	else if (lost)
	{
		lScreen->update();
	}
	else if (isSpecial)
	{
		if (SDL_GetTicks() - specialTime <= 1000 && specialRect.x + specialRect.w > 800)
		{
			specialRect.x -= 50;
		}
		if (SDL_GetTicks() - specialTime >= 1500 && specialRect.x <= 800)
		{
			specialRect.x += 50;
			if (specialRect.x == 800)
			{
				isSpecial = false;
				for (auto &e : effects)
				{
					if (e->getComponent<ColliderComponent>().tag == "TB")
					{
						Mix_PlayChannel(-1, assets->GetSFX("triggerbreak"), 0);
					}
					if (e->getComponent<ColliderComponent>().tag == "DB")
					{
						Mix_PlayChannel(-1, assets->GetSFX("dashbreak"), 0);
					}
				}
			}
		}
	}
}

void Game::render()
{
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

	if (isStart)
	{
		for (auto &bg : backgrounds)
		{
			bg->draw();
		}
		hud->draw();
		SDL_RenderCopy(renderer, combotex, NULL, &combotexRect);
		SDL_RenderCopy(renderer, scoretex, NULL, &scoretexRect);
		for (auto &b : buildings)
		{
			b->draw();
		}
		for (auto &t : trees)
		{
			t->draw();
		}
		for (auto &g : grasses)
		{
			g->draw();
		}
		for (auto &c : colliders)
		{
			c->draw();
		}
		for (auto &p : projectiles)
		{
			p->draw();
		}
		for (auto &e : enemies)
		{
			e->draw();
		}
		for (auto &p : players)
		{
			p->draw();
		}
		for (auto &e : effects)
		{
			e->draw();
		}
		if (isdef)
		{
			SDL_RenderCopy(renderer, assets->GetTexture("Barrier"), NULL, &barrierRect);
		}

		for (auto &p : projectiles)
		{
			p->draw();
		}
		if (pause)
		{
			pWindow->draw();
		}
		if (lost)
		{
			lScreen->draw();
		}
		if (isSpecial)
		{
			SDL_RenderCopy(renderer, assets->GetTexture("special"), NULL, &specialRect);
		}
	}
	else
	{
		menu->draw();
	}
	SDL_RenderPresent(renderer);
}

void Game::clean()
{
	trees.clear();
	grasses.clear();
	buildings.clear();
	effects.clear();
	backgrounds.clear();
	assets->FreeBGM();
	assets->FreeSFX();
	delete menu;
	delete pWindow;
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	Mix_Quit();
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}

void Game::levelup()
{
	level++;
	Mix_HaltMusic();
	Mix_PlayMusic(assets->GetBGM("stage" + std::to_string(level)), -1);
}

void Game::Reset()
{
	trees.clear();
	grasses.clear();
	buildings.clear();
	effects.clear();
	enemies.clear();
	projectiles.clear();

	lost = false;
	level = 1;
	score = 0;
	exp = 0;
	time = 0;
	hp = maxhp = 500;
	mp = 50;
	maxmp = 50;
	shield = 20;
	player.getComponent<TransformComponent>().position.x = 100;
	player.getComponent<TransformComponent>().position.y = 400;
	Mix_HaltMusic();
}