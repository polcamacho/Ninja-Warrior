#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Audio.h"
#include "j1Textures.h"
#include "j1Player.h"
#include "j1Window.h"
#include "j1Map.h"
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

	data_player.walkFX = config.child("walkFX").attribute("source").as_string();
	data_player.deathFX = config.child("deathFX").attribute("source").as_string();

	return ret;

	

}

void j1Player::DrawPlayer()
{
	
	if (data_player.player_flip == true) {
		App->render->Blit(data_player.Tex_Player, data_player.position.x, data_player.position.y, &(data_player.current_animation->GetCurrentFrame()), SDL_FLIP_HORIZONTAL, -1.0);
	}
	else {
		App->render->Blit(data_player.Tex_Player, data_player.position.x, data_player.position.y, &(data_player.current_animation->GetCurrentFrame()), SDL_FLIP_NONE, -1.0);
	}
}

bool j1Player::Start() {

	
	data_player.injump = false;
	
//	data_player.Pushbacks();
	data_player.current_animation = &data_player.idle;
	
	data_player.v = { 1,0 };
	data_player.position.x = 10;
	data_player.position.y = 50;

	App->render->camera.x = data_player.position.x;
	App->render->camera.y = data_player.position.y;

	App->audio->LoadFx(data_player.walkFX.GetString());
	App->audio->LoadFx(data_player.deathFX.GetString());

	data_player.Tex_Player = App->tex->Load(PATH(folder.GetString(), texture.GetString()));

	return	true;

}

bool j1Player::PreUpdate() {

	return true;

}

bool j1Player::Update() {

	return true;

}

bool j1Player::PostUpdate() {

	//Camera_Player();

	return true;
}



//Called to start the player at initial pos
//void j1Player::Restart() {

//}

//Change the sound depending the animation
//void j1Player::SFX(int channel, int repeat) {

	

//}

//Puts the camera on player and follows
/*bool j1Player::Camera_Player() {

	App->render->camera.x = data_player.position.x;
	if (App->render->camera.x < 0)App->render->camera.x = 0;
	App->render->camera.y = data_player.position.y - App->render->camera.h / 2;
	if (App->render->camera.y + App->win->height > App->map->data.height*App->map->data.tile_height)App->render->camera.y = App->map->data.height*App->map->data.tile_height - App->win->height;
	return true;
}*/


// Called before quitting
bool j1Player::CleanUp()
{
	LOG("Unloading player");

	App->tex->UnLoad(data_player.Tex_Player);
	
	return true;
}
bool j1Player::Load(pugi::xml_node& node) {

	data_player.position.x = node.child("position").attribute("x").as_int();
	data_player.position.y = node.child("position").attribute("y").as_int();

	data_player.a.x = node.child("acceleration").attribute("x").as_int();
	data_player.a.y = node.child("acceleration").attribute("y").as_int();

	data_player.vj.x = node.child("jump_velocity").attribute("x").as_int();
	data_player.vj.y = node.child("jump_velocity").attribute("y").as_int();

	data_player.v.x = node.child("velocity").attribute("x").as_int();
	data_player.v.y = node.child("velocity").attribute("y").as_int();

	return true;

}
bool j1Player::Save(pugi::xml_node& node) const {

	node.append_child("position").append_attribute("x") = data_player.position.x;
	node.child("position").attribute("y") = data_player.position.y;

	node.append_child("acceleration").append_attribute("x") = data_player.a.x;
	node.child("acceleration").attribute("y") = data_player.a.y;

	node.append_child("jump_velocity").append_attribute("x") = data_player.vj.x;
	node.child("jump_velocity").attribute("y") = data_player.vj.y;

	node.append_child("velocity").append_attribute("x") = data_player.v.x;
	node.child("velocity").attribute("y") = data_player.v.y;

	return true;

}

/*Flip Image

if (Player.speed.x < 0)
		Player.flip = true;
	else if(Player.speed.x > 0)
		Player.flip = false;

*/


