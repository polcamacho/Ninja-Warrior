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
#include "j1EntityManager.h"
#include "j1Player.h"
#include "j1Pathfinding.h"
#include <math.h>
#include "..//Brofiler/Brofiler.h"


j1Golem1::j1Golem1(int x, int y) : j1Entity(entity_type::GOLEM_GRASS_ENEMY)
{
	name.create("golem_grass");
	data_golem.ipos.x = x;
	data_golem.ipos.y = y;
	pathT = App->tex->Load("maps/cross.png");

}

// Destructor
j1Golem1::~j1Golem1()
{}

bool j1Golem1::Awake(pugi::xml_node& config) {

	bool ret = true;

	//Load All Player Features from Config

	v.x = config.child("golem_grass").child("velocity").attribute("x").as_int();
		
	gravity = config.child("golem_grass").child("gravity").attribute("grav").as_float();

	return ret;

}

bool j1Golem1::Start() {

	pretimer = 0;
	globaltime = SDL_GetTicks();	//Sets the Global time to the death timer
	position.x = data_golem.ipos.x;
	position.y = data_golem.ipos.y;
	v.x = 5;
	gravity = 15;
	Pushbacks();	//Call all the Pushback of animations
	
	current_animation = &idle;

	SDL_Rect c;
	c.x = position.x;
	c.y = position.y;
	c.w = 39;
	c.h = 53;

	entity_colliders = App->collider->AddCollider(&c, COLLIDER_ENEMY, this);	//Sets The Collider Type and Dimensions to Player
	
	return	true;

}

bool j1Golem1::PreUpdate(float dt) {

	BROFILER_CATEGORY("PreUpdateGolem", Profiler::Color::Aquamarine);

	return true;

}

bool j1Golem1::Update(float dt) {

	BROFILER_CATEGORY("UPDATEGOLEM", Profiler::Color::DarkGray);

	current_animation = &idle;

	position.y += gravity;
	preposition = position;
	
	CheckState(dt);	//Checks the state where is the player
	State(dt);	//Set the animation relationed with the state that he is

	if (App->entity->GetPlayer()->position.x > position.x - 400 && App->entity->GetPlayer()->position.x < position.x + 400 && App->entity->GetPlayer()->position.y + 100 && App->entity->GetPlayer()->position.y - 100) {
		
		const p2DynArray<iPoint>* path = App->pathfinding->GetLastPath();

		if (App->collider->debug == true)
		{
			for (uint i = 0; i < path->Count(); ++i)
			{
				iPoint next_position = App->map->MapToWorld(path->At(i)->x, path->At(i)->y);
				App->render->Blit(App->scene->debug_tex, next_position.x, next_position.y);

			}

		}

		if (data_golem.pathfinding == true) {
			
			v = { 0, 0 };

			iPoint origin;
			iPoint player_pos = App->entity->GetPlayer()->position;

			player_pos = App->map->WorldToMap(player_pos.x + 10, player_pos.y + 10);
			origin = App->map->WorldToMap(position.x + 10, position.y + 10);

			App->pathfinding->CreatePath(origin, player_pos);

			if (path->Count() > 0)
			{

				iPoint path_position = iPoint(path->At(0)->x, path->At(0)->y);

				if (path_position.x < origin.x) {
					current_stateE2 == WALK2;
					position.x -= 4 * LIMIT_TIMER * dt;
					flip = true;
				}

				if (path_position.x > origin.x) {
					current_stateE2 == WALK2;
					position.x += 4 * LIMIT_TIMER * dt;
					flip = false;
				}

			}
		}
	}

	DrawCollider();

	//Player Draw
	if (flip) {
		App->render->Blit(App->entity->Tex_Golems_Grass, position.x, position.y, &(current_animation->GetCurrentFrame()), SDL_FLIP_HORIZONTAL, 1.0);	//Draw Player Flipped
	}
	else {
		App->render->Blit(App->entity->Tex_Golems_Grass, position.x, position.y, &(current_animation->GetCurrentFrame()), SDL_FLIP_NONE, 1.0);	//Draw Player Normal
	}

	if (grounded == false) {	//Sets that if the player is not touching the ground puts the fall animation

		//current_state = JUMP_FALL2;

	}

	return true;

}

bool j1Golem1::PostUpdate(float dt) {

	return true;
}

// Called before quitting
/*bool j1Golem1::CleanUp()
{
	LOG("Unloading player");
	j1Entity::CleanUp();
	App->tex->UnLoad(App->entity->Tex_Golems);	//Unload The Player texture
	App->collider->CleanUp();	//Unload the Player collider
	
	return true;
}*/

