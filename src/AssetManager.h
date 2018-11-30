#pragma once

#include <map>
#include <string>
#include "TextureManager.h"
#include "Vector2D.h"
#include "ECS/ECS.h"
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

class AssetManager
{
public:
  AssetManager(Manager *man);
  ~AssetManager();

  void CreateProjectile(Vector2D pos, Vector2D vel, int range, int speed,int dmg, std::string id);
  void AutoGenerate(std::size_t type);
  void AddTexture(std::string id, const char *path);
  SDL_Texture *GetTexture(std::string id);
  void CreateEffect(std::string type,Vector2D pos,std::string tag);
  void CreateExplosion(std::string type,Vector2D pos,std::string tag);
  void AddFont(std::string id,std::string path,int fontSize);
  TTF_Font* GetFont(std::string id);
  void AddBGM(std::string id,const char* path);
  Mix_Music* GetBGM(std::string id);
  void AddSFX(std::string id,const char* path);
  Mix_Chunk* GetSFX(std::string id);
  void FreeBGM();
  void FreeSFX();

private:
  Manager *manager;
  std::map<std::string, SDL_Texture *> textures;
  std::map<std::string, TTF_Font*> fonts;
  std::map<std::string, Mix_Music*> bgm;
  std::map<std::string, Mix_Chunk*> sfx; 
};