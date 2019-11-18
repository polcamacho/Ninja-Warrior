#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Audio.h"
#include "j1Textures.h"
#include "j1Enemy.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Input.h"
#include "j1Collider.h"
#include "j1Scene.h"
#include <math.h>

j1Enemy::j1Enemy() : j1Module()
{
	name.create("enemy");
	
}

// Destructor
j1Enemy::~j1Enemy()
{}

bool j1Enemy::Awake(pugi::xml_node& config) {

	bool ret = true;

	//Load All Player Features from Config

	folder.create(config.child("folder").child_value());	
	texture = config.child("texture").attribute("source").as_string();

	data_enemy.position.x = config.child("position").attribute("x").as_int();
	data_enemy.position.y = config.child("position").attribute("y").as_int();

	data_enemy.v.x = config.child("velocity").attribute("x").as_int();
		
	data_enemy.gravity = config.child("gravity").attribute("grav").as_float();

	return ret;

}

void j1Enemy::DrawEnemy()
{
	

	if (data_enemy.player_flip) {
		App->render->Blit(data_enemy.Tex_Player, data_enemy.position.x, data_enemy.position.y, &(data_enemy.current_animation->GetCurrentFrame()), SDL_FLIP_HORIZONTAL, 0);	//Draw Player Flipped
	}
	else {
		App->render->Blit(data_enemy.Tex_Player, data_enemy.position.x, data_enemy.position.y, &(data_enemy.current_animation->GetCurrentFrame()), SDL_FLIP_NONE, 0);	//Draw Player Normal
	}


}

bool j1Enemy::Start() {

	pretimer = 0;
	//globaltime = SDL_GetTicks();	//Sets the Global time to the death timer
	
	Pushbacks();	//Call all the Pushback of animations
	
	data_enemy.Tex_Player = App->tex->Load(PATH(folder.GetString(), texture.GetString()));	//Load The Texture of player
	LOG("%s", data_enemy.Tex_Player);
	data_enemy.colliders = App->collider->AddCollider({ data_enemy.position.x, data_enemy.position.y, 39,53 }, COLLIDER_ENEMY, this);	//Sets The Collider Type and Dimensions to Player
	
	return	true;

}

bool j1Enemy::PreUpdate(float dt) {

	return true;

}

bool j1Enemy::Update(float dt) {

	

	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) {

		if (godmode == false)	//If godmode is false sets None Collider to player for he can fly around map and not collide
		{

			data_enemy.gravity = 0 * (dt * LIMIT_TIMER);	//Sets new gravity to player for he can move around map
			godmode = true;	

		}
		else if (godmode == true)
		{

			data_enemy.gravity = 20 * (dt * LIMIT_TIMER);	//Sets normal gravity to player 
			godmode = false;
			
		}
	}
	
	data_enemy.position.y += data_enemy.gravity;
	data_enemy.preposition = data_enemy.position;
	
	CheckState(dt);	//Checks the state where is the player
	State(dt);	//Set the animation relationed with the state that he is
	
	//Player Collider Draw
	data_enemy.colliders->SetPos(data_enemy.position.x, data_enemy.position.y);	//Sets the Player Collider Position

	//Player Draw
	if (data_enemy.player_flip) {
		App->render->Blit(data_enemy.Tex_Player, data_enemy.position.x, data_enemy.position.y, &(data_enemy.current_animation->GetCurrentFrame()), SDL_FLIP_HORIZONTAL, 1.0);	//Draw Player Flipped
	}
	else {
		App->render->Blit(data_enemy.Tex_Player, data_enemy.position.x, data_enemy.position.y, &(data_enemy.current_animation->GetCurrentFrame()), SDL_FLIP_NONE, 1.0);	//Draw Player Normal
	}

	if (data_enemy.grounded == false) {	//Sets that if the player is not touching the ground puts the fall animation

		current_state = JUMP_FALL2;

	}
		
	return true;

	data_enemy.current_animation = &data_enemy.idle;

}

