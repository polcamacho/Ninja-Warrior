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
#include "j1Scene.h"
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

	data_player.jumpvel = config.child("jump_velocity").attribute("jumpvel").as_int();

	data_player.vel_doublejump = config.child("vel_double_jump").attribute("veldbjump").as_int();

	data_player.v.x = config.child("velocity").attribute("x").as_int();

	//data_player.walkFX = config.child("walkFX").attribute("source").as_string();
	//data_player.deathFX = config.child("deathFX").attribute("source").as_string();

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
	
	data_player.colliders = App->collider->AddCollider({ data_player.position.x, data_player.position.y, 39,53 }, COLLIDER_PLAYER, this);	//Collider player
	
	globaltime = SDL_GetTicks();

	return	true;

}

bool j1Player::PreUpdate() {

	return true;

}

bool j1Player::Update(float dt) {

	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN) {

		if (godmode == false)
		{

			data_player.colliders->to_delete = true;
			data_player.colliders = App->collider->AddCollider({ data_player.position.x,data_player.position.y, 39,53 }, COLLIDER_NONE, this);
			data_player.gravity = 0;
			godmode = true;

		}
		else if (godmode == true)
		{

			data_player.colliders->to_delete = true;
			data_player.colliders = App->collider->AddCollider({ data_player.position.x,data_player.position.y, 39,53 }, COLLIDER_PLAYER, this);
			data_player.gravity = 20;
			godmode = false;
			
		}
	}
	
	if (godmode == true) {

		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
			data_player.position.y -= 20;

		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
			data_player.position.y += 20;

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
			data_player.position.x -= 20;

		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
			data_player.position.x += 20;
	}
	
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
	App->collider->CleanUp();
	
	return true;
}

bool j1Player::Load(pugi::xml_node& node) {

	if (!node.child("position").empty())
	{

		data_player.position.x = node.child("position").attribute("x").as_int();
		data_player.position.y = node.child("position").attribute("y").as_int();
	}

	else {
		data_player.position.x = data_player.position.x;
		data_player.position.y = data_player.position.y;
	}
	return true;

}
bool j1Player::Save(pugi::xml_node& node) const {

	if (node.child("position").empty())
	{
		pugi::xml_node&  save = node.append_child("position");
		save.append_attribute("x").set_value(data_player.position.x);
		save.append_attribute("y").set_value(data_player.position.y);
	}
	else
	{
		node.child("position").attribute("x").set_value(data_player.position.x);
		node.child("position").attribute("y").set_value(data_player.position.y);
	}
	

	return true;

}

