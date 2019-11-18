/*#pragma once
#ifndef __j1ENTITY_H__
#define __j1ENTITY_H__

#define LIMIT_TIMER 50

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"
#include "p2Animation.h"
#include "j1Map.h"
#include "j1Textures.h"

class Entity{

public:
	
	enum entity_type {
		
		NONE,
		PLAYER,
		GOLEM_GRASS_ENEMY,
		GOLEM_ROCK_ENEMY,
		MINOTAUR_ENEMY,
		EYE_ENEMY,
		BAT_ENEMY,

	};

public:
	
	//CONSTRUCTOR
	Entity();
	Entity(entity_type type);
	
	//DESTRUCTOR
	~Entity();
	
	//Called at first
	virtual bool Start() { return true; };
	virtual bool PreUpdate() { return true; };
	virtual bool Update(float dt) { return true; };
	virtual bool PostUpdate() { return true; };

	// Called before render is available
	virtual bool Awake(pugi::xml_node & config) { return true; };
	
	// Called before quitting
	virtual void CleanUp() {};

	// Called each loop iteration
	virtual void Save(pugi::xml_node& file) const {};
	virtual void Load(pugi::xml_node& file) {};

	virtual void CheckState(float dt);	//Load keys to check states
	virtual void State(float dt);	//Check animations
	virtual void Pushbacks() {};
	virtual void OnCollision(Collider* c1, Collider* c2);
	virtual void Reset() {};
	virtual bool pretime(float sec);
	

private:

public:

	Animation*			current_animation;
	Animation			idle;
	Animation			walk;
	Animation			fall;
	Animation			death;
	
	entity_type type;

	bool				grounded = false;
	bool				platformdrop;
	bool				platformdrop2;
	bool				left = false;	//If left = true, jump running backward
	bool				right = false;	//If right = true, jump running forward
	bool				flip;
	bool				showcolliders = false;
	bool				die = false;
	bool				godmode = false;

	iPoint				position;	//Position of the player (x,y)
	iPoint				preposition;
	iPoint				v;	//Velocity of the player (x,y)
	
	SDL_Rect			col;	//Collider RECT
	
	SDL_Texture*		texture;	//TEXTURE

	int					gravity;
	int					globaltime;
	int					pretimer = 0;
	
	Collider*			colliders = nullptr;

};

#endif // __j1ENTITY_H__*/