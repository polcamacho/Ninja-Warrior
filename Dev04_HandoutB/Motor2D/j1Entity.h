#ifndef __j1ENTITY_H__
#define __j1ENTITY_H__

#define LIMIT_TIMER 60

#include "PugiXml/src/pugixml.hpp"
#include "SDL/include/SDL.h"
#include "j1Module.h"
#include "p2Point.h"
#include "p2Animation.h"

struct Collider;
struct SDL_Texture;
struct Animation;

class j1Entity: public j1Module
{

public:

	enum class entity_type {

		ENTITY_NONE,
		PLAYER,
		GOLEM_GRASS_ENEMY,
		GOLEM_ROCK_ENEMY,
		MINOTAUR_ENEMY,
		FLYING_EYE_ENEMY,
		BAT_ENEMY,

	};

	entity_type type;

	j1Entity(entity_type type);

	virtual ~j1Entity();

	bool CleanUp();
	
	// Called each loop iteration
	bool Save(pugi::xml_node&) const;
	bool Load(pugi::xml_node&);

	//void OnCollision(Collider* c1, Collider* c2);

	void State(float dt);	//Check animations
	void Pushbacks() {};
	void Reset() {};
	bool PreTime(float sec);
	void DrawCollider();

private:

public:
	
	iPoint				position;	//Position of the player (x,y)
	iPoint				preposition;
	iPoint				v;	//Velocity of the player (x,y)
	int					gravity;

	Animation* current_animation = nullptr;
	Animation  idle;
	Animation  walk;
	Animation  fall;
	Animation  death;

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

	Collider *entity_colliders;
	bool destroy = false;

};

#endif // __j1ENTITY_H__*/

