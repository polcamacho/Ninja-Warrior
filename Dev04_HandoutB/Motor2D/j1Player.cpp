#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Player.h"
#include <math.h>

j1Player::j1Player() : j1Module()
{
	name.create("player");
}

// Destructor
j1Player::~j1Player()
{}


void j1Player::DrawPlayer()
{
	

}

// Called before quitting
bool j1Player::CleanUp()
{
	LOG("Unloading player");

	
	return true;
}

bool j1Player::Load(pugi::xml_node& node) {

}
bool j1Player::Save(pugi::xml_node& node) const {

}



