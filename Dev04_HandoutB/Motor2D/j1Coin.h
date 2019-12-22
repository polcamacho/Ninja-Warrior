#ifndef __j1Coin_H__
#define __j1Coin_H__

#define LIMIT_TIMER 50

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"
#include "p2Animation.h"
#include "j1Entity.h"

struct MapObject;
struct Collider;

enum states_entity6 {

	IDLE6,
	NONE6

};

struct CoinData
{

	int				jumpCounter = 2;

	iPoint			colOffset;
	iPoint			ipos;

	SDL_Texture*	Tex_Coin;

};

// ----------------------------------------------------
class j1Coin : public j1Entity
{
public:

	//Constructor
	j1Coin(int x, int y);

	// Destructor
	~j1Coin();

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

	//void CheckState(float dt);	//Load keys to check states
	void State(float dt);	//Check animations
	void Pushbacks();	//Load the frame animations
	void Reset();
	bool PreTime(float sec);

	// Collisions
	void OnCollision(Collider* c1, Collider* c2);

private:


public:

	CoinData			data_coin;
	states_entity6		current_stateE5;

private:


	p2SString			folder;
	p2SString			texture;


};



#endif