void j1Player::Pushbacks() {

	data_player.idle.PushBack({ 7,554,48,52 }, 0.2, 0, 0);
	data_player.idle.PushBack({ 84,554,41,52 }, 0.2, 10, 0);
	data_player.idle.PushBack({ 156,555,42,50 }, 0.3, 10, 0);
	data_player.idle.PushBack({ 232,557,41,48 }, 0.3, 10, 0);
	data_player.idle.PushBack({ 291,557,41,48 }, 0.3, 10, 0);
	data_player.idle.PushBack({ 351,557,40,48 }, 0.3, 10, 0);
	data_player.idle.PushBack({ 415,556,41,49 }, 0.3, 10, 0);
	data_player.idle.PushBack({ 474,556,42,49 }, 0.3, 10, 0);
	data_player.idle.PushBack({ 533,557,41,48 }, 0.3, 10, 0);
	data_player.idle.PushBack({ 591,554,41,51 }, 0.2, 0, 0);
	data_player.idle.PushBack({ 643,554,52,52 }, 0.2, -1, 0);
	data_player.idle.PushBack({ 705,555,44,50 }, 0.2, -6, 0);
	data_player.idle.PushBack({ 763,554,43,52 }, 0.3, -20, 0);
	data_player.idle.PushBack({ 819,554,45,52 }, 0.3, -20, 0);
	data_player.idle.PushBack({ 880,554,43,51 }, 0.3, -20, 0);
	data_player.idle.PushBack({ 938,555,43,51 }, 0.3, -20, 0);
	data_player.idle.PushBack({ 995,554,44,52 }, 0.3, -20, 0);
	data_player.idle.loop = true;

	data_player.walk.PushBack({ 7,4,42,49 }, 0.2, 0, 0);
	data_player.walk.PushBack({ 65,4,41,49 }, 0.2, 0, 0);
	data_player.walk.PushBack({ 131,3,38,50 }, 0.2, 0, 0);
	data_player.walk.PushBack({ 201,3,36,51 }, 0.2, 0, 0);
	data_player.walk.PushBack({ 259,5,38,48 }, 0.2, 0, 0);
	data_player.walk.PushBack({ 317,4,44,50 }, 0.2, 0, 0);
	data_player.walk.loop = true;

	data_player.walk2.PushBack({ 7,4,42,49 }, 0.4, 0, 0);
	data_player.walk2.PushBack({ 65,4,41,49 }, 0.4, 0, 0);
	data_player.walk2.PushBack({ 131,3,38,50 }, 0.4, 0, 0);
	data_player.walk2.PushBack({ 201,3,36,51 }, 0.4, 0, 0);
	data_player.walk2.PushBack({ 259,5,38,48 }, 0.4, 0, 0);
	data_player.walk2.PushBack({ 317,4,44,50 }, 0.4, 0, 0);
	data_player.walk2.loop = true;

	data_player.jump.PushBack({ 7,128,52,43 }, 0.6, 0, 0);
	data_player.jump.PushBack({ 81,114,47,57 }, 0.3, 0, 0);
	data_player.jump.PushBack({ 155,113,42,58 }, 0.3, 0, 0);
	data_player.jump.PushBack({ 221,118,52,53 }, 0.2, 0, 0);
	data_player.jump.PushBack({ 285,116,53,55 }, 0.15, 0, 0);
	data_player.jump.PushBack({ 348,119,56,52 }, 0.05, 0, 0);
	data_player.jump.loop = false;


	data_player.fall.PushBack({ 348,119,56,52 }, 0.05, 0, 0);
	data_player.fall.PushBack({ 423,129,53,42 }, 0.5, 0, 0);
	data_player.fall.loop = true;

	data_player.death.PushBack({ 8,464,48,55 }, 0.15, 0, 0);
	data_player.death.PushBack({ 72,456,62,63 }, 0.15, 0, 0);
	data_player.death.PushBack({ 150,454,69,65 }, 0.15, 0, 0);
	data_player.death.PushBack({ 257,464,82,56 }, 0.15, 0, 0);
	data_player.death.PushBack({ 367,447,83,72 }, 0.15, 0, 0);
	data_player.death.PushBack({ 488,447,84,73 }, 0.15, 0, 0);
	data_player.death.PushBack({ 609,457,94,62 }, 0.15, 0, 0);
	data_player.death.PushBack({ 734,455,113,65 }, 0.15, 0, 0);
	data_player.death.PushBack({ 894,445,127,74 }, 0.15, 0, 0);
	data_player.death.PushBack({ 1054,433,116,87 }, 0.15, 0, 0);
	data_player.death.loop = false;

	

}

