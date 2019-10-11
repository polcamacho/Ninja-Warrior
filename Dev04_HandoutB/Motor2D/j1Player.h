#pragma once
#ifndef __j1PLAYER_H__
#define __j1PLAYER_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"
#include "p2Animation.h"


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
	
	//bool Awake(pugi::xml_node& conf);

	
	// Called each loop iteration
	
	void DrawPlayer();

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;


	// Called before quitting
	bool CleanUp();

	//Called to start the player at initial pos
	void Restart();
	
	//Change the sound depending the animation
	void SFX(int channel, int repeat);
	
	//Puts the camera on player and follows
	bool Camera_Player();

private:

	
public:


private:

	PlayerData	data_player;
	p2SString			folder;
	p2SString			texture;

};

struct PlayerData
{

	Animation*		current_animation;
	Animation		idle;
	Animation		walk;
	Animation		running;
	Animation		jump;
	Animation		fall;
	Animation		dying;

	p2SString		jumpFX;
	p2SString		deathFX;
	
	iPoint position;	//Position of the player (x,y)
	iPoint a;	//Acceleration (x,y)
	iPoint vj;	//Velocity of the jump (x,y)
	iPoint v;	//Velocity of the player (x,y)

	SDL_Rect		col;
	SDL_Texture*	Tex_Player;

	float jump_time;	//Time of the jump
	
	bool injump;	//Says that the player is jumping or not
	
	void Pushbacks();

};

#endif // __j1PLAYER_H__