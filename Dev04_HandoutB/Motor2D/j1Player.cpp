#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Audio.h"
#include "j1Textures.h"
#include "j1Player.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Input.h"
#include "j1Collider.h"
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

	data_player.vj.x = config.child("jump_velocity").attribute("jumpvel").as_int();

	data_player.v.x = config.child("velocity").attribute("x").as_int();
	data_player.v.y = config.child("velocity").attribute("y").as_int();

	data_player.walkFX = config.child("walkFX").attribute("source").as_string();
	data_player.deathFX = config.child("deathFX").attribute("source").as_string();

	data_player.colOffset.x = config.child("colOffset").attribute("x").as_int();
	data_player.colOffset.y = config.child("colOffset").attribute("y").as_int();
	
	data_player.gravity = config.child("gravity").attribute("grav").as_float();

	
	return ret;

}

void j1Player::DrawPlayer()
{
	

	if (data_player.player_flip) {
		App->render->Blit(data_player.Tex_Player, data_player.position.x, data_player.position.y, &(data_player.current_animation->GetCurrentFrame()), SDL_FLIP_HORIZONTAL, -1.0);
	}
	else {
		App->render->Blit(data_player.Tex_Player, data_player.position.x, data_player.position.y, &(data_player.current_animation->GetCurrentFrame()), SDL_FLIP_NONE, -1.0);
	}


}

bool j1Player::Start() {

	
	//data_player.injump = false;
	
	Pushbacks();
	data_player.current_animation = &data_player.idle;


	App->audio->LoadFx(data_player.walkFX.GetString());
	App->audio->LoadFx(data_player.deathFX.GetString());

	data_player.Tex_Player = App->tex->Load(PATH(folder.GetString(), texture.GetString()));
	
	data_player.colliders = App->collider->AddCollider({ data_player.position.x, data_player.position.y, 43,53 }, COLLIDER_PLAYER, this);	//Collider player
	


	return	true;

}

bool j1Player::PreUpdate() {

	return true;

}

bool j1Player::Update(float dt) {

	data_player.position.y += data_player.gravity;

	CheckState();
	Animations();
	
	//Player Collider Draw
	data_player.colliders->SetPos(data_player.position.x, data_player.position.y);


	//Player Draw
	if (data_player.player_flip) {
		App->render->Blit(data_player.Tex_Player, data_player.position.x, data_player.position.y, &(data_player.current_animation->GetCurrentFrame()), SDL_FLIP_HORIZONTAL, -1.0);
	}
	else {
		App->render->Blit(data_player.Tex_Player, data_player.position.x, data_player.position.y, &(data_player.current_animation->GetCurrentFrame()), SDL_FLIP_NONE, -1.0);
	}

	
	return true;

}

bool j1Player::PostUpdate(float dt) {

	return true;
}

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
	
	data_player.gravity = node.child("gravity").attribute("grav").as_float();

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

	node.child("gravity").attribute("grav") = data_player.gravity;

	return true;

}

