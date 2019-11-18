#pragma once
#ifndef __j1ENEMY_H__
#define __j1ENEMY_H__

#define LIMIT_TIMER 50

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"
#include "p2Animation.h"

struct MapObject;
struct Collider;

enum states_enemy {
	IDLE2,
	WALK2,
	RUN2,
	JUMP_UP2,
	JUMP_FALL2,
	JUMP_WALK2,
	JUMP_RUN2,
	DOUBLE_JUMP2,
	IDLE_ATTACK2,
	DEATH2,
	NONE2
};

struct EnemyData
{

	Animation*		current_animation;
	Animation		idle;
	Animation		walk;
	Animation		hurt;
	Animation		death;
	Animation		attack;
	
	bool			grounded = false;
	bool			platformdrop;
	bool			platformdrop2;
	bool			move=false;

	int jumpCounter = 2;

	iPoint position;	//Position of the player (x,y)
	iPoint preposition;
	iPoint a;	//Acceleration (x,y)
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

	float velrun;			//velocity while running
	   
	Collider* colliders = nullptr;
	Collider* colliders2 = nullptr;
	bool showcolliders = false;

	//void Pushbacks();	//Load the frame animations

};

// ----------------------------------------------------
class j1Enemy : public j1Module
{
public:

	//Constructor
	j1Enemy();

	// Destructor
	virtual ~j1Enemy();

	//Called at first
	bool Start();
	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);

	// Called before render is available
	
	bool Awake(pugi::xml_node& conf);

	
	// Called each loop iteration
	
	void DrawEnemy();

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	// Called before quitting
	bool CleanUp();

	//Called to start the player at initial pos
	//void Restart();
	
	//Change the sound depending the animation
	
	
	//Puts the camera on player and follows
	
	void CheckState(float dt);	//Load keys to check states
	void State(float dt);	//Check animations
	void Pushbacks();	//Load the frame animations
	void Reset();
	bool pretime(float sec);

	// Collisions
	void OnCollision(Collider* c1, Collider* c2);

private:

	
public:

	EnemyData			data_enemy;
	int globaltime;
	int pretimer=0;
	bool die = false;
	bool godmode = false;

private:

	
	states_enemy		current_state;
	p2SString			folder;
	p2SString			texture;


};



#endif // __j1PLAYER_H__