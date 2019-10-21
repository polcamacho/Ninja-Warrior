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

	data_player.jumpvel = config.child("jump_velocity").attribute("jumpvel").as_int();

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
	data_player.preposition = data_player.position;
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

	data_player.jumpvel = node.child("jump_velocity").attribute("jumpvel").as_int();

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

	node.append_child("jump_velocity").append_attribute("jumpvel") = data_player.jumpvel;

	node.append_child("velocity").append_attribute("x") = data_player.v.x;
	node.child("velocity").attribute("y") = data_player.v.y;

	node.child("gravity").attribute("grav") = data_player.gravity;

	return true;

}

void j1Player::Pushbacks() {

	data_player.idle.PushBack({ 7,554,48,52 }, 0.4, 0, 0);
	data_player.idle.PushBack({ 84,554,41,52 }, 0.4, 10, 0);
	data_player.idle.PushBack({ 156,555,42,50 }, 0.6, 10, 0);
	data_player.idle.PushBack({ 232,557,41,48 }, 0.6, 10, 0);
	data_player.idle.PushBack({ 291,557,41,48 }, 0.6, 10, 0);
	data_player.idle.PushBack({ 351,557,40,48 }, 0.6, 10, 0);
	data_player.idle.PushBack({ 415,556,41,49 }, 0.6, 10, 0);
	data_player.idle.PushBack({ 474,556,42,49 }, 0.6, 10, 0);
	data_player.idle.PushBack({ 533,557,41,48 }, 0.6, 10, 0);
	data_player.idle.PushBack({ 591,554,41,51 }, 0.4, 0, 0);
	data_player.idle.PushBack({ 643,554,52,52 }, 0.6, -1, 0);
	data_player.idle.PushBack({ 705,555,44,50 }, 0.6, -6, 0);
	data_player.idle.PushBack({ 763,554,43,52 }, 0.6, -20, 0);
	data_player.idle.PushBack({ 819,554,45,52 }, 0.6, -20, 0);
	data_player.idle.PushBack({ 880,554,43,51 }, 0.6, -20, 0);
	data_player.idle.PushBack({ 938,555,43,51 }, 0.6, -20, 0);
	data_player.idle.PushBack({ 995,554,44,52 }, 0.6, -20, 0);
	data_player.idle.loop = true;

	data_player.walk.PushBack({ 7,4,42,49 }, 0.3, 0, 0);
	data_player.walk.PushBack({ 65,4,41,49 }, 0.3, 0, 0);
	data_player.walk.PushBack({ 131,3,38,50 }, 0.3, 0, 0);
	data_player.walk.PushBack({ 201,3,36,51 }, 0.3, 0, 0);
	data_player.walk.PushBack({ 259,5,38,48 }, 0.3, 0, 0);
	data_player.walk.PushBack({ 317,4,44,50 }, 0.3, 0, 0);
	data_player.walk.loop = true;

	data_player.walk2.PushBack({ 7,4,42,49 }, 0.6, 0, 0);
	data_player.walk2.PushBack({ 65,4,41,49 }, 0.6, 0, 0);
	data_player.walk2.PushBack({ 131,3,38,50 }, 0.6, 0, 0);
	data_player.walk2.PushBack({ 201,3,36,51 }, 0.6, 0, 0);
	data_player.walk2.PushBack({ 259,5,38,48 }, 0.6, 0, 0);
	data_player.walk2.PushBack({ 317,4,44,50 }, 0.6, 0, 0);
	data_player.walk2.loop = true;

	data_player.jump.PushBack({ 7,128,52,43 }, 0.5, 0, 0);
	data_player.jump.PushBack({ 81,114,47,57 }, 0.5, 0, 0);
	data_player.jump.PushBack({ 155,113,42,58 }, 0.5, 0, 0);
	data_player.jump.PushBack({ 221,118,52,53 }, 0.5, 0, 0);
	data_player.jump.PushBack({ 285,116,53,55 }, 0.5, 0, 0);
	data_player.jump.PushBack({ 348,119,56,52 }, 0.5, 0, 0);
	data_player.jump.PushBack({ 423,129,53,42 }, 0.5, 0, 0);
	data_player.jump.loop = true;

	data_player.death.PushBack({ 8,464,48,55 }, 0.5, 0, 0);
	data_player.death.PushBack({ 72,456,62,63 }, 0.5, 0, 0);
	data_player.death.PushBack({ 150,454,69,65 }, 0.5, 0, 0);
	data_player.death.PushBack({ 257,464,82,56 }, 0.5, 0, 0);
	data_player.death.PushBack({ 367,447,83,72 }, 0.5, 0, 0);
	data_player.death.PushBack({ 488,447,84,73 }, 0.5, 0, 0);
	data_player.death.PushBack({ 609,457,94,62 }, 0.5, 0, 0);
	data_player.death.PushBack({ 734,455,113,65 }, 0.5, 0, 0);
	data_player.death.PushBack({ 894,445,127,74 }, 0.5, 0, 0);
	data_player.death.PushBack({ 1054,433,116,87 }, 0.5, 0, 0);
	data_player.death.loop = false;


}