void j1Player::CheckState()
{
	
	
	if(godmode==false){
		
		data_player.velrun = (data_player.v.x)+0.25;
		
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && data_player.canjump == true) {		//if "D" is pressed animation walk forward 

			current_state = WALK;
			data_player.position.x += data_player.v.x;
			data_player.player_flip = false;
			if (App->scene->current_map == "Map.tmx") {
				App->audio->PlayFx(App->scene->walk_FX);
			}
			else
			{

			}
		
			if (App->input->GetKey(SDL_SCANCODE_SPACE)==KEY_DOWN) {		//if  "SPACE" is pressed when "D" is pressed, the player jumps forward

				data_player.position.x += data_player.v.x;
				current_state = JUMP_WALK;
				App->audio->PlayFx(App->scene->jump_FX);

				if (data_player.canjump == true) {
					data_player.jumpenergy = data_player.jumpvel;
				}
				data_player.jumpCounter--;
				LOG("%i", data_player.jumpCounter);

				if (data_player.jumpCounter == 0) {
					data_player.jump.Reset();
				}

			}

			if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) {		//if  "LSHIFT" is pressed when "D" is pressed, the player runs forward
				current_state = RUN;
				data_player.position.x += data_player.velrun;
				data_player.player_flip = false;


				if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {		//if "SPACE" is pressed when "LSHIFT" is pressed, and when "D" is pressed, the player jumps running forward
					data_player.jumpCounter = 2;

					data_player.right = true;
					data_player.left = false;
					App->audio->PlayFx(App->scene->jump_FX);


					current_state = JUMP_RUN;

					data_player.jumpCounter--;
					LOG("%i", data_player.jumpCounter);

					if (data_player.jumpCounter == 0) {
						data_player.jump.Reset();
					}
				}

			}
		}

		else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && data_player.canjump == true) {		//if "A" is pressed animation walk backward actives flips to the Blit

			current_state = WALK;
			data_player.position.x -= data_player.v.x;
			data_player.player_flip = true;
		
			if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {		//if "SPACE" is pressed 

				data_player.position.x -= data_player.v.x;
				current_state = JUMP_WALK;
				App->audio->PlayFx(App->scene->jump_FX);

				if (data_player.canjump == true) {
					data_player.jumpenergy = data_player.jumpvel;
				}

				data_player.jumpCounter--;
				LOG("%i", data_player.jumpCounter);

				if (data_player.jumpCounter == 0) {
					data_player.jump.Reset();
				}
			}
			if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) {
				current_state = RUN;
				data_player.position.x -= data_player.velrun;
			
				data_player.player_flip = true;

				if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {		//if "SPACE" is pressed 

					data_player.jumpCounter = 2;
					data_player.left = true;
					data_player.right = false;
					App->audio->PlayFx(App->scene->jump_FX);

					current_state = JUMP_RUN;

					data_player.jumpCounter--;
					LOG("%i", data_player.jumpCounter);

					if (data_player.jumpCounter == 0) {
						data_player.jump.Reset();
					}
				}
			}
		}


		else if ((App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) && (data_player.jumpCounter!=0)) {		//if "SPACE" is pressed 
			
				current_state = JUMP_UP;
				data_player.player_flip = false;
				data_player.jumpenergy = data_player.jumpvel;
				data_player.jumpCounter--;
				LOG("%i", data_player.jumpCounter);
				App->audio->PlayFx(App->scene->jump_FX);

				
				if(data_player.jumpCounter==0){
					data_player.jump.Reset();
				}

		}


		else if(data_player.canjump==true && App->input->GetKey(SDL_SCANCODE_SPACE) == NULL && App->input->GetKey(SDL_SCANCODE_A) == NULL && App->input->GetKey(SDL_SCANCODE_D) == NULL){
			current_state = IDLE;
			data_player.player_flip = false;

			

		}
	}
}

