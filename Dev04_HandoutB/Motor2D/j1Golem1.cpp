#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Audio.h"
#include "j1Textures.h"
#include "j1Golem1.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Input.h"
#include "j1Collider.h"
#include "j1Scene.h"
#include <math.h>
#include "..//Brofiler/Brofiler.h"


j1Golem1::j1Golem1() : j1Module()
{
	name.create("enemy");
	
}

// Destructor
j1Golem1::~j1Golem1()
{}

bool j1Golem1::Awake(pugi::xml_node& config) {

	bool ret = true;

	//Load All Player Features from Config

	folder.create(config.child("folder").child_value());	
	texture = config.child("texture").attribute("source").as_string();

	data_golem.position.x = config.child("position").attribute("x").as_int();
	data_golem.position.y = config.child("position").attribute("y").as_int();

	data_golem.v.x = config.child("velocity").attribute("x").as_int();
		
	data_golem.gravity = config.child("gravity").attribute("grav").as_float();

	return ret;

}

void j1Golem1::DrawEnemy()
{
	

	if (data_golem.player_flip) {
		App->render->Blit(data_golem.Tex_Player, data_golem.position.x, data_golem.position.y, &(data_golem.current_animation->GetCurrentFrame()), SDL_FLIP_HORIZONTAL, 0);	//Draw Player Flipped
	}
	else {
		App->render->Blit(data_golem.Tex_Player, data_golem.position.x, data_golem.position.y, &(data_golem.current_animation->GetCurrentFrame()), SDL_FLIP_NONE, 0);	//Draw Player Normal
	}


}

bool j1Golem1::Start() {

	pretimer = 0;
	//globaltime = SDL_GetTicks();	//Sets the Global time to the death timer
	
	Pushbacks();	//Call all the Pushback of animations
	
	data_golem.Tex_Player = App->tex->Load(PATH(folder.GetString(), texture.GetString()));	//Load The Texture of player
	LOG("%s", data_golem.Tex_Player);
	SDL_Rect c;
	c.x = data_golem.position.x;
	c.y = data_golem.position.y;
	c.w = 39;
	c.h = 53;

	data_golem.colliders = App->collider->AddCollider(&c, COLLIDER_ENEMY, this);	//Sets The Collider Type and Dimensions to Player
	
	return	true;

}

bool j1Golem1::PreUpdate(float dt) {

	return true;

}

bool j1Golem1::Update(float dt) {

	

	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) {

		if (godmode == false)	//If godmode is false sets None Collider to player for he can fly around map and not collide
		{

			data_golem.gravity = 0 * (dt * LIMIT_TIMER);	//Sets new gravity to player for he can move around map
			godmode = true;	

		}
		else if (godmode == true)
		{

			data_golem.gravity = 20 * (dt * LIMIT_TIMER);	//Sets normal gravity to player 
			godmode = false;
			
		}
	}
	
	data_golem.position.y += data_golem.gravity;
	data_golem.preposition = data_golem.position;
	
	CheckState(dt);	//Checks the state where is the player
	State(dt);	//Set the animation relationed with the state that he is
	
	//Player Collider Draw
	data_golem.colliders->SetPos(data_golem.position.x, data_golem.position.y);	//Sets the Player Collider Position

	//Player Draw
	if (data_golem.player_flip) {
		App->render->Blit(data_golem.Tex_Player, data_golem.position.x, data_golem.position.y, &(data_golem.current_animation->GetCurrentFrame()), SDL_FLIP_HORIZONTAL, 1.0);	//Draw Player Flipped
	}
	else {
		App->render->Blit(data_golem.Tex_Player, data_golem.position.x, data_golem.position.y, &(data_golem.current_animation->GetCurrentFrame()), SDL_FLIP_NONE, 1.0);	//Draw Player Normal
	}

	if (data_golem.grounded == false) {	//Sets that if the player is not touching the ground puts the fall animation

		current_state = JUMP_FALL2;

	}
		
	return true;

	data_golem.current_animation = &data_golem.idle;

}

bool j1Golem1::PostUpdate(float dt) {

	return true;
}
// Called before quitting
bool j1Golem1::CleanUp()
{
	LOG("Unloading player");

	App->tex->UnLoad(data_golem.Tex_Player);	//Unload The Player texture
	App->collider->CleanUp();	//Unload the Player collider
	
	return true;
}

