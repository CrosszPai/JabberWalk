#include "AssetManager.h"
///
#include "ECS/Components.h"
///
AssetManager::AssetManager(Manager *man) : manager(man)
{
}

AssetManager::~AssetManager()
{
}
///
void AssetManager::CreateProjectile(Vector2D pos, Vector2D vel, int range, int speed, int dmg, std::string id)
{
    auto &projectile(manager->addEntity());
    projectile.addComponent<TransformComponent>(pos.x, pos.y, 50, 50, 1);
    projectile.addComponent<SpriteComponent>(id, true, 6);
    projectile.addComponent<ProjectileComponent>(range, speed, vel);
    projectile.addComponent<ColliderComponent>("projectile", dmg);
    projectile.addGroup(Game::groupProjectiles);
}
///
void AssetManager::AutoGenerate(std::size_t type)
{
    switch (type)
    {
    case Game::ParticleType1:
        if ((std::rand() % 100) < 1)
        {
            auto &ptc1(manager->addEntity());
            ptc1.addComponent<TransformComponent>(800, 500 - 64 * 5, 64, 64, 5);
            ptc1.addComponent<SpriteComponent>("tree" + std::to_string(Game::level), false, 1);
            ptc1.getComponent<TransformComponent>().velocity.x = -10;
            ptc1.addGroup(Game::ParticleType1);
        }
        break;
    case Game::ParticleType2:
        if ((std::rand() % 100) < 30)
        {
            auto &ptc2(manager->addEntity());
            ptc2.addComponent<TransformComponent>(800, 500 - 16 * 2, 16, 16, 2);
            ptc2.addComponent<SpriteComponent>("grass" + std::to_string(Game::level), false, 1);
            ptc2.getComponent<TransformComponent>().velocity.x = -10;
            ptc2.addGroup(Game::ParticleType2);
        }
        break;
    case Game::ParticleType3:
        if ((std::rand() % 1000) < 1)
        {
            auto &ptc3(manager->addEntity());
            ptc3.addComponent<TransformComponent>(800, 500 - 128 * 2, 128, 128, 2);
            ptc3.addComponent<SpriteComponent>("building" + std::to_string(Game::level), false, 1);
            ptc3.getComponent<TransformComponent>().velocity.x = -10;
            ptc3.addGroup(Game::ParticleType3);
        }
        break;
    case Game::Elite:
        if ((std::rand() % 10000) < 20 * Game::level)
        {
            auto &en(manager->addEntity());
            en.addComponent<TransformComponent>(800, 500 - 16 * 4, 16, 16, 4);
            en.addComponent<SpriteComponent>("elite", true, 4);
            en.addComponent<ColliderComponent>("elite");
            if (Game::time > 3000)
            {
                en.addComponent<BehaviorComponent>(120 * Game::level * (std::rand() % 10) + 1, 40 * Game::level * (std::rand() % 10) + 1, 60 * Game::level * (std::rand() % 10) + 1, "_arrow");
            }
            else
            {
                en.addComponent<BehaviorComponent>(120 * Game::level, 40 * Game::level, 60 * Game::level, "_arrow");
            }
            en.getComponent<TransformComponent>().velocity.x = -1 * Game::level * 0.5;
            en.addGroup(Game::groupEnemies);
        }
        break;
    case Game::Range:
        if ((std::rand() % 200) < 1 * Game::level)
        {
            auto &en(manager->addEntity());
            en.addComponent<TransformComponent>(800, 500 - 16 * 3 * (std::rand() % 3 + 1), 16, 16, 3);
            en.addComponent<SpriteComponent>("range", true, 3);
            en.addComponent<ColliderComponent>("range");
            if (Game::time > 3000)
            {
                en.addComponent<BehaviorComponent>(35 * Game::level * (std::rand() % 10) + 1, 10 * Game::level * (std::rand() % 10) + 1, 15 * Game::level * (std::rand() % 10) + 1, "_ball");
            }
            else
            {
                en.addComponent<BehaviorComponent>(35 * Game::level, 10 * Game::level, 15 * Game::level, "_ball");
            }
            en.getComponent<TransformComponent>().velocity.x = -4 * Game::level * 0.5;
            en.addGroup(Game::groupEnemies);
        }
        break;
    case Game::Melee:
        if ((std::rand() % 200) < 2 * Game::level)
        {
            auto &en(manager->addEntity());
            en.addComponent<TransformComponent>(800, 500 - 16 * 3, 16, 16, 3);
            en.addComponent<SpriteComponent>("melee", true, 3);
            en.addComponent<ColliderComponent>("melee");
            if (Game::time > 3000)
            {
                en.addComponent<BehaviorComponent>(65 * Game::level * (std::rand() % 10) + 1, 20 * Game::level * (std::rand() % 10) + 1, 0, "melee");
            }
            else
            {
                en.addComponent<BehaviorComponent>(65 * Game::level, 20 * Game::level, 0, "melee");
            }
            en.getComponent<TransformComponent>().velocity.x = -8 * Game::level * 0.5;
            en.addGroup(Game::groupEnemies);
        }
        break;
    }
}
void AssetManager::AddTexture(std::string id, const char *path)
{
    textures.emplace(id, TextureManager::LoadTexture(path));
}

