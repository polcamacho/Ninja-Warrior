#pragma once
#ifndef __j1PLAYER_H__
#define __j1PLAYER_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"
#include "p2Animation.h"

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

	Animation*		current_animation;
	Animation		idle;
	Animation		walk;
	Animation		walk2;
	Animation		running;
	Animation		jump;
	Animation		fall;
	Animation		idle_attack;
	Animation		walk_attack;
	Animation		death;

	
	bool			grounded = false;
	bool			platformdrop;
	bool			platformdrop2;
	bool			move=false;

	int jumpCounter = 2;

	iPoint position;	//Position of the player (x,y)
	iPoint preposition;
	iPoint a;	//Acceleration (x,y)
		//Velocity of the jump (x,y)
	iPoint v;	//Velocity of the player (x,y)

	SDL_Rect		col;
	iPoint			colOffset;
	SDL_Texture*	Tex_Player;
	
	bool injump=false;	//Says that the player is jumping or not
	bool canjump = false;  //Says the player can jump or not
	int jumpvel;		//Jump velocity only in y
	int jumpenergy;		//energy while is jumping
	bool left=false;	//If left = true, jump running backward
	bool right=false;	//If right = true, jump running forward
	bool player_flip;
	bool TimeAttack = false;

	int gravity;

	int velrun;			//velocity while running
	   
	Collider* colliders = nullptr;
	Collider* colliders2 = nullptr;
	bool showcolliders = false;

	//void Pushbacks();	//Load the frame animations

};

// ----------------------------------------------------
class j1Player : public j1Module
{
public:

	//Constructor
	j1Player();

	// Destructor
	virtual ~j1Player();

	//Called at first
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate(float dt);

	// Called before render is available
	
	bool Awake(pugi::xml_node& conf);

	
	// Called each loop iteration
	
	void DrawPlayer();

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	// Called before quitting
	bool CleanUp();

	//Called to start the player at initial pos
	//void Restart();
	
	//Change the sound depending the animation
	
	
	//Puts the camera on player and follows
	
	void CheckState();	//Load keys to check states
	void State();	//Check animations
	void Pushbacks();	//Load the frame animations
	void Reset();
	bool pretime(float sec);

	// Collisions
	void OnCollision(Collider* c1, Collider* c2);

private:

	
public:

	PlayerData			data_player;
	int globaltime;
	int pretimer=0;
	bool die = false;
	bool godmode = false;

private:

	
	states_player		current_state;
	p2SString			folder;
	p2SString			texture;


};



#endif // __j1PLAYER_H__