bool j1Enemy::PostUpdate(float dt) {

	return true;
}
// Called before quitting
bool j1Enemy::CleanUp()
{
	LOG("Unloading player");

	App->tex->UnLoad(data_enemy.Tex_Player);	//Unload The Player texture
	App->collider->CleanUp();	//Unload the Player collider
	
	return true;
}

bool j1Enemy::Load(pugi::xml_node& node) {

	if (!node.child("position").empty())	//Load the valors if save_confign file is empty link
	{

		data_enemy.position.x = node.child("position").attribute("x").as_int();	//Load Player X
		data_enemy.position.y = node.child("position").attribute("y").as_int();	//Load Player Y
	}

	else {	
		data_enemy.position.x = data_enemy.position.x;	//Load Player X
		data_enemy.position.y = data_enemy.position.y;	//Load Player Y
	}
	return true;

}
bool j1Enemy::Save(pugi::xml_node& node) const {

	if (node.child("position").empty())	//Save The Position Valors in the save_config file
	{
		pugi::xml_node&  save = node.append_child("position");	
		save.append_attribute("x").set_value(data_enemy.position.x);	//Save Player X
		save.append_attribute("y").set_value(data_enemy.position.y);	//Save Player Y
	}
	else
	{
		node.child("position").attribute("x").set_value(data_enemy.position.x);	//Save Player X
		node.child("position").attribute("y").set_value(data_enemy.position.y);	//Save Player Y
	}
	
	return true;

}

