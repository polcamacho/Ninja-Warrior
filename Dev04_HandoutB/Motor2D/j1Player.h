#pragma once
#ifndef __j1PLAYER_H__
#define __j1PLAYER_H__

#define LIMIT_TIMER 60

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"
#include "p2Animation.h"
#include "j1Entity.h"


struct MapObject;
struct Collider;

enum states_player {
	IDLE,
	WALK,
	RUN,
	JUMP_UP,
	JUMP_FALL,
	JUMP_WALK,
	JUMP_RUN,
	DOUBLE_JUMP,
	IDLE_ATTACK,
	DEATH,
	NONE
};

struct PlayerData
{
	Animation		walk2;
	Animation		running;
	Animation		jump;
	Animation		idle_attack;
	Animation		walk_attack;

	bool			move=false;

	int jumpCounter = 2;

	iPoint a;	//Acceleration (x,y)
		//Velocity of the jump (x,y)
	iPoint			colOffset;
	SDL_Texture*	Tex_Player;
	
	bool injump=false;	//Says that the player is jumping or not
	bool canjump = false;  //Says the player can jump or not
	int jumpvel;		//Jump velocity only in y
	int jumpenergy;		//energy while is jumping
	bool left=false;	//If left = true, jump running backward
	bool right=false;	//If right = true, jump running forward
	bool TimeAttack = false;

	float velrun;			//velocity while running
};

// ----------------------------------------------------
class j1Player : public j1Entity
{
public:

	//Constructor
	j1Player(int x, int y);

	// Destructor
	~j1Player();

	//Called at first
	bool Start();
	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);

	// Called before render is available
	bool Awake(pugi::xml_node& conf);
		
	// Called each loop iteration
	
	void DrawPlayer();

	//Called to start the player at initial pos
	//void Restart();
	
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

	PlayerData			data_player;
	
	bool godmode = false;

	p2SString			name;

private:

	
	states_player		current_state;
	p2SString			folder;
	p2SString			texture;


};



#endif // __j1PLAYER_H__