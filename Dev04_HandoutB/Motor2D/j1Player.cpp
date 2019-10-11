#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Audio.h"
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

bool j1Player::Awake(pugi::xml_node& config) {

	bool ret = true;

	folder.create(config.child("folder").child_value());
	texture = config.child("texture").attribute("source").as_string();

	data_player.position.x = config.child("position").attribute("x").as_int();
	data_player.position.y = config.child("position").attribute("y").as_int();

	data_player.a.x = config.child("acceleration").attribute("x").as_int();
	data_player.a.y = config.child("acceleration").attribute("y").as_int();

	data_player.vj.x = config.child("jump_velocity").attribute("x").as_int();
	data_player.vj.y = config.child("jump_velocity").attribute("y").as_int();

	data_player.v.x = config.child("velocity").attribute("x").as_int();
	data_player.v.y = config.child("velocity").attribute("y").as_int();

	data_player.jumpFX = config.child("jumpFX").attribute("source").as_string();
	data_player.deathFX = config.child("deathFX").attribute("source").as_string();

	return ret;

	

}

void j1Player::DrawPlayer()
{
	

}


bool j1Player::Start() {

	
	data_player.injump = false;
	
	data_player.Pushbacks();
	data_player.current_animation = &data_player.idle;
	
	data_player.v = { 1,0 };
	data_player.position.x = 10;
	data_player.position.y = 50;

	App->audio->LoadFx(data_player.jumpFX.GetString());
	App->audio->LoadFx(data_player.deathFX.GetString());

	


}
bool j1Player::PreUpdate() {

}
bool j1Player::Update() {

}
bool j1Player::PostUpdate() {

}




//Called to start the player at initial pos
void j1Player::Restart() {

}

//Change the sound depending the animation
void j1Player::SFX(int channel, int repeat) {

}

//Puts the camera on player and follows
bool j1Player::Camera_Player() {

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




