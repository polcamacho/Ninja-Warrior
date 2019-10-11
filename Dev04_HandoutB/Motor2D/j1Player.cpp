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
	
	data_player.Pushbacks();
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

void PlayerData::Pushbacks() {

	idle.PushBack({ 7,554,48,52 });
	idle.PushBack({ 84,554,41,52 });
	idle.PushBack({ 156,555,42,50 });
	idle.PushBack({ 232,557,41,48 });
	idle.PushBack({ 291,557,41,48 });
	idle.PushBack({ 351,557,40,48 });
	idle.PushBack({ 415,556,41,49 });
	idle.PushBack({ 474,556,42,49 });
	idle.PushBack({ 533,557,41,48 });
	idle.PushBack({ 591,554,41,51 });
	idle.PushBack({ 643,554,52,52 });
	idle.PushBack({ 705,555,44,50 });
	idle.PushBack({ 763,554,43,52 });
	idle.PushBack({ 819,554,45,52 });
	idle.PushBack({ 880,554,43,51 });
	idle.PushBack({ 938,555,43,51 });
	idle.PushBack({ 995,554,44,52 });
	idle.loop = true;
	idle.speed = 10.1f;

	walk.PushBack({ 7,4,42,49 });
	walk.PushBack({ 65,4,41,49 });
	walk.PushBack({ 131,3,38,50 });
	walk.PushBack({ 201,3,36,51 });
	walk.PushBack({ 259,5,38,48 });
	walk.PushBack({ 317,4,44,50 });
	walk.loop = true;
	walk.speed = 10.1f;

	jump.PushBack({ 7,128,52,43 });
	jump.PushBack({ 81,114,47,57 });
	jump.PushBack({ 155,113,42,58 });
	jump.PushBack({ 221,118,52,53 });
	jump.PushBack({ 285,116,53,55 });
	jump.PushBack({ 348,119,56,52 });
	jump.PushBack({ 423,129,53,42 });
	jump.loop = true;
	jump.speed = 10.1f;

	death.PushBack({ 8,464,48,55 });
	death.PushBack({ 72,456,62,63 });
	death.PushBack({ 150,454,69,65 });
	death.PushBack({ 257,464,82,56 });
	death.PushBack({ 367,447,83,72 });
	death.PushBack({ 488,447,84,73 });
	death.PushBack({ 609,457,94,62 });
	death.PushBack({ 734,455,113,65 });
	death.PushBack({ 894,445,127,74 });
	death.PushBack({ 1054,433,116,87 });
	death.loop = true;
	death.speed = 10.1f;


}

/*Flip Image

if (Player.speed.x < 0)
		Player.flip = true;
	else if(Player.speed.x > 0)
		Player.flip = false;

*/