void j1Player::CheckState()
{
	data_player.velrun = data_player.v.x*5;
	
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && data_player.canjump == true) {		//if "D" is pressed animation walk forward 

		current_state = WALK;
		data_player.position.x += data_player.v.x;
		data_player.player_flip = false;
		
	}

	else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && data_player.canjump == true) {		//if "A" is pressed animation walk backward actives flips to the Blit

		current_state = WALK;
		data_player.position.x -= data_player.v.x;
		data_player.player_flip = true;
		
	}

	else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT && data_player.canjump == true) {		//if "A" and "SHIFT" are pressed animation walk backward actives flips to the Blit

		current_state = RUN;
		data_player.position.x -= data_player.velrun;
		data_player.player_flip = true;
		
	}

	else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT && data_player.canjump == true) {


		current_state = RUN;
		data_player.position.x += data_player.velrun;
		data_player.player_flip = false;
	}

	else if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {		//if "SPACE" is pressed 

		current_state = JUMP_UP;

		if (data_player.canjump == true) {
			data_player.jumpenergy = data_player.jumpvel;
		}



	}
	else if(data_player.canjump==true && App->input->GetKey(SDL_SCANCODE_SPACE) == NULL && App->input->GetKey(SDL_SCANCODE_A) == NULL && App->input->GetKey(SDL_SCANCODE_D) == NULL){
		current_state = IDLE;
		data_player.player_flip = false;

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

	if (current_state == JUMP_UP) {

		data_player.canjump = false;
		data_player.injump = true;
		data_player.current_animation = &data_player.jump;

		if (data_player.jumpenergy < data_player.gravity) {
			data_player.jumpenergy += 0.5;
			data_player.position.y = data_player.position.y + data_player.jumpenergy;
		}
		else if (data_player.jumpenergy == data_player.gravity) {
			data_player.position.y = data_player.position.y + data_player.gravity;
		}
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			data_player.position.x -= data_player.v.x;
			data_player.player_flip = true;
		}
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			data_player.position.x += data_player.v.x;
			data_player.player_flip = false;
		}
	}

	if (current_state == DEATH) {
		data_player.current_animation = &data_player.death;		//If any key pressed animation idle
	}

}


void j1Player::OnCollision(Collider* c1, Collider* c2) {
	
	/*if (data_player.colliders == c1 && c2->type == COLLIDER_FLOOR)
	{
		data_player.canjump = true;
		LOG("COLLIDERS WOOOOOOOOOOOOOOOOOOOOOORKS");
	}

	switch (c2->type)
	{
	case COLLIDER_FLOOR: // what happens when colliders collide 


		data_player.position.y = data_player.position.y - data_player.gravity;

		
		break;
	case COLLIDER_DEAD:

		data_player.current_animation = &data_player.death;
		

		break;
	default:
		break;
	}*/

	if (c1->type == ColliderType:: COLLIDER_PLAYER && c2->type == ColliderType::COLLIDER_FLOOR) {

		//from above
		if (data_player.preposition.y < c2->rect.y || data_player.position.y == c2->rect.y - data_player.colliders->rect.h) {
			data_player.position.y = c2->rect.y - data_player.colliders->rect.h;
			data_player.grounded = true;
			data_player.canjump=true;
		}
		//from below
		else if (data_player.preposition.y > (c2->rect.y + c2->rect.h)) {
			data_player.position.y = c2->rect.y + c2->rect.h;
		}
		//from a side
		else if ((data_player.position.x < c2->rect.x + c2->rect.w && data_player.position.x > c2->rect.x) ||
			(data_player.position.x + data_player.colliders->rect.w < c2->rect.x + c2->rect.w && data_player.position.x + data_player.colliders->rect.w > c2->rect.x)) {
			LOG("WALL");
			if ((data_player.position.x + data_player.colliders->rect.w) < (c2->rect.x + c2->rect.w / 2)) { //Player to the left 
				data_player.position.x = c2->rect.x - data_player.colliders->rect.w;
			}
			else if (data_player.position.x < (c2->rect.x + c2->rect.w)) {
				data_player.position.x = c2->rect.x + c2->rect.w;
			}
		}
	}


	if (c1->type == ColliderType::COLLIDER_PLAYER && c2->type == ColliderType::COLLIDER_PLATFORM) {

		if (data_player.platformdrop == false) {
			if (data_player.preposition.y < c2->rect.y || data_player.position.y == c2->rect.y - data_player.colliders->rect.h) {
				data_player.position.y = c2->rect.y - data_player.colliders->rect.h;
				data_player.grounded = true;
				data_player.canjump = true;
			}
			else if ((data_player.position.y >= data_player.preposition.y) && (data_player.preposition.y + data_player.colliders->rect.h) < c2->rect.y) {
				data_player.position.y = c2->rect.y - data_player.colliders->rect.h;
				//data_player.grounded=true;
				//data_player.canjump = false;
			}
		}

	}

	if (c1->type == ColliderType::COLLIDER_PLAYER && c2->type == ColliderType::COLLIDER_DEAD) {

		if (data_player.preposition.y < c2->rect.y || data_player.position.y == c2->rect.y - data_player.colliders->rect.h) {
			
			current_state = DEATH;
			
			data_player.position.y = c2->rect.y - data_player.colliders->rect.h;
			data_player.grounded = false;
			data_player.canjump = false;

			data_player.position.x = 100;
			data_player.position.y = 300;
			

		}

	}

}
