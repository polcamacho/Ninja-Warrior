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

	IDLE1,
	IDLE_ATTACK1,
	WALK1,
	DEATH1,
	NONE1,
	RUN1,
	JUMP_UP1,
	JUMP_FALL1,
	JUMP_WALK1,
	JUMP_RUN1,
	DOUBLE_JUMP1,

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
	float D_Timer = 0;
	float D2_Timer = 0;

	iPoint			colOffset;
	SDL_Texture*	Tex_Player;
	
	bool injump=false;	//Says that the player is jumping or not
	bool canjump = false;  //Says the player can jump or not
	int jumpvel;		//Jump velocity only in y
	int jumpenergy;		//energy while is jumping
	bool left=false;	//If left = true, jump running backward
	bool right=false;	//If right = true, jump running forward
	bool TimeAttack = false;
	bool checkpoint = false;

	iPoint		ipos;

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
	bool CleanUp();
	// Called before render is available
	bool Awake(pugi::xml_node& conf);
	
	bool Load(pugi::xml_node& data);
	bool Save(pugi::xml_node& data) const;
	// Called each loop iteration

	//Called to start the player at initial pos
	//void Restart();
	
	//Change the sound depending the animation
	
	void Camera();
	
	//Puts the camera on player and follows
	
	void CheckState(float dt);	//Load keys to check states
	void State(float dt);	//Check animations
	void Pushbacks();	//Load the frame animations
	void Reset();
	//bool PreTime(float sec);

	// Collisions
	void OnCollision(Collider* c1, Collider* c2);

private:

	
public:

	PlayerData			data_player;
	
	bool godmode = false;
	int count = 0;
	p2SString			name;
	

private:


	states_player		current_stateP;
	p2SString			folder;
	p2SString			texture;


};



#endif // __j1PLAYER_H__