void j1Player::Animations() {

	if (current_state == IDLE) {
		data_player.current_animation = &data_player.idle;		//If any key pressed animation idle
		data_player.jump.Reset();
		data_player.fall.Reset();
		
	}

	if (current_state == WALK) {
		data_player.current_animation = &data_player.walk;		//If any key pressed animation idle
		data_player.jump.Reset();
		data_player.fall.Reset();
		
	}
	if(current_state==JUMP_WALK){

		current_state = JUMP_UP;

	}

	if (current_state == JUMP_RUN) {
		

		if (data_player.left == true) {
			data_player.position.x -= data_player.velrun;
		}
		else if (data_player.right == true) {
			data_player.position.x += data_player.velrun;
		}

		data_player.canjump = false;
		data_player.injump = true;
		data_player.current_animation = &data_player.jump;

		if (data_player.jumpenergy <= data_player.gravity) {
			data_player.jumpenergy += 0.5;
			data_player.position.y = data_player.position.y + data_player.jumpenergy;

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
		
	if (current_state == RUN) {
		data_player.current_animation = &data_player.walk2;		//If any key pressed animation idle
		data_player.jump.Reset();
		data_player.fall.Reset();
		
	}

	if (current_state == JUMP_UP) {


		data_player.canjump = false;
		data_player.injump = true;
		data_player.current_animation = &data_player.jump;

		if (data_player.jumpenergy <= data_player.gravity) {
			data_player.jumpenergy += 0.5;
			data_player.position.y = data_player.position.y + data_player.jumpenergy;

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

	if (current_state == JUMP_FALL) {

		
		data_player.current_animation = &data_player.fall;
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

	/*if (current_state == DOUBLE_JUMP) {
		data_player.jump.Reset();
		current_state = JUMP_UP;
		data_player.position.y = data_player.position.y + data_player.jumpenergy*(2/3);

	}*/

	if (current_state == DEATH) {
		data_player.current_animation = &data_player.death;		//If any key pressed animation idle
	}
	
	

}

void j1Player::OnCollision(Collider* c1, Collider* c2) {

	if (c1->type == ColliderType::COLLIDER_PLAYER && c2->type == ColliderType::COLLIDER_FLOOR) {

		//ABOVE
		if (data_player.preposition.y < c2->rect.y || data_player.position.y == c2->rect.y - data_player.colliders->rect.h) {

			data_player.position.y = c2->rect.y - data_player.colliders->rect.h;
			data_player.grounded = true;
			data_player.canjump = true;

			if (data_player.injump == true) {
				data_player.jump.Reset();
			}
			if (data_player.grounded == true) {
				data_player.jumpCounter = 2;
			}

		}

		//BELOW
		else if (data_player.preposition.y > (c2->rect.y + c2->rect.h)) {
			data_player.position.y = c2->rect.y + c2->rect.h;
			current_state = JUMP_FALL;
		}
		//SIDES
		else if ((data_player.position.x < c2->rect.x + c2->rect.w && data_player.position.x > c2->rect.x) ||


			(data_player.position.x + data_player.colliders->rect.w < c2->rect.x + c2->rect.w && data_player.position.x + data_player.colliders->rect.w > c2->rect.x)) {
			LOG("WALL");
			if ((data_player.position.x + data_player.colliders->rect.w) < (c2->rect.x + c2->rect.w)) { //Player to the left 
				data_player.position.x = c2->rect.x - data_player.colliders->rect.w;
			}
			else if (data_player.position.x < (c2->rect.x + c2->rect.w)) {
				data_player.position.x = c2->rect.x + c2->rect.w;
			}
		}

	}

	if (c1->type == ColliderType::COLLIDER_PLAYER && c2->type == ColliderType::COLLIDER_PLATFORM) {



		if (data_player.preposition.y < c2->rect.y || data_player.position.y == c2->rect.y - data_player.colliders->rect.h) {

			data_player.position.y = c2->rect.y - data_player.colliders->rect.h;
			data_player.grounded = true;
			data_player.canjump = true;

			//current_state = IDLE;

			if (data_player.grounded == true) {
				data_player.jumpCounter = 2;
			}
		}
		else if ((data_player.position.y >= data_player.preposition.y) && (data_player.preposition.y + data_player.colliders->rect.h) < c2->rect.y) {
			data_player.position.y = c2->rect.y - data_player.colliders->rect.h;

		}


	}

	if (c1->type == ColliderType::COLLIDER_PLAYER && c2->type == ColliderType::COLLIDER_DEAD) {

		if (data_player.preposition.y < c2->rect.y || data_player.position.y == c2->rect.y - data_player.colliders->rect.h) {

			pretime = SDL_GetTicks();

			data_player.position.y = c2->rect.y - data_player.colliders->rect.h;
			current_state = DEATH;
			data_player.grounded = true;
			data_player.canjump = false;
			die = true;

		}

		else if (data_player.preposition.y > (c2->rect.y + c2->rect.h)) {

			pretime = SDL_GetTicks();
			data_player.position.y = c2->rect.y + c2->rect.h;
			current_state = DEATH;

			data_player.grounded = true;
			data_player.canjump = false;
			die = true;



		}

		if (die == true) {

			if (App->scene->current_map == "Map.tmx") {

				if (pretime >= globaltime + 5000) {

					data_player.position.x = 100;
					data_player.position.y = 300;

				}

			}


			else {

				if (pretime >= globaltime + 3500) {

					data_player.position.x = 55;
					data_player.position.y = 100;

				}

			}


			Reset();

			die = false;
		}


	}



	if (c1->type == ColliderType::COLLIDER_PLAYER && c2->type == ColliderType::COLLIDER_NEXT) {

		//ABOVE
		if (data_player.preposition.y < c2->rect.y || data_player.position.y == c2->rect.y - data_player.colliders->rect.h) {
			App->scene->SecondMap();
		}
		//BELOW
		else if (data_player.preposition.y > (c2->rect.y + c2->rect.h)) {
			App->scene->SecondMap();
		}
		//SIDES
		else if ((data_player.position.x < c2->rect.x + c2->rect.w && data_player.position.x > c2->rect.x) || (data_player.position.x + data_player.colliders->rect.w < c2->rect.x + c2->rect.w && data_player.position.x + data_player.colliders->rect.w > c2->rect.x)) {
			
			if ((data_player.position.x + data_player.colliders->rect.w) < (c2->rect.x + c2->rect.w)) { //Player to the left 
				App->scene->SecondMap();
			}
			else if (data_player.position.x < (c2->rect.x + c2->rect.w)) {
				App->scene->SecondMap();
			}
		}
	}

	if (data_player.grounded == false) {
		
		current_state = JUMP_FALL;
		
	}

}


void j1Player::Reset() {

	data_player.death.Reset();
	data_player.jump.Reset();
	data_player.walk.Reset();
	data_player.walk2.Reset();
	data_player.idle.Reset();
	data_player.fall.Reset();
	data_player.running.Reset();
	die = false;
}
