#pragma once
#ifndef __j1GOLEM1_H__
#define __j1GOLEM1_H__


#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"
#include "p2Animation.h"
#include "j1Entity.h"

struct MapObject;
struct Collider;

enum states_entity2 {

	IDLE2,
	IDLE_ATTACK2,
	WALK2,
	DEATH2,
	NONE2

};

struct GolemData
{

	Animation		death;
	
	bool			pathfinding = true;

	iPoint			colOffset;
	iPoint			ipos;

};

// ----------------------------------------------------
class j1Golem1 : public j1Entity
{
public:

	//Constructor
	j1Golem1(int x, int y);

	// Destructor
	~j1Golem1();

	//Called at first
	bool Start();
	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);
	bool CleanUpGolem1();
	// Called before render is available
	
	bool Awake(pugi::xml_node& conf);

	//bool Load(pugi::xml_node&);
	//bool Save(pugi::xml_node&) const;

	// Called before quitting
	

	//Called to start the player at initial pos
	void Restart();
	bool Pathfinding(float dt);
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

	GolemData			data_golem;
	states_entity2		current_stateE2;

private:

	p2SString			folder;
	p2SString			texture;


};



#endif // __j1PLAYER_H__