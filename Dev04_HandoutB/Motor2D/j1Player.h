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
	Animation		death;

	p2SString		jumpFX;
	p2SString		deathFX;
	p2SString		walkFX;
	
	bool			grounded = false;
	bool			platformdrop;
	bool			platformdrop2;
	bool			move=false;

	iPoint position;	//Position of the player (x,y)
	iPoint preposition;
	iPoint a;	//Acceleration (x,y)
		//Velocity of the jump (x,y)
	iPoint v;	//Velocity of the player (x,y)

	SDL_Rect		col;
	iPoint			colOffset;
	SDL_Texture*	Tex_Player;

	float jump_time=0.0f;	//Time of the jump

	bool injump=false;	//Says that the player is jumping or not
	bool canjump = false;  //Says the player can jump or not
	int jumpvel;
	int jumpenergy;
	int doublejump;
	bool left=false;
	bool right=false;
	bool player_flip;
	float gravity;

	int velrun;

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
	//void SFX(int channel, int repeat);
	
	//Puts the camera on player and follows
	
	void CheckState();	//Load keys to check states
	void Animations();	//Check animations
	void Pushbacks();	//Load the frame animations
	
	// Collisions
	void OnCollision(Collider* c1, Collider* c2);

private:

	
public:

	PlayerData			data_player;

private:

	
	states_player		current_state;
	p2SString			folder;
	p2SString			texture;




};



#endif // __j1PLAYER_H__