bool j1Golem1::Load(pugi::xml_node& node) {

	if (!node.child("golem1position").empty())	//Load the valors if save_confign file is empty link
	{

		position.x = node.child("golem1position").attribute("x").as_int();	//Load Player X
		position.y = node.child("golem1position").attribute("y").as_int();	//Load Player Y
	}

	else {
		position.x = position.x;	//Load Player X
		position.y = position.y;	//Load Player Y
	}
	return true;

}
bool j1Golem1::Save(pugi::xml_node& node) const {

	if (node.child("golem1position").empty())	//Save The Position Valors in the save_config file
	{
		pugi::xml_node&  save = node.append_child("golem1position");
		save.append_attribute("x").set_value(position.x);	//Save Player X
		save.append_attribute("y").set_value(position.y);	//Save Player Y
	}
	else
	{
		node.child("golem1position").attribute("x").set_value(position.x);	//Save Player X
		node.child("golem1position").attribute("y").set_value(position.y);	//Save Player Y
	}

	return true;

}

void j1Golem1::Pushbacks() {

	//Sets the Idle Animation
	idle.PushBack({ 0,0,43,63 }, 0.4, 0, 0);
	idle.PushBack({ 46,0,41,64 }, 0.4, -2, 0);
	idle.PushBack({ 92,0,41,63 }, 0.4, -2, 0);
	idle.PushBack({ 136,0,45,64 }, 0.4, -4, 0);
	idle.PushBack({ 182,0,42,63 }, 0.4, -4, 0);
	idle.PushBack({ 228,0,42,63 }, 0.4, -2, 0);
	idle.PushBack({ 274,0,42,62 }, 0.4, -4, 0);
	idle.PushBack({ 318,0,42,64 }, 0.4, -4, 0);
	idle.PushBack({ 365,0,42,64 }, 0.4, -4, 0);
	idle.PushBack({ 410,0,44,63 }, 0.4, -2, 0);
	idle.PushBack({ 456,0,43,63 }, 0.4, -2, 0);
	idle.PushBack({ 502,0,41,63 }, 0.4, -2, 0);
	idle.PushBack({ 546,0,43,63 }, 0.4, -4, 0);
	idle.PushBack({ 593,0,41,63 }, 0.4, -4, 0);
	idle.PushBack({ 640,0,44,63 }, 0.4, -4, 0);
	idle.PushBack({ 687,0,42,63 }, 0.4, -4, 0);
	idle.PushBack({ 731,0,44,63 }, 0.4, -4, 0);
	idle.PushBack({ 777,0,43,63 }, 0.4, -4, 0);
	idle.loop = true;

	//Sets the Walk Animation
	walk.PushBack({ 0,76,43,63 }, 0.4, 0, 0);
	walk.PushBack({ 42,76,43,63 }, 0.4, 0, 0);
	walk.PushBack({ 88,76,43,64 }, 0.4, 0, 0);
	walk.PushBack({ 132,76,43,64 }, 0.4, 0, 0);
	walk.PushBack({ 178,76,42,63 }, 0.4, 0, 0);
	walk.PushBack({ 221,76,45,64 }, 0.4, 0, 0);
	walk.PushBack({ 267,76,44,62 }, 0.4, 0, 0);
	walk.PushBack({ 312,76,44,63 }, 0.4, 0, 0);
	walk.PushBack({ 357,76,44,63 }, 0.4, 0, 0);
	walk.PushBack({ 403,76,43,63 }, 0.4, 0, 0);
	walk.PushBack({ 447,76,43,64 }, 0.4, 0, 0);
	walk.PushBack({ 492,76,43,64 }, 0.4, 0, 0);
	walk.PushBack({ 537,76,43,64 }, 0.4, 0, 0);
	walk.PushBack({ 582,76,41,64 }, 0.4, 0, 0);
	walk.PushBack({ 626,76,44,64 }, 0.4, 0, 0);
	walk.PushBack({ 672,76,43,64 }, 0.4, 0, 0);
	walk.PushBack({ 719,76,42,64 }, 0.4, 0, 0);
	walk.PushBack({ 764,76,42,64 }, 0.4, 0, 0);
	walk.PushBack({ 809,76,43,63 }, 0.4, 0, 0);
	walk.PushBack({ 853,76,44,62 }, 0.4, 0, 0);
	walk.PushBack({ 899,76,43,62 }, 0.4, 0, 0);
	walk.PushBack({ 943,76,44,63 }, 0.4, 0, 0);
	walk.PushBack({ 988,76,44,64 }, 0.4, 0, 0);
	walk.PushBack({ 1033,76,45,64 }, 0.4, 0, 0);
	walk.loop = true;

	//Sets the Attack Animation
	/*data_golem.attack.PushBack({ 0,152,44,63 }, 0.3, 0, 0);
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
	data_golem.attack.loop = false;*/

	//Sets the Hurt Animation
	/*data_golem.hurt.PushBack({ 0,228,42,64 }, 0.3, 0, 0);
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
	data_golem.hurt.loop = false;*/

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
		current_animation = &walk;
	}
	if (App->input->GetKey(SDL_SCANCODE_X) == KEY_REPEAT) {
		//current_animation = &data_golem.attack;
		//data_golem.attack.Reset();
	}
	if (App->input->GetKey(SDL_SCANCODE_V) == KEY_REPEAT) {
		current_animation = &data_golem.death;
		data_golem.death.Reset();

	}
	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT) {
		//current_animation = &data_golem.hurt;
		//data_golem.hurt.Reset();

	}

}

