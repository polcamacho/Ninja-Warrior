#pragma once
#ifndef _j1ENTITYMANAGER_H_
#define _j1ENTITYMANAGER_H_

#include "j1Module.h"
#include "p2List.h"
#include "PugiXml/src/pugixml.hpp"

#define LIMIT_TIMER 60

struct SDL_Texture;

class j1Entity;

enum entity_type {
	
		ENTITY_NONE,
		PLAYER,
		GOLEM_GRASS_ENEMY,
		GOLEM_ROCK_ENEMY,
		MINOTAUR_ENEMY,
		FLYING_EYE_ENEMY,
		BAT_ENEMY,
	
	};

class j1EntityManager :public j1Module {

public:
	j1EntityManager();
	~j1EntityManager();

	bool Awake(pugi::xml_node&);
	bool Start();
	bool PreUpdate(float dt);
	bool PostUpdate(float dt);
	bool Update(float dt);
	bool CleanUp();

	j1Entity* DrawEntity(int x, int y, entity_type type);

	bool CleanEntity();
	void DeleteEntity();

	j1Entity* GetPlayer();

public:
	p2List<j1Entity*>	entities;
	SDL_Texture*	Tex_Player = nullptr;
	SDL_Texture*	Tex_Golems = nullptr;

	p2SString		folder;
	p2SString		texture1;
	p2SString		texture2;

};


#endif // !_j1ENTITYMANAGER_H_