bool j1Golem1::Load(pugi::xml_node& node) {

	if (!node.child("position").empty())	//Load the valors if save_confign file is empty link
	{

		data_golem.position.x = node.child("position").attribute("x").as_int();	//Load Player X
		data_golem.position.y = node.child("position").attribute("y").as_int();	//Load Player Y
	}

	else {	
		data_golem.position.x = data_golem.position.x;	//Load Player X
		data_golem.position.y = data_golem.position.y;	//Load Player Y
	}
	return true;

}
bool j1Golem1::Save(pugi::xml_node& node) const {

	if (node.child("position").empty())	//Save The Position Valors in the save_config file
	{
		pugi::xml_node&  save = node.append_child("position");	
		save.append_attribute("x").set_value(data_golem.position.x);	//Save Player X
		save.append_attribute("y").set_value(data_golem.position.y);	//Save Player Y
	}
	else
	{
		node.child("position").attribute("x").set_value(data_golem.position.x);	//Save Player X
		node.child("position").attribute("y").set_value(data_golem.position.y);	//Save Player Y
	}
	
	return true;

}

void j1Golem1::Pushbacks() {

	//Sets the Idle Animation
	data_golem.idle.PushBack({ 0,0,43,63 }, 0.4, 0, 0);
	data_golem.idle.PushBack({ 46,0,41,64 }, 0.4, -2, 0);
	data_golem.idle.PushBack({ 92,0,41,63 }, 0.4, -2, 0);
	data_golem.idle.PushBack({ 136,0,45,64 }, 0.4, -4, 0);
	data_golem.idle.PushBack({ 182,0,42,63 }, 0.4, -4, 0);
	data_golem.idle.PushBack({ 228,0,42,63 }, 0.4, -2, 0);
	data_golem.idle.PushBack({ 274,0,42,62 }, 0.4, -4, 0);
	data_golem.idle.PushBack({ 318,0,42,64 }, 0.4, -4, 0);
	data_golem.idle.PushBack({ 365,0,42,64 }, 0.4, -4, 0);
	data_golem.idle.PushBack({ 410,0,44,63 }, 0.4, -2, 0);
	data_golem.idle.PushBack({ 456,0,43,63 }, 0.4, -2, 0);
	data_golem.idle.PushBack({ 502,0,41,63 }, 0.4, -2, 0);
	data_golem.idle.PushBack({ 546,0,43,63 }, 0.4, -4, 0);
	data_golem.idle.PushBack({ 593,0,41,63 }, 0.4, -4, 0);
	data_golem.idle.PushBack({ 640,0,44,63 }, 0.4, -4, 0);
	data_golem.idle.PushBack({ 687,0,42,63 }, 0.4, -4, 0);
	data_golem.idle.PushBack({ 731,0,44,63 }, 0.4, -4, 0);
	data_golem.idle.PushBack({ 777,0,43,63 }, 0.4, -4, 0);
	data_golem.idle.loop = true;

	//Sets the Walk Animation
	data_golem.walk.PushBack({ 0,76,43,63 }, 0.4, 0, 0);
	data_golem.walk.PushBack({ 42,76,43,63 }, 0.4, 0, 0);
	data_golem.walk.PushBack({ 88,76,43,64 }, 0.4, 0, 0);
	data_golem.walk.PushBack({ 132,76,43,64 }, 0.4, 0, 0);
	data_golem.walk.PushBack({ 178,76,42,63 }, 0.4, 0, 0);
	data_golem.walk.PushBack({ 221,76,45,64 }, 0.4, 0, 0);
	data_golem.walk.PushBack({ 267,76,44,62 }, 0.4, 0, 0);
	data_golem.walk.PushBack({ 312,76,44,63 }, 0.4, 0, 0);
	data_golem.walk.PushBack({ 357,76,44,63 }, 0.4, 0, 0);
	data_golem.walk.PushBack({ 403,76,43,63 }, 0.4, 0, 0);
	data_golem.walk.PushBack({ 447,76,43,64 }, 0.4, 0, 0);
	data_golem.walk.PushBack({ 492,76,43,64 }, 0.4, 0, 0);
	data_golem.walk.PushBack({ 537,76,43,64 }, 0.4, 0, 0);
	data_golem.walk.PushBack({ 582,76,41,64 }, 0.4, 0, 0);
	data_golem.walk.PushBack({ 626,76,44,64 }, 0.4, 0, 0);
	data_golem.walk.PushBack({ 672,76,43,64 }, 0.4, 0, 0);
	data_golem.walk.PushBack({ 719,76,42,64 }, 0.4, 0, 0);
	data_golem.walk.PushBack({ 764,76,42,64 }, 0.4, 0, 0);
	data_golem.walk.PushBack({ 809,76,43,63 }, 0.4, 0, 0);
	data_golem.walk.PushBack({ 853,76,44,62 }, 0.4, 0, 0);
	data_golem.walk.PushBack({ 899,76,43,62 }, 0.4, 0, 0);
	data_golem.walk.PushBack({ 943,76,44,63 }, 0.4, 0, 0);
	data_golem.walk.PushBack({ 988,76,44,64 }, 0.4, 0, 0);
	data_golem.walk.PushBack({ 1033,76,45,64 }, 0.4, 0, 0);
	data_golem.walk.loop = true;

	//Sets the Attack Animation
	data_golem.attack.PushBack({ 0,152,44,63 }, 0.3, 0, 0);
	data_golem.attack.PushBack({ 46,152,45,63 }, 0.3, 0, 0);
	data_golem.attack.PushBack({ 95,152,69,65 }, 0.3, 0, 0);
	data_golem.attack.PushBack({ 168,152,67,66 }, 0.3, 0, 0);
	data_golem.attack.PushBack({ 238,152,69,67 }, 0.3, 0, 0);
	data_golem.attack.PushBack({ 311,152,63,65 }, 0.3, 0, 0);
	data_golem.attack.PushBack({ 382,152,66,64 }, 0.3, 0, 0);
	data_golem.attack.PushBack({ 453,152,67,64 }, 0.3, 0, 0);
	data_golem.attack.PushBack({ 522,152,64,66 }, 0.3, 0, 0);
	data_golem.attack.PushBack({ 593,152,64,65 }, 0.3, 0, 0);
	data_golem.attack.PushBack({ 662,152,61,65 }, 0.3, 0, 0);
	data_golem.attack.PushBack({ 729,152,64,64 }, 0.3, 0, 0);
	data_golem.attack.loop = false;

	//Sets the Hurt Animation
	data_golem.hurt.PushBack({ 0,228,42,64 }, 0.3, 0, 0);
	data_golem.hurt.PushBack({ 47,228,44,65 }, 0.3, 0, 0);
	data_golem.hurt.PushBack({ 97,228,44,66 }, 0.3, 0, 0);
	data_golem.hurt.PushBack({ 147,228,45,64 }, 0.3, 0, 0);
	data_golem.hurt.PushBack({ 196,228,52,63 }, 0.3, 0, 0);
	data_golem.hurt.PushBack({ 253,228,55,61 }, 0.3, 0, 0);
	data_golem.hurt.PushBack({ 315,228,52,61 }, 0.3, 0, 0);
	data_golem.hurt.PushBack({ 373,228,50,61 }, 0.3, 0, 0);
	data_golem.hurt.PushBack({ 430,228,47,62 }, 0.3, 0, 0);
	data_golem.hurt.PushBack({ 484,228,45,62 }, 0.3, 0, 0);
	data_golem.hurt.PushBack({ 535,228,44,61 }, 0.3, 0, 0);
	data_golem.hurt.PushBack({ 585,228,43,62 }, 0.3, 0, 0);
	data_golem.hurt.loop = false;

	//Sets the Death Animation
	data_golem.death.PushBack({ 0,305,42,64 }, 0.3, 0, 0);
	data_golem.death.PushBack({ 47,305,44,65 }, 0.3, 0, 1);
	data_golem.death.PushBack({ 102,305,43,65 }, 0.3, 0, 2);
	data_golem.death.PushBack({ 155,305,43,65 }, 0.3, 0, 4);
	data_golem.death.PushBack({ 207,305,44,64 }, 0.3, 0, 8);
	data_golem.death.PushBack({ 256,305,50,60 }, 0.3, 0, 10);
	data_golem.death.PushBack({ 313,305,59,56 }, 0.3, 0, 15);
	data_golem.death.PushBack({ 381,305,50,61 }, 0.3, 0, 20);
	data_golem.death.PushBack({ 451,305,64,46 }, 0.3, 0, 20);
	data_golem.death.PushBack({ 525,305,66,42 }, 0.3, 0, 20);
	data_golem.death.PushBack({ 601,305,66,42 }, 0.3, 0, 20);
	data_golem.death.PushBack({ 673,305,65,41 }, 0.3, 0, 20);
	data_golem.death.PushBack({ 749,305,64,41 }, 0.3, 0, 20);

	data_golem.death.loop = false;


}

