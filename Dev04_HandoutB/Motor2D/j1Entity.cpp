/*#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Audio.h"
#include "j1Textures.h"
#include "j1Entity.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Input.h"
#include "j1Collider.h"
#include "j1Scene.h"
#include <math.h>

Entity:: Entity()
{
}

Entity::Entity(entity_type type) :type(type)
{
}

// Destructor
Entity::~Entity()
{}*/

/*

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
	data_enemy.idle.PushBack({ 0,0,49,75 }, 0.1, 0, 0);
	data_enemy.idle.PushBack({ 58,0,47,76 }, 0.1, 10, 0);
	data_enemy.idle.PushBack({ 117,0,47,76 }, 0.2, 10, 0);
	data_enemy.idle.PushBack({ 176,0,47,76 }, 0.2, 10, 0);
	data_enemy.idle.PushBack({ 233,0,47,76 }, 0.2, 10, 0);
	data_enemy.idle.PushBack({ 291,0,47,76 }, 0.2, 10, 0);
	data_enemy.idle.PushBack({ 348,0,47,76 }, 0.2, 10, 0);
	data_enemy.idle.PushBack({ 405,0,47,76 }, 0.2, 10, 0);
	data_enemy.idle.PushBack({ 463,0,47,76 }, 0.2, 10, 0);
	data_enemy.idle.PushBack({ 521,0,47,76 }, 0.1, 0, 0);
	data_enemy.idle.PushBack({ 579,0,47,76 }, 0.1, -1, 0);
	data_enemy.idle.PushBack({ 636,0,47,76 }, 0.1, -6, 0);
	data_enemy.idle.PushBack({ 693,0,47,76 }, 0.2, -20, 0);
	data_enemy.idle.PushBack({ 751,0,47,76 }, 0.2, -20, 0);
	data_enemy.idle.PushBack({ 812,0,47,76 }, 0.2, -20, 0);
	data_enemy.idle.PushBack({ 870,0,47,76 }, 0.2, -20, 0);
	data_enemy.idle.PushBack({ 927,0,47,76 }, 0.2, -20, 0);
	data_enemy.idle.PushBack({ 985,0,47,76 }, 0.2, -20, 0);
	data_enemy.idle.loop = true;

	/Sets the Walk Animation
	data_player.walk.PushBack({ 7,4,42,49 }, 0.2, 0, 0);
	data_player.walk.PushBack({ 65,4,41,49 }, 0.2, 0, 0);
	data_player.walk.PushBack({ 131,3,38,50 }, 0.2, 0, 0);
	data_player.walk.PushBack({ 201,3,36,51 }, 0.2, 0, 0);
	data_player.walk.PushBack({ 259,5,38,48 }, 0.2, 0, 0);
	data_player.walk.PushBack({ 317,4,44,50 }, 0.2, 0, 0);
	data_player.walk.loop = true;*/

	/*//Sets the Run Animation
	data_player.walk2.PushBack({ 7,4,42,49 }, 0.4, 0, 0);
	data_player.walk2.PushBack({ 65,4,41,49 }, 0.4, 0, 0);
	data_player.walk2.PushBack({ 131,3,38,50 }, 0.4, 0, 0);
	data_player.walk2.PushBack({ 201,3,36,51 }, 0.4, 0, 0);
	data_player.walk2.PushBack({ 259,5,38,48 }, 0.4, 0, 0);
	data_player.walk2.PushBack({ 317,4,44,50 }, 0.4, 0, 0);
	data_player.walk2.loop = true;*/

	/*//Sets the Jump Animation
	data_player.jump.PushBack({ 7,128,52,43 }, 0.6, 0, 0);
	data_player.jump.PushBack({ 81,114,47,57 }, 0.3, 0, 0);
	data_player.jump.PushBack({ 155,113,42,58 }, 0.3, 0, 0);
	data_player.jump.PushBack({ 221,118,52,53 }, 0.2, 0, 0);
	data_player.jump.PushBack({ 285,116,53,55 }, 0.15, 0, 0);
	data_player.jump.PushBack({ 348,119,56,52 }, 0.05, 0, 0);
	data_player.jump.loop = false;

	//Sets the Fall Animation
	data_enemy.fall.PushBack({ 0,0,49,75 }, 0.05, 0, 0);
	data_enemy.fall.PushBack({ 0,0,49,75 }, 0.5, 0, 0);
	data_enemy.fall.loop = true;

	//Sets the Attack Animation
	data_player.idle_attack.PushBack({ 7,759,58,53 }, 0.1, 0, 0);
	data_player.idle_attack.PushBack({ 70,758,60,54 }, 0.1, 0, 0);
	data_player.idle_attack.PushBack({ 130,763,61,50 }, 0.1, 0, 0);
	data_player.idle_attack.PushBack({ 201,763,59,51 }, 0.1, 0, 0);
	data_player.idle_attack.PushBack({ 270,758,56,54 }, 0.1, 0, 0);
	data_player.idle_attack.PushBack({ 333,757,39,54 }, 0.1, 0, 0);
	data_player.idle_attack.PushBack({ 397,759,38,53 }, 0.1, 0, 0);
	data_player.idle_attack.loop = false;

	//Sets the Death Animation
	data_player.death.PushBack({ 8,464,48,55 }, 0.2, 0, 0);
	data_player.death.PushBack({ 72,456,62,63 }, 0.2, 0, 0);
	data_player.death.PushBack({ 150,454,69,65 }, 0.2, 0, 0);
	data_player.death.PushBack({ 257,464,82,56 }, 0.2, 0, 0);
	data_player.death.PushBack({ 367,447,83,72 }, 0.2, 0, 0);
	data_player.death.PushBack({ 488,447,84,73 }, 0.2, 0, 0);
	data_player.death.PushBack({ 609,457,94,62 }, 0.2, 0, 0);
	data_player.death.PushBack({ 734,455,113,65 }, 0.2, 0, 0);
	data_player.death.PushBack({ 894,445,127,74 }, 0.2, 0, 0);
	data_player.death.PushBack({ 1054,433,116,87 }, 0.2, 0, 0);
	data_player.death.loop = false;

}

void j1Enemy::CheckState(float dt)
{
	
	

}

void j1Enemy::State(float dt) {

	if (current_state == IDLE2) {
		
		data_enemy.current_animation = &data_enemy.idle;
		data_enemy.jump.Reset();
		data_enemy.fall.Reset();
		
	}

	if (current_state == WALK2) {

		data_enemy.current_animation = &data_enemy.walk;
		data_enemy.jump.Reset();
		data_enemy.fall.Reset();
		
	}
	
	if (current_state == JUMP_UP2) {

		data_enemy.canjump = false;
		data_enemy.injump = true;
		data_enemy.current_animation = &data_enemy.jump;

		if (data_enemy.jumpenergy <= data_enemy.gravity) {									//character will jump up until it do not accomplish this condition 
			
			data_enemy.jumpenergy += 0.5;														// jump up increments 0.5 each time
			data_enemy.position.y = data_enemy.position.y + (data_enemy.jumpenergy *(dt * LIMIT_TIMER));			// y position increments 0.5 each time

		}

				
	}
	
	if (current_state == JUMP_FALL2) {

		data_enemy.current_animation = &data_enemy.fall;
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			data_enemy.position.x -= data_enemy.v.x*(dt * LIMIT_TIMER);
			data_enemy.player_flip = true;
		}
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			data_enemy.position.x += data_enemy.v.x*(dt * LIMIT_TIMER);
			data_enemy.player_flip = false;
			
		}
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
					data_enemy.jump.Reset();
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
	data_enemy.jump.Reset();
	data_enemy.walk.Reset();
	data_enemy.idle.Reset();
	data_enemy.fall.Reset();
	die = false;
}

bool j1Enemy::pretime(float sec)
{
	bool ret = false;
	pretimer++;
	if (pretimer >= sec) { ret = true; pretimer = 0; }
	return ret;
}*/