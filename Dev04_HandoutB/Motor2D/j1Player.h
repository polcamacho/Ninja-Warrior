#pragma once
#ifndef __j1PLAYER_H__
#define __j1PLAYER_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"
#include "p2Animation.h"

enum states_player {
	IDLE,
	WALK_FORWARD,
	WALK_BACKWARD,
	JUMP_UP,
	JUMP_FALL,
	DEATH
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
	bool			grounded;

	iPoint position;
	//fPoint position = fPoint(0, 0);
	//p2Point<int> position;	//Position of the player (x,y)
	iPoint a;	//Acceleration (x,y)
	iPoint vj;	//Velocity of the jump (x,y)
	iPoint v;	//Velocity of the player (x,y)

	SDL_Rect		col;
	iPoint			colOffset;
	SDL_Texture*	Tex_Player;

	float jump_time=0.0f;	//Time of the jump

	bool injump=false;	//Says that the player is jumping or not
	bool player_flip;
	int jumpspeed=0;
	int jumpspeedx=0;
	int initialposy;



	

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
	bool Update();
	bool PostUpdate();

	// Called before render is available
	
	bool Awake(pugi::xml_node& conf);

	
	// Called each loop iteration
	
	void DrawPlayer();

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	

	// Called before quitting
	bool CleanUp();

	
	
	void Animation();	//Load keys to change the animations
	void Pushbacks();	//Load the frame animations
	
	iPoint GetPosition();


private:

	
public:


private:

	PlayerData			data_player;
	states_player		current_state;
	p2SString			folder;
	p2SString			texture;




};



#endif // __j1PLAYER_H__