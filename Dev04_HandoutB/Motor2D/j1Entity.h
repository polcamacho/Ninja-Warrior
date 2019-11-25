#pragma once
#ifndef __j1ENTITY_H__
#define __j1ENTITY_H__

#define LIMIT_TIMER 60

#include "SDL/include/SDL.h"
#include "p2Point.h"
#include "j1EntityManager.h"


class Animation;

class j1Entity
{

public:

	j1Entity(entity_type type);

	virtual ~j1Entity();

	//Called at first
	virtual bool Update(float dt);

	// Called before render is available
	virtual bool Awake(pugi::xml_node& config);

	// Called each loop iteration
	virtual bool Save(pugi::xml_node&) const;
	virtual bool Load(pugi::xml_node&);

	virtual bool DrawEntity(int x, int y, entity_type type);

	void State(float dt);	//Check animations
	void Pushbacks() {};
	void OnCollision(Collider* c1, Collider* c2);
	void Reset() {};
	bool PreTime(float sec);
	

private:

public:

	//trash ??
	Animation* current_animation=nullptr;

	entity_type type = entity_type::ENTITY_NONE;
	SDL_Texture* texture;	//TEXTURE
	SDL_Texture* path_texture = nullptr;

	bool				grounded = false;
	bool				platformdrop;
	bool				flip;
	bool				showcolliders = false;
	bool				die = false;

	SDL_Rect			col;	//Collider RECT

	int					globaltime;
	int					pretimer = 0;

	Collider* colliders = nullptr;
	bool destroy = false;

	

};

struct EntityData {

	iPoint				position;	//Position of the player (x,y)
	iPoint				preposition;
	iPoint				v;	//Velocity of the player (x,y)
	int					gravity;
	

};

EntityData data_entity;
	


#endif // __j1ENTITY_H__*/