SDL_Texture *AssetManager::GetTexture(std::string id)
{
    return textures[id];
}

void AssetManager::CreateEffect(std::string type, Vector2D pos, std::string tag)
{
    auto &effect(manager->addEntity());
    effect.addComponent<TransformComponent>(pos.x + 32, pos.y - 64, 225, 225, 1);
    effect.addComponent<ColliderComponent>(tag, pos.x, pos.y, 225 * 1);
    effect.addComponent<EffectComponent>(type, true, 6);
    effect.addGroup(Game::Effect);
    Mix_PlayChannel(-1, GetSFX("slash"), 0);
    Mix_PlayChannel(-1, GetSFX("effect"), 0);
}

void AssetManager::CreateExplosion(std::string type, Vector2D pos, std::string tag)
{
    if (tag == "AB")
    {
        auto &explosion(manager->addEntity());
        explosion.addComponent<TransformComponent>(pos.x, pos.y - 110 * 4 + 64 * 4, 110, 110, 3);
        explosion.addComponent<ColliderComponent>(tag, pos.x, pos.y - 110 * 3 + 64 * 4, 110 * 3);
        explosion.addComponent<EffectComponent>(type + std::to_string((std::rand() % 4 + 1)), true, 6);
        explosion.getComponent<TransformComponent>().velocity.x = 10;
        explosion.addGroup(Game::Effect);
        
    }
    if (tag == "TB")
    {
        auto &explosion(manager->addEntity());
        explosion.addComponent<TransformComponent>(pos.x - 64, pos.y - 64, 274, 1161, 0.5);
        explosion.addComponent<ColliderComponent>(tag, pos.x - 64, pos.y - 64, 1161, 274);
        explosion.addComponent<EffectComponent>(type, true, 6);
        explosion.addGroup(Game::Effect);
    }
    if (tag == "DB")
    {
        auto &explosion(manager->addEntity());
        explosion.addComponent<TransformComponent>(pos.x + 64, pos.y + 96 - 350, 350, 300, 1);
        explosion.addComponent<ColliderComponent>(tag, pos.x + 64, pos.y + 96 - 350, 300, 350);
        explosion.addComponent<EffectComponent>(type, true, 12);
        explosion.addGroup(Game::Effect);
    }
}

void AssetManager::AddFont(std::string id, std::string path, int fontSize)
{
    fonts.emplace(id, TTF_OpenFont(path.c_str(), fontSize));
}

TTF_Font *AssetManager::GetFont(std::string id)
{
    return fonts[id];
}

void AssetManager::AddBGM(std::string id, const char *path)
{
    bgm.emplace(id, Mix_LoadMUS(path));
}

Mix_Music *AssetManager::GetBGM(std::string id)
{
    return bgm[id];
}

void AssetManager::AddSFX(std::string id, const char *path)
{
    sfx.emplace(id, Mix_LoadWAV(path));
}

Mix_Chunk *AssetManager::GetSFX(std::string id)
{
    return sfx[id];
}

void AssetManager::FreeBGM()
{
    for (auto &item : bgm)
    {
        Mix_FreeMusic(item.second);
    }
}

void AssetManager::FreeSFX()
{
    for (auto &item : sfx)
    {
        Mix_FreeChunk(item.second);
    }
}