void j1Golem1::CheckState(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT|| App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		data_golem.current_animation = &data_golem.walk;
	}
	if (App->input->GetKey(SDL_SCANCODE_X) == KEY_REPEAT) {
		data_golem.current_animation = &data_golem.attack;
		data_golem.attack.Reset();
	}
	if (App->input->GetKey(SDL_SCANCODE_V) == KEY_REPEAT) {
		data_golem.current_animation = &data_golem.death;
		data_golem.death.Reset();

	}
	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT) {
		data_golem.current_animation = &data_golem.hurt;
		data_golem.hurt.Reset();

	}

}

void j1Golem1::State(float dt) {

	if (current_state == IDLE2) {
		
		data_golem.current_animation = &data_golem.idle;
		//data_enemy.jump.Reset();
		//data_enemy.fall.Reset();
		
	}

	if (current_state == WALK2) {

		data_golem.current_animation = &data_golem.walk;
		///data_enemy.jump.Reset();
		//data_enemy.fall.Reset();
		
	}
	
	if (current_state == JUMP_UP2) {

		data_golem.canjump = false;
		data_golem.injump = true;
		//data_enemy.current_animation = &data_enemy.jump;

		if (data_golem.jumpenergy <= data_golem.gravity) {									//character will jump up until it do not accomplish this condition 
			
			data_golem.jumpenergy += 0.5;														// jump up increments 0.5 each time
			data_golem.position.y = data_golem.position.y + (data_golem.jumpenergy *(dt * LIMIT_TIMER));			// y position increments 0.5 each time

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
					data_golem.position.x = 100;	//Set Player X	
					data_golem.position.y = 300;	//Set Player Y
					current_state = JUMP_FALL2;	//Sets the Animation when he reapears
					data_golem.death.Reset();
				}

			}

			else {	//If player is not in map 1 is in map 2

				

				if (pretime (20)) {	//Do a timer to stop the game during the Death Animation
					data_golem.current_animation = &data_golem.death;	//Current Animation is Death
					App->audio->PlayFx(App->scene->death_FX);	//Sets the Death Audio
					//Sets the Position that player goes when he dies
					data_golem.position.x = 55;	//Set Player X	
					data_golem.position.y = 10;	//Set Player Y
					current_state = JUMP_FALL2;	//Sets the Animation when he reapears
					data_golem.death.Reset();

				}

			}

					
		}
		die = false;

	}
	
}

