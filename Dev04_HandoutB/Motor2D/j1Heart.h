#ifndef __j1Heart_H__
#define __j1Heart_H__

#define LIMIT_TIMER 50

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"
#include "p2Animation.h"
#include "j1Entity.h"

struct MapObject;
struct Collider;

enum states_entity5 {

	IDLE5,
	NONE5

};

struct HeartData
{

	int				jumpCounter = 2;
	
	iPoint			colOffset;
	iPoint			ipos;
	
	SDL_Texture*	Tex_Heart;
	
};

// ----------------------------------------------------
class j1Heart : public j1Entity
{
public:

	//Constructor
	j1Heart(int x, int y);

	// Destructor
	~j1Heart();

	//Called at first
	bool Start();
	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);
	bool CleanUpBat();
	// Called before render is available

	bool Awake(pugi::xml_node& conf);

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	// Called before quitting


	//Called to start the player at initial pos
	//void Restart();

	//Change the sound depending the animation


	//Puts the camera on player and follows

	//void CheckState(float dt);	//Load keys to check states
	void State(float dt);	//Check animations
	void Pushbacks();	//Load the frame animations
	void Reset();
	bool PreTime(float sec);

	// Collisions
	void OnCollision(Collider* c1, Collider* c2);

private:


public:

	HeartData			data_heart;
	states_entity5		current_stateE4;

private:


	p2SString			folder;
	p2SString			texture;


};



#endif