void j1Enemy::Pushbacks() {

	//Sets the Idle Animation
	data_enemy.idle.PushBack({ 0,0,43,63 }, 0.4, 0, 0);
	data_enemy.idle.PushBack({ 46,0,41,64 }, 0.4, -2, 0);
	data_enemy.idle.PushBack({ 92,0,41,63 }, 0.4, -2, 0);
	data_enemy.idle.PushBack({ 136,0,45,64 }, 0.4, -4, 0);
	data_enemy.idle.PushBack({ 182,0,42,63 }, 0.4, -4, 0);
	data_enemy.idle.PushBack({ 228,0,42,63 }, 0.4, -2, 0);
	data_enemy.idle.PushBack({ 274,0,42,62 }, 0.4, -4, 0);
	data_enemy.idle.PushBack({ 318,0,42,64 }, 0.4, -4, 0);
	data_enemy.idle.PushBack({ 365,0,42,64 }, 0.4, -4, 0);
	data_enemy.idle.PushBack({ 410,0,44,63 }, 0.4, -2, 0);
	data_enemy.idle.PushBack({ 456,0,43,63 }, 0.4, -2, 0);
	data_enemy.idle.PushBack({ 502,0,41,63 }, 0.4, -2, 0);
	data_enemy.idle.PushBack({ 546,0,43,63 }, 0.4, -4, 0);
	data_enemy.idle.PushBack({ 593,0,41,63 }, 0.4, -4, 0);
	data_enemy.idle.PushBack({ 640,0,44,63 }, 0.4, -4, 0);
	data_enemy.idle.PushBack({ 687,0,42,63 }, 0.4, -4, 0);
	data_enemy.idle.PushBack({ 731,0,44,63 }, 0.4, -4, 0);
	data_enemy.idle.PushBack({ 777,0,43,63 }, 0.4, -4, 0);
	data_enemy.idle.loop = true;

	//Sets the Walk Animation
	data_enemy.walk.PushBack({ 0,76,43,63 }, 0.4, 0, 0);
	data_enemy.walk.PushBack({ 42,76,43,63 }, 0.4, 0, 0);
	data_enemy.walk.PushBack({ 88,76,43,64 }, 0.4, 0, 0);
	data_enemy.walk.PushBack({ 132,76,43,64 }, 0.4, 0, 0);
	data_enemy.walk.PushBack({ 178,76,42,63 }, 0.4, 0, 0);
	data_enemy.walk.PushBack({ 221,76,45,64 }, 0.4, 0, 0);
	data_enemy.walk.PushBack({ 267,76,44,62 }, 0.4, 0, 0);
	data_enemy.walk.PushBack({ 312,76,44,63 }, 0.4, 0, 0);
	data_enemy.walk.PushBack({ 357,76,44,63 }, 0.4, 0, 0);
	data_enemy.walk.PushBack({ 403,76,43,63 }, 0.4, 0, 0);
	data_enemy.walk.PushBack({ 447,76,43,64 }, 0.4, 0, 0);
	data_enemy.walk.PushBack({ 492,76,43,64 }, 0.4, 0, 0);
	data_enemy.walk.PushBack({ 537,76,43,64 }, 0.4, 0, 0);
	data_enemy.walk.PushBack({ 582,76,41,64 }, 0.4, 0, 0);
	data_enemy.walk.PushBack({ 626,76,44,64 }, 0.4, 0, 0);
	data_enemy.walk.PushBack({ 672,76,43,64 }, 0.4, 0, 0);
	data_enemy.walk.PushBack({ 719,76,42,64 }, 0.4, 0, 0);
	data_enemy.walk.PushBack({ 764,76,42,64 }, 0.4, 0, 0);
	data_enemy.walk.PushBack({ 809,76,43,63 }, 0.4, 0, 0);
	data_enemy.walk.PushBack({ 853,76,44,62 }, 0.4, 0, 0);
	data_enemy.walk.PushBack({ 899,76,43,62 }, 0.4, 0, 0);
	data_enemy.walk.PushBack({ 943,76,44,63 }, 0.4, 0, 0);
	data_enemy.walk.PushBack({ 988,76,44,64 }, 0.4, 0, 0);
	data_enemy.walk.PushBack({ 1033,76,45,64 }, 0.4, 0, 0);
	data_enemy.walk.loop = true;

	//Sets the Attack Animation
	data_enemy.attack.PushBack({ 0,152,44,63 }, 0.3, 0, 0);
	data_enemy.attack.PushBack({ 46,152,45,63 }, 0.3, 0, 0);
	data_enemy.attack.PushBack({ 95,152,69,65 }, 0.3, 0, 0);
	data_enemy.attack.PushBack({ 168,152,67,66 }, 0.3, 0, 0);
	data_enemy.attack.PushBack({ 238,152,69,67 }, 0.3, 0, 0);
	data_enemy.attack.PushBack({ 311,152,63,65 }, 0.3, 0, 0);
	data_enemy.attack.PushBack({ 382,152,66,64 }, 0.3, 0, 0);
	data_enemy.attack.PushBack({ 453,152,67,64 }, 0.3, 0, 0);
	data_enemy.attack.PushBack({ 522,152,64,66 }, 0.3, 0, 0);
	data_enemy.attack.PushBack({ 593,152,64,65 }, 0.3, 0, 0);
	data_enemy.attack.PushBack({ 662,152,61,65 }, 0.3, 0, 0);
	data_enemy.attack.PushBack({ 729,152,64,64 }, 0.3, 0, 0);
	data_enemy.attack.loop = false;

	//Sets the Hurt Animation
	data_enemy.hurt.PushBack({ 0,228,42,64 }, 0.3, 0, 0);
	data_enemy.hurt.PushBack({ 47,228,44,65 }, 0.3, 0, 0);
	data_enemy.hurt.PushBack({ 97,228,44,66 }, 0.3, 0, 0);
	data_enemy.hurt.PushBack({ 147,228,45,64 }, 0.3, 0, 0);
	data_enemy.hurt.PushBack({ 196,228,52,63 }, 0.3, 0, 0);
	data_enemy.hurt.PushBack({ 253,228,55,61 }, 0.3, 0, 0);
	data_enemy.hurt.PushBack({ 315,228,52,61 }, 0.3, 0, 0);
	data_enemy.hurt.PushBack({ 373,228,50,61 }, 0.3, 0, 0);
	data_enemy.hurt.PushBack({ 430,228,47,62 }, 0.3, 0, 0);
	data_enemy.hurt.PushBack({ 484,228,45,62 }, 0.3, 0, 0);
	data_enemy.hurt.PushBack({ 535,228,44,61 }, 0.3, 0, 0);
	data_enemy.hurt.PushBack({ 585,228,43,62 }, 0.3, 0, 0);
	data_enemy.hurt.loop = false;

	//Sets the Death Animation
	data_enemy.death.PushBack({ 0,305,42,64 }, 0.3, 0, 0);
	data_enemy.death.PushBack({ 47,305,44,65 }, 0.3, 0, 1);
	data_enemy.death.PushBack({ 102,305,43,65 }, 0.3, 0, 2);
	data_enemy.death.PushBack({ 155,305,43,65 }, 0.3, 0, 4);
	data_enemy.death.PushBack({ 207,305,44,64 }, 0.3, 0, 8);
	data_enemy.death.PushBack({ 256,305,50,60 }, 0.3, 0, 10);
	data_enemy.death.PushBack({ 313,305,59,56 }, 0.3, 0, 15);
	data_enemy.death.PushBack({ 381,305,50,61 }, 0.3, 0, 20);
	data_enemy.death.PushBack({ 451,305,64,46 }, 0.3, 0, 20);
	data_enemy.death.PushBack({ 525,305,66,42 }, 0.3, 0, 20);
	data_enemy.death.PushBack({ 601,305,66,42 }, 0.3, 0, 20);
	data_enemy.death.PushBack({ 673,305,65,41 }, 0.3, 0, 20);
	data_enemy.death.PushBack({ 749,305,64,41 }, 0.3, 0, 20);

	data_enemy.death.loop = false;


}