void j1Golem1::OnCollision(Collider* c1, Collider* c2) {	//Check if the Player collides with something

	if(godmode==false){

		if (c1->type == ColliderType::COLLIDER_ENEMY && c2->type == ColliderType::COLLIDER_FLOOR) {	//If player collide with floor

			if (data_golem.preposition.y < c2->rect.y || data_golem.position.y == c2->rect.y - data_golem.colliders->rect.h) {	//Checks that player collider from above

				data_golem.position.y = c2->rect.y - data_golem.colliders->rect.h;
				data_golem.grounded = true;	//Sets that player is touching the floor
				data_golem.canjump = true;		//Sets tha player can jump

				if (data_golem.injump == true) {
					//data_enemy.jump.Reset();
				}
				if (data_golem.grounded == true) {
					data_golem.jumpCounter = 2;
				}

			}

			else if (data_golem.preposition.y > (c2->rect.y + c2->rect.h)) {	//Checks that player collider from below
			
				data_golem.position.y = c2->rect.y + c2->rect.h;
				current_state = JUMP_FALL2;	//Sets the animation 

			}
		
			else if ((data_golem.position.x < c2->rect.x + c2->rect.w && data_golem.position.x > c2->rect.x) ||(data_golem.position.x + data_golem.colliders->rect.w < c2->rect.x + c2->rect.w && data_golem.position.x + data_golem.colliders->rect.w > c2->rect.x)) {	//Checks that player collider from sides
			
				if ((data_golem.position.x + data_golem.colliders->rect.w) < (c2->rect.x + c2->rect.w)) { //Checks that player collides from left
				
					data_golem.position.x = c2->rect.x - data_golem.colliders->rect.w;

				}
				else if (data_golem.position.x < (c2->rect.x + c2->rect.w)) {	//Checks that player collides from right
				
					data_golem.position.x = c2->rect.x + c2->rect.w;

				}
			}

		}

		if (c1->type == ColliderType::COLLIDER_ENEMY && c2->type == ColliderType::COLLIDER_PLATFORM) {		//Checks that player collides with platform

			if (data_golem.preposition.y < c2->rect.y || data_golem.position.y == c2->rect.y - data_golem.colliders->rect.h) {	//Checks that player collider from above

				data_golem.position.y = c2->rect.y - data_golem.colliders->rect.h;
				data_golem.grounded = true;	//Sets that player is touching the floor
				data_golem.canjump = true;		//Sets tha player can jump

				if (data_golem.grounded == true) {
				
					data_golem.jumpCounter = 2;

				}

			}
			else if ((data_golem.position.y >= data_golem.preposition.y) && (data_golem.preposition.y + data_golem.colliders->rect.h) < c2->rect.y) {	//Checks that player collider from below
			
				data_golem.position.y = c2->rect.y - data_golem.colliders->rect.h;

			}

		}

		if (c1->type == ColliderType::COLLIDER_ENEMY && c2->type == ColliderType::COLLIDER_DEAD) {		//Checks that player collides with something that he can die

			//pretime = SDL_GetTicks();	//Sets the pretime to death timer
		
			if (data_golem.preposition.y < c2->rect.y || data_golem.position.y == c2->rect.y - data_golem.colliders->rect.h) {	//Checks that player collider from above
			
				data_golem.current_animation = &data_golem.death;	//Current Animation is Death
				App->audio->PlayFx(App->scene->death_FX);	//Sets the Death Audio
				data_golem.position.y = c2->rect.y - data_golem.colliders->rect.h;
				current_state = DEATH2;	//Sets player to Death state
			
				data_golem.grounded = true;	//Sets that player is touching the floor
				data_golem.canjump = false;	//Sets tha player can jump
				//die = true;	//Sets die bool to true for timer start

			}

			else if (data_golem.preposition.y > (c2->rect.y + c2->rect.h)) {	//Checks that player collider from below
			
				data_golem.current_animation = &data_golem.death;	//Current Animation is Death
				App->audio->PlayFx(App->scene->death_FX);	//Sets the Death Audio
				//pretime = SDL_GetTicks();	//Sets the pretime to death timer

				//data_player.position.y = c2->rect.y + c2->rect.h;
				current_state = DEATH2;	//Sets player to Death state

				data_golem.grounded = true;	//Sets that player is touching the floor
				data_golem.canjump = false;	//Sets tha player can jump
				//die = true;	//Sets die bool to true for timer start

			}

		}

		if (c1->type == ColliderType::COLLIDER_ENEMY && c2->type == ColliderType::COLLIDER_NEXT) {

			if (data_golem.preposition.y < c2->rect.y || data_golem.position.y == c2->rect.y - data_golem.colliders->rect.h) {	//Checks that player collider from above	
				App->scene->SecondMap();	//Pass to next map
			}
		
			else if (data_golem.preposition.y > (c2->rect.y + c2->rect.h)) {	//Checks that player collider from below
				App->scene->SecondMap();	//Pass to next map
			}
		
			else if ((data_golem.position.x < c2->rect.x + c2->rect.w && data_golem.position.x > c2->rect.x) || (data_golem.position.x + data_golem.colliders->rect.w < c2->rect.x + c2->rect.w && data_golem.position.x + data_golem.colliders->rect.w > c2->rect.x)) {	//Checks that player collider from sides
			
				if ((data_golem.position.x + data_golem.colliders->rect.w) < (c2->rect.x + c2->rect.w)) {		//Checks that player collides from left
					App->scene->SecondMap();	//Pass to next map
				}
				else if (data_golem.position.x < (c2->rect.x + c2->rect.w)) {	//Checks that player collides from right
					App->scene->SecondMap();	//Pass to next map
				}
			}
		}

	}
	else {

	}

}

void j1Golem1::Reset() {	//Reset All Player Animations

	data_golem.death.Reset();
	//data_enemy.jump.Reset();
	data_golem.walk.Reset();
	data_golem.idle.Reset();
	//data_enemy.fall.Reset();
	die = false;
}

bool j1Golem1::pretime(float sec)
{
	bool ret = false;
	pretimer++;
	if (pretimer >= sec) { ret = true; pretimer = 0; }
	return ret;
}