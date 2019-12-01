#ifndef __j1Bat_H__
#define __j1Bat_H__

#define LIMIT_TIMER 50

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"
#include "p2Animation.h"
#include "j1Entity.h"

struct MapObject;
struct Collider;

enum states_entity4 {

	IDLE4,
	IDLE_ATTACK4,
	WALK4,
	DEATH4,
	NONE4

};

struct BatData
{

	Animation		hurt;
	Animation		death;
	Animation		attack;

	bool			move = false;
	bool			TimeAttack = false;
	bool			pathfinding = true;

	int				jumpCounter = 2;

	iPoint			colOffset;
	iPoint			ipos;

	SDL_Texture*	Tex_Golem;
	
	

};

// ----------------------------------------------------
class j1Bat : public j1Entity
{
public:

	//Constructor
	j1Bat(int x, int y);

	// Destructor
	~j1Bat();

	//Called at first
	bool Start();
	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);
	bool CleanUp();
	// Called before render is available

	bool Awake(pugi::xml_node& conf);

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	// Called before quitting


	//Called to start the player at initial pos
	void Restart();

	//Change the sound depending the animation


	//Puts the camera on player and follows

	void CheckState(float dt);	//Load keys to check states
	void State(float dt);	//Check animations
	void Pushbacks();	//Load the frame animations
	void Reset();
	bool PreTime(float sec);

	// Collisions
	void OnCollision(Collider* c1, Collider* c2);

private:


public:

	BatData				data_bat;
	states_entity4		current_stateE3;

private:


	p2SString			folder;
	p2SString			texture;


};



#endif // __j1PLAYER_H__