void j1Player::Pushbacks() {

	data_player.idle.PushBack({ 7,554,48,52 });
	data_player.idle.PushBack({ 84,554,41,52 });
	data_player.idle.PushBack({ 156,555,42,50 });
	data_player.idle.PushBack({ 232,557,41,48 });
	data_player.idle.PushBack({ 291,557,41,48 });
	data_player.idle.PushBack({ 351,557,40,48 });
	data_player.idle.PushBack({ 415,556,41,49 });
	data_player.idle.PushBack({ 474,556,42,49 });
	data_player.idle.PushBack({ 533,557,41,48 });
	data_player.idle.PushBack({ 591,554,41,51 });
	data_player.idle.PushBack({ 643,554,52,52 });
	data_player.idle.PushBack({ 705,555,44,50 });
	data_player.idle.PushBack({ 763,554,43,52 });
	data_player.idle.PushBack({ 819,554,45,52 });
	data_player.idle.PushBack({ 880,554,43,51 });
	data_player.idle.PushBack({ 938,555,43,51 });
	data_player.idle.PushBack({ 995,554,44,52 });
	data_player.idle.loop = true;
	data_player.idle.speed = 0.3f;

	data_player.walk.PushBack({ 7,4,42,49 });
	data_player.walk.PushBack({ 65,4,41,49 });
	data_player.walk.PushBack({ 131,3,38,50 });
	data_player.walk.PushBack({ 201,3,36,51 });
	data_player.walk.PushBack({ 259,5,38,48 });
	data_player.walk.PushBack({ 317,4,44,50 });
	data_player.walk.loop = true;
	data_player.walk.speed = 0.3f;

	data_player.walk2.PushBack({ 7,4,42,49 });
	data_player.walk2.PushBack({ 65,4,41,49 });
	data_player.walk2.PushBack({ 131,3,38,50 });
	data_player.walk2.PushBack({ 201,3,36,51 });
	data_player.walk2.PushBack({ 259,5,38,48 });
	data_player.walk2.PushBack({ 317,4,44,50 });
	data_player.walk2.loop = true;
	data_player.walk2.speed = 0.6f;

	data_player.jump.PushBack({ 7,128,52,43 });
	data_player.jump.PushBack({ 81,114,47,57 });
	data_player.jump.PushBack({ 155,113,42,58 });
	data_player.jump.PushBack({ 221,118,52,53 });
	data_player.jump.PushBack({ 285,116,53,55 });
	data_player.jump.PushBack({ 348,119,56,52 });
	data_player.jump.PushBack({ 423,129,53,42 });
	data_player.jump.loop = true;
	data_player.jump.speed = 0.5f;

	data_player.death.PushBack({ 8,464,48,55 });
	data_player.death.PushBack({ 72,456,62,63 });
	data_player.death.PushBack({ 150,454,69,65 });
	data_player.death.PushBack({ 257,464,82,56 });
	data_player.death.PushBack({ 367,447,83,72 });
	data_player.death.PushBack({ 488,447,84,73 });
	data_player.death.PushBack({ 609,457,94,62 });
	data_player.death.PushBack({ 734,455,113,65 });
	data_player.death.PushBack({ 894,445,127,74 });
	data_player.death.PushBack({ 1054,433,116,87 });
	data_player.death.loop = true;
	data_player.death.speed = 1.0f;


}

void j1Player::CheckState()
{

	
	

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && data_player.canjump == true) {		//if "D" is pressed animation walk forward 

		current_state = WALK;
		data_player.position.x += App->render->velcamera;
		data_player.player_flip = false;
		
	}

	else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && data_player.canjump == true) {		//if "A" is pressed animation walk backward actives flips to the Blit

		current_state = WALK;
		data_player.position.x -= App->render->velcamera;
		data_player.player_flip = true;
		
	}

	else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT && data_player.canjump == true) {		//if "A" and "SHIFT" are pressed animation walk backward actives flips to the Blit

		current_state = RUN;
		data_player.position.x -= App->render->velcamera;
		data_player.player_flip = true;
		
	}

	else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT && data_player.canjump == true) {


		current_state = RUN;
		data_player.position.x += App->render->velcamera;
		data_player.player_flip = false;
	}

	else if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN || (data_player.injump == true)) {		//if "SPACE" is pressed 

		data_player.canjump = false;
		current_state = JUMP_UP;

		data_player.current_animation = &data_player.jump;
		data_player.injump = true;
		float jumpspeed = 6.0f;
		data_player.position.y -= jumpspeed;
		jumpspeed -= 0.5;

		if (jumpspeed < -7)
		{
			data_player.injump = false;
			data_player.position.y = 220;
			jumpspeed = 6;
			data_player.current_animation = &data_player.idle;
		}

	}
	else if(data_player.canjump==true && App->input->GetKey(SDL_SCANCODE_SPACE) == NULL && App->input->GetKey(SDL_SCANCODE_A) == NULL && App->input->GetKey(SDL_SCANCODE_D) == NULL){
		current_state = IDLE;
		
	}

}

void j1Player::Animations() {

	if (current_state == IDLE) {
		data_player.current_animation = &data_player.idle;		//If any key pressed animation idle
	}

	if (current_state == WALK) {
		data_player.current_animation = &data_player.walk;		//If any key pressed animation idle
	}
		
	if (current_state == RUN) {
		data_player.current_animation = &data_player.walk2;		//If any key pressed animation idle
	}

}


iPoint j1Player::GetPosition() {

	return data_player.position;
}

void j1Player::OnCollision(Collider* c1, Collider* c2) {
	
	if (data_player.colliders == c1 && c2->type == COLLIDER_FLOOR)
	{
		data_player.injump = false;
		LOG("COLLIDERS WOOOOOOOOOOOOOOOOOOOOOORKS");
	}

	switch (c2->type)
	{
	case COLLIDER_FLOOR: // what happens when colliders collide 


		data_player.position.y = data_player.position.y - data_player.gravity;

		
		break;
	case COLLIDER_DEAD:

		break;
	default:
		break;
	}
}
