//#pragma once
//#ifndef __j1ENTITY_H__
//#define __j1ENTITY_H__
//
//#define LIMIT_TIMER 50
//
//#include "PugiXml/src/pugixml.hpp"
//#include "p2List.h"
//#include "p2Point.h"
//#include "j1Module.h"
//#include "p2Animation.h"
//#include "j1Map.h"
//#include "j1Textures.h"
//
//class Entity;
//
//enum entity_type {
//
//	NONE,
//	PLAYER,
//	GOLEM_GRASS_ENEMY,
//	GOLEM_ROCK_ENEMY,
//	MINOTAUR_ENEMY,
//	EYE_ENEMY,
//	BAT_ENEMY,
//
//};
//
//class j1Entity : public j1Module
//{
//
//public:
//	
//	
//
//public:
//	
//	//CONSTRUCTOR
//	j1Entity();
//	j1Entity(entity_type type);
//	
//	//DESTRUCTOR
//	~j1Entity();
//	
//	//Called at first
//	bool Start();
//	bool PreUpdate();
//	bool Update(float dt);
//	bool PostUpdate();
//
//	// Called before render is available
//	bool Awake(pugi::xml_node & config);
//	
//	// Called before quitting
//	void CleanUp();
//
//	// Called each loop iteration
//	void Save(pugi::xml_node& file) const;
//	void Load(pugi::xml_node& file);
//
//	void DrawEntity(int x, int y, entity_type entity);
//
//	//void CheckState(float dt);	//Load keys to check states
//	//void State(float dt);	//Check animations
//	//void Pushbacks() {};
//	//void OnCollision(Collider* c1, Collider* c2);
//	//void Reset() {};
//	//bool pretime(float sec);
//	
//
//private:
//
//public:
//
//	Animation*			current_animation;
//	Animation			idle;
//	Animation			walk;
//	Animation			fall;
//	Animation			death;
//	
//	entity_type type;
//
//	bool				grounded = false;
//	bool				platformdrop;
//	bool				platformdrop2;
//	bool				left = false;	//If left = true, jump running backward
//	bool				right = false;	//If right = true, jump running forward
//	bool				flip;
//	bool				showcolliders = false;
//	bool				die = false;
//	bool				godmode = false;
//
//	iPoint				position;	//Position of the player (x,y)
//	iPoint				preposition;
//	iPoint				v;	//Velocity of the player (x,y)
//	
//	SDL_Rect			col;	//Collider RECT
//	
//	SDL_Texture*		texture;	//TEXTURE
//
//	int					gravity;
//	int					globaltime;
//	int					pretimer = 0;
//	
//	Collider*			colliders = nullptr;
//
//	p2List<Entity*> entities;
//
//	SDL_Texture* path_texture = nullptr;
//
//	bool destroy = false;
//};
//
//
//#endif // __j1ENTITY_H__*/
//
