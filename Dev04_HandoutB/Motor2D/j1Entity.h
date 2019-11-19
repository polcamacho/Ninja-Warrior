#pragma once
#ifndef __j1ENTITY_H__
#define __j1ENTITY_H__

#define LIMIT_TIMER 50

#include "SDL/include/SDL.h"
#include "p2Point.h"
#include "j1EntityManager.h"

//
//
//enum entity_type {

//};
//

class Animation;

class j1Entity : public j1Module
{

public:



public:

	//CONSTRUCTOR
	j1Entity();
	j1Entity(entity_type type);

	//DESTRUCTOR
	virtual ~j1Entity();

	//Called at first
	virtual bool Start();
	virtual bool PreUpdate();
	virtual bool Update(float dt);
	virtual bool PostUpdate();

	// Called before render is available
	virtual bool Awake(pugi::xml_node & config);

	// Called before quitting
	virtual bool CleanUp();

	// Called each loop iteration
	virtual bool Save(pugi::xml_node&) const;
	virtual bool Load(pugi::xml_node&);


	void DrawEntity(int x, int y, entity_type entity);

	void CheckState(float dt);	//Load keys to check states
	void State(float dt);	//Check animations
	void Pushbacks() {};
	//void OnCollision(Collider* c1, Collider* c2);
	//void Reset() {};
	//bool pretime(float sec);


private:

public:

	Animation*			current_animation;

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

	SDL_Texture* path_texture = nullptr;

	bool destroy = false;
};


#endif // __j1ENTITY_H__*/

