#pragma once
#ifndef _j1ENTITYMANAGER_H_
#define _j1ENTITYMANAGER_H_

#include "j1Module.h"
#include "p2List.h"

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
	bool Update(float dt);
	bool CleanUp();

	bool DrawEntity(int x, int y, entity_type type);


public:
	p2List<j1Entity*> entities;
};


#endif // !_j1ENTITYMANAGER_H_