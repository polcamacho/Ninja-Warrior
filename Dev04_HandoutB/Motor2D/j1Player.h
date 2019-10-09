#pragma once
#ifndef __j1PLAYER_H__
#define __j1PLAYER_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"
#include "p2Animayio


// ----------------------------------------------------
class j1Player : public j1Module
{
public:

	j1Player();

	// Destructor
	virtual ~j1Player();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* path);



	// TODO 8: Create a method that translates x,y coordinates from map positions to world positions
	iPoint GetWorldPos(int x, int y) const; //Gets the position in the world

private:

	bool LoadPlayer(pugi::xml_node& node); //Load the map

public:


private:

	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;
	uint idtile = 0;
	SDL_Texture* texture;

	Animation		idle;
	Animation		running;
	Animation		jumping_up;
	Animation		falling;
	Animation		dashing;
	Animation		dying;
	j1Timer			dashtimer;
	p2SString		jumpFX;
	p2SString		deathFX;
	p2SString		landFX;
	p2SString		dashFX;
	p2SString		pickupFX;
	p2SString		timestopFX;


};

#endif // __j1PLAYER_H__