void j1Golem1::State(float dt) {

	if (current_stateE2 == IDLE2) {
		current_animation = &idle;
	}

	if (current_stateE2 == WALK2) {
		current_animation = &walk;
	}
	
	if (current_stateE2 == DEATH2) {	
		
		die = true;	//Sets the die to true

		if (App->scene->current_map == "Map.tmx") {	//If player is in map 1
				
			if (PreTime (20)) {	//Do a timer to stop the game during the Death animation
					
					//Sets the Position that player goes when he dies
					data_golem.death.Reset();
			}

		}

		else {	//If player is not in map 1 is in map 2
				
			if (PreTime (20)) {	//Do a timer to stop the game during the Death Animation
					
					current_animation = &data_golem.death;	//Current Animation is Death
					//Sets the Position that player goes when he dies
					data_golem.death.Reset();

			}

		}

		die = false;

	}
	
}

void j1Golem1::OnCollision(Collider* c1, Collider* c2) {	//Check if the Player collides with something

		if (c1->type == ColliderType::COLLIDER_ENEMY && c2->type == ColliderType::COLLIDER_FLOOR) {	//If player collide with floor

			if (preposition.y < c2->rect.y || position.y == c2->rect.y - entity_colliders->rect.h) {	//Checks that player collider from above

				position.y = c2->rect.y - entity_colliders->rect.h;
				grounded = true;	//Sets that player is touching the floor
			
			}

			else if (preposition.y > (c2->rect.y + c2->rect.h)) {	//Checks that player collider from below
			
				position.y = c2->rect.y + c2->rect.h;
				
			}
		
			else if ((position.x < c2->rect.x + c2->rect.w && position.x > c2->rect.x) ||(position.x + entity_colliders->rect.w < c2->rect.x + c2->rect.w && position.x + entity_colliders->rect.w > c2->rect.x)) {	//Checks that player collider from sides
			
				if ((position.x + entity_colliders->rect.w) < (c2->rect.x + c2->rect.w)) { //Checks that player collides from left
				
					position.x = c2->rect.x - entity_colliders->rect.w;

				}
				else if (position.x < (c2->rect.x + c2->rect.w)) {	//Checks that player collides from right
				
					position.x = c2->rect.x + c2->rect.w;

				}
			}

		}

		if (c1->type == ColliderType::COLLIDER_ENEMY && c2->type == ColliderType::COLLIDER_PLATFORM) {		//Checks that player collides with platform

			if (preposition.y < c2->rect.y || position.y == c2->rect.y - entity_colliders->rect.h) {	//Checks that player collider from above

				position.y = c2->rect.y - entity_colliders->rect.h;
				grounded = true;	//Sets that player is touching the floor


			}
			else if ((position.y >= preposition.y) && (preposition.y + entity_colliders->rect.h) < c2->rect.y) {	//Checks that player collider from below
			
				position.y = c2->rect.y - entity_colliders->rect.h;

			}

		}
		
		if (c1->type == ColliderType::COLLIDER_ENEMY && c2->type == ColliderType::COLLIDER_DEAD) {		//Checks that player collides with something that he can die

			pretimer = SDL_GetTicks();	//Sets the PreTime to death timer
		
			if (preposition.y < c2->rect.y || position.y == c2->rect.y - entity_colliders->rect.h) {	//Checks that player collider from above
			
				current_animation = &data_golem.death;	//Current Animation is Death
				pretimer = SDL_GetTicks();	//Sets the PreTime to death timer
				position.y = c2->rect.y - entity_colliders->rect.h;
				current_stateE2 = DEATH2;	//Sets player to Death state
				grounded = true;	//Sets that player is touching the floor
				entity_colliders->to_delete = true;
				App->tex->UnLoad(App->entity->Tex_Golems_Grass);	//Unload The Player texture
			}

			else if (preposition.y > (c2->rect.y + c2->rect.h)) {	//Checks that player collider from below
			
				current_animation = &data_golem.death;	//Current Animation is Death
				pretimer = SDL_GetTicks();	//Sets the PreTime to death timer
				current_stateE2 = DEATH2;	//Sets player to Death state
				grounded = true;	//Sets that player is touching the floor
				entity_colliders->to_delete = true;
				App->tex->UnLoad(App->entity->Tex_Golems_Grass);	//Unload The Player texture
			}

		}

}

void j1Golem1::Reset() {	//Reset All Player Animations

	data_golem.death.Reset();
	//data_enemy.jump.Reset();
	walk.Reset();
	idle.Reset();
	//data_enemy.fall.Reset();
	die = false;
}

bool j1Golem1::PreTime(float sec)
{
	bool ret = false;
	pretimer++;
	if (pretimer >= sec) { ret = true; pretimer = 0; }
	return ret;
}

bool j1Golem1::Pathfinding(float dt) {


		/*if (path->At(1)->y < origin.y) {
			position.y -= 2 * LIMIT_TIMER * dt;
		}

		if (path->At(1)->y > origin.y) {
			position.y += 2 * LIMIT_TIMER * dt;
		}*/
	

	return true;
}