void j1Enemy::CheckState(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT|| App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		data_enemy.current_animation = &data_enemy.walk;
	}
	if (App->input->GetKey(SDL_SCANCODE_X) == KEY_REPEAT) {
		data_enemy.current_animation = &data_enemy.attack;
		data_enemy.attack.Reset();
	}
	if (App->input->GetKey(SDL_SCANCODE_V) == KEY_REPEAT) {
		data_enemy.current_animation = &data_enemy.death;
		data_enemy.death.Reset();

	}
	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT) {
		data_enemy.current_animation = &data_enemy.hurt;
		data_enemy.hurt.Reset();

	}

}

void j1Enemy::State(float dt) {

	if (current_state == IDLE2) {
		
		data_enemy.current_animation = &data_enemy.idle;
		//data_enemy.jump.Reset();
		//data_enemy.fall.Reset();
		
	}

	if (current_state == WALK2) {

		data_enemy.current_animation = &data_enemy.walk;
		///data_enemy.jump.Reset();
		//data_enemy.fall.Reset();
		
	}
	
	if (current_state == JUMP_UP2) {

		data_enemy.canjump = false;
		data_enemy.injump = true;
		//data_enemy.current_animation = &data_enemy.jump;

		if (data_enemy.jumpenergy <= data_enemy.gravity) {									//character will jump up until it do not accomplish this condition 
			
			data_enemy.jumpenergy += 0.5;														// jump up increments 0.5 each time
			data_enemy.position.y = data_enemy.position.y + (data_enemy.jumpenergy *(dt * LIMIT_TIMER));			// y position increments 0.5 each time

		}

				
	}
	
	if (current_state == JUMP_FALL2) {

		/*data_enemy.current_animation = &data_enemy.fall;
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			data_enemy.position.x -= data_enemy.v.x*(dt * LIMIT_TIMER);
			data_enemy.player_flip = true;
		}
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			data_enemy.position.x += data_enemy.v.x*(dt * LIMIT_TIMER);
			data_enemy.player_flip = false;
			
		}*/
	}

	if (current_state == DEATH2) {	
		
		die = true;	//Sets the die to true
		LOG("GLOBAL: %d", globaltime);
		LOG("PRE: %d", pretimer);
		if (die == true) {

			if (App->scene->current_map == "Map.tmx") {	//If player is in map 1
				
				

				if (pretime (20)) {	//Do a timer to stop the game during the Death animation
					

					
					//Sets the Position that player goes when he dies
					data_enemy.position.x = 100;	//Set Player X	
					data_enemy.position.y = 300;	//Set Player Y
					current_state = JUMP_FALL2;	//Sets the Animation when he reapears
					data_enemy.death.Reset();
				}

			}

			else {	//If player is not in map 1 is in map 2

				

				if (pretime (20)) {	//Do a timer to stop the game during the Death Animation
					data_enemy.current_animation = &data_enemy.death;	//Current Animation is Death
					App->audio->PlayFx(App->scene->death_FX);	//Sets the Death Audio
					//Sets the Position that player goes when he dies
					data_enemy.position.x = 55;	//Set Player X	
					data_enemy.position.y = 10;	//Set Player Y
					current_state = JUMP_FALL2;	//Sets the Animation when he reapears
					data_enemy.death.Reset();

				}

			}

					
		}
		die = false;

	}
	
}

void j1Enemy::OnCollision(Collider* c1, Collider* c2) {	//Check if the Player collides with something

	if(godmode==false){

		if (c1->type == ColliderType::COLLIDER_ENEMY && c2->type == ColliderType::COLLIDER_FLOOR) {	//If player collide with floor

			if (data_enemy.preposition.y < c2->rect.y || data_enemy.position.y == c2->rect.y - data_enemy.colliders->rect.h) {	//Checks that player collider from above

				data_enemy.position.y = c2->rect.y - data_enemy.colliders->rect.h;
				data_enemy.grounded = true;	//Sets that player is touching the floor
				data_enemy.canjump = true;		//Sets tha player can jump

				if (data_enemy.injump == true) {
					//data_enemy.jump.Reset();
				}
				if (data_enemy.grounded == true) {
					data_enemy.jumpCounter = 2;
				}

			}

			else if (data_enemy.preposition.y > (c2->rect.y + c2->rect.h)) {	//Checks that player collider from below
			
				data_enemy.position.y = c2->rect.y + c2->rect.h;
				current_state = JUMP_FALL2;	//Sets the animation 

			}
		
			else if ((data_enemy.position.x < c2->rect.x + c2->rect.w && data_enemy.position.x > c2->rect.x) ||(data_enemy.position.x + data_enemy.colliders->rect.w < c2->rect.x + c2->rect.w && data_enemy.position.x + data_enemy.colliders->rect.w > c2->rect.x)) {	//Checks that player collider from sides
			
				if ((data_enemy.position.x + data_enemy.colliders->rect.w) < (c2->rect.x + c2->rect.w)) { //Checks that player collides from left
				
					data_enemy.position.x = c2->rect.x - data_enemy.colliders->rect.w;

				}
				else if (data_enemy.position.x < (c2->rect.x + c2->rect.w)) {	//Checks that player collides from right
				
					data_enemy.position.x = c2->rect.x + c2->rect.w;

				}
			}

		}

		if (c1->type == ColliderType::COLLIDER_ENEMY && c2->type == ColliderType::COLLIDER_PLATFORM) {		//Checks that player collides with platform

			if (data_enemy.preposition.y < c2->rect.y || data_enemy.position.y == c2->rect.y - data_enemy.colliders->rect.h) {	//Checks that player collider from above

				data_enemy.position.y = c2->rect.y - data_enemy.colliders->rect.h;
				data_enemy.grounded = true;	//Sets that player is touching the floor
				data_enemy.canjump = true;		//Sets tha player can jump

				if (data_enemy.grounded == true) {
				
					data_enemy.jumpCounter = 2;

				}

			}
			else if ((data_enemy.position.y >= data_enemy.preposition.y) && (data_enemy.preposition.y + data_enemy.colliders->rect.h) < c2->rect.y) {	//Checks that player collider from below
			
				data_enemy.position.y = c2->rect.y - data_enemy.colliders->rect.h;

			}

		}

		if (c1->type == ColliderType::COLLIDER_ENEMY && c2->type == ColliderType::COLLIDER_DEAD) {		//Checks that player collides with something that he can die

			//pretime = SDL_GetTicks();	//Sets the pretime to death timer
		
			if (data_enemy.preposition.y < c2->rect.y || data_enemy.position.y == c2->rect.y - data_enemy.colliders->rect.h) {	//Checks that player collider from above
			
				data_enemy.current_animation = &data_enemy.death;	//Current Animation is Death
				App->audio->PlayFx(App->scene->death_FX);	//Sets the Death Audio
				data_enemy.position.y = c2->rect.y - data_enemy.colliders->rect.h;
				current_state = DEATH2;	//Sets player to Death state
			
				data_enemy.grounded = true;	//Sets that player is touching the floor
				data_enemy.canjump = false;	//Sets tha player can jump
				//die = true;	//Sets die bool to true for timer start

			}

			else if (data_enemy.preposition.y > (c2->rect.y + c2->rect.h)) {	//Checks that player collider from below
			
				data_enemy.current_animation = &data_enemy.death;	//Current Animation is Death
				App->audio->PlayFx(App->scene->death_FX);	//Sets the Death Audio
				//pretime = SDL_GetTicks();	//Sets the pretime to death timer

				//data_player.position.y = c2->rect.y + c2->rect.h;
				current_state = DEATH2;	//Sets player to Death state

				data_enemy.grounded = true;	//Sets that player is touching the floor
				data_enemy.canjump = false;	//Sets tha player can jump
				//die = true;	//Sets die bool to true for timer start

			}

		}

		if (c1->type == ColliderType::COLLIDER_ENEMY && c2->type == ColliderType::COLLIDER_NEXT) {

			if (data_enemy.preposition.y < c2->rect.y || data_enemy.position.y == c2->rect.y - data_enemy.colliders->rect.h) {	//Checks that player collider from above	
				App->scene->SecondMap();	//Pass to next map
			}
		
			else if (data_enemy.preposition.y > (c2->rect.y + c2->rect.h)) {	//Checks that player collider from below
				App->scene->SecondMap();	//Pass to next map
			}
		
			else if ((data_enemy.position.x < c2->rect.x + c2->rect.w && data_enemy.position.x > c2->rect.x) || (data_enemy.position.x + data_enemy.colliders->rect.w < c2->rect.x + c2->rect.w && data_enemy.position.x + data_enemy.colliders->rect.w > c2->rect.x)) {	//Checks that player collider from sides
			
				if ((data_enemy.position.x + data_enemy.colliders->rect.w) < (c2->rect.x + c2->rect.w)) {		//Checks that player collides from left
					App->scene->SecondMap();	//Pass to next map
				}
				else if (data_enemy.position.x < (c2->rect.x + c2->rect.w)) {	//Checks that player collides from right
					App->scene->SecondMap();	//Pass to next map
				}
			}
		}

	}
	else {

	}

}

void j1Enemy::Reset() {	//Reset All Player Animations

	data_enemy.death.Reset();
	//data_enemy.jump.Reset();
	data_enemy.walk.Reset();
	data_enemy.idle.Reset();
	//data_enemy.fall.Reset();
	die = false;
}

bool j1Enemy::pretime(float sec)
{
	bool ret = false;
	pretimer++;
	if (pretimer >= sec) { ret = true; pretimer = 0; }
	return ret;
}