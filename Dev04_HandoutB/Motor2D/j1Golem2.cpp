#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Audio.h"
#include "j1Textures.h"
#include "j1Golem2.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Input.h"
#include "j1Collider.h"
#include "j1Scene.h"
#include "j1EntityManager.h"
#include <math.h>
#include "..//Brofiler/Brofiler.h"


j1Golem2::j1Golem2(int x, int y) : j1Entity(entity_type::PLAYER)
{
	data_golem2.ipos.x = x;
	data_golem2.ipos.y = y;

}

// Destructor
j1Golem2::~j1Golem2()
{}

bool j1Golem2::Awake(pugi::xml_node& config) {

	bool ret = true;

	//Load All Player Features from Config

	v.x = config.child("velocity").attribute("x").as_int();

	gravity = config.child("gravity").attribute("grav").as_float();

	return ret;

}

bool j1Golem2::Start() {

	pretimer = 0;
	//globaltime = SDL_GetTicks();	//Sets the Global time to the death timer
	position.x = data_golem2.ipos.x;
	position.y = data_golem2.ipos.y;

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

bool j1Golem2::PreUpdate(float dt) {

	BROFILER_CATEGORY("PreUpdateGolem", Profiler::Color::Aquamarine);

	return true;

}

bool j1Golem2::Update(float dt) {

	current_animation = &idle;

	position.y += gravity;
	preposition = position;

	//CheckState(dt);	//Checks the state where is the player
	//State(dt);	//Set the animation relationed with the state that he is
	DrawCollider();
	//Player Draw
	if (flip) {
		App->render->Blit(App->entity->Tex_Golems, position.x, position.y, &(current_animation->GetCurrentFrame()), SDL_FLIP_HORIZONTAL, 1.0);	//Draw Player Flipped
	}
	else {
		App->render->Blit(App->entity->Tex_Golems, position.x, position.y, &(current_animation->GetCurrentFrame()), SDL_FLIP_NONE, 1.0);	//Draw Player Normal
	}

	if (grounded == false) {	//Sets that if the player is not touching the ground puts the fall animation

		//current_state = JUMP_FALL2;

	}

	return true;

}

bool j1Golem2::PostUpdate(float dt) {

	return true;
}
// Called before quitting
bool j1Golem2::CleanUp()
{
	LOG("Unloading player");
	j1Entity::CleanUp();
	App->tex->UnLoad(App->entity->Tex_Golems);	//Unload The Player texture
	App->collider->CleanUp();	//Unload the Player collider

	return true;
}


/*bool j1Golem2::Load(pugi::xml_node& node) {

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
bool j1Golem2::Save(pugi::xml_node& node) const {

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

}*/

void j1Golem2::Pushbacks() {

	//Sets the Idle Animation
		idle.PushBack({ 0,434,44,60 }, 0.4, 0, 0);
		idle.PushBack({ 46,434,44,60 }, 0.4, -2, 0);
		idle.PushBack({ 92,434,44,60 }, 0.4, -2, 0);
		idle.PushBack({ 136,434,44,60 }, 0.4, -4, 0);
		idle.PushBack({ 182,434,44,60 }, 0.4, -4, 0);
		idle.PushBack({ 228,434,44,60 }, 0.4, -2, 0);
		idle.PushBack({ 274,434,44,60 }, 0.4, -4, 0);
		idle.PushBack({ 318,434,44,60 }, 0.4, -4, 0);
		idle.PushBack({ 364,434,44,60 }, 0.4, -4, 0);
		idle.PushBack({ 409,434,44,60 }, 0.4, -2, 0);
		idle.PushBack({ 453,434,44,60 }, 0.4, -2, 0);
		idle.PushBack({ 501,434,44,60 }, 0.4, -2, 0);
		idle.PushBack({ 549,434,44,60 }, 0.4, -4, 0);
		idle.PushBack({ 598,434,44,60 }, 0.4, -4, 0);
		idle.PushBack({ 646,434,44,60 }, 0.4, -4, 0);
		idle.PushBack({ 696,434,44,60 }, 0.4, -4, 0);
		idle.PushBack({ 744,434,44,60 }, 0.4, -4, 0);
		idle.PushBack({ 792,434,44,60 }, 0.4, -4, 0);
		idle.loop = true;
	
		//Sets the Walk Animation
		walk.PushBack({ 0,509,44,60 }, 0.4, 0, 0);
		walk.PushBack({ 48,509,44,60 }, 0.4, 0, 0);
		walk.PushBack({ 95,509,44,60 }, 0.4, 0, 0);
		walk.PushBack({ 144,509,44,60 }, 0.4, 0, 0);
		walk.PushBack({ 192,509,44,60 }, 0.4, 0, 0);
		walk.PushBack({ 240,509,44,60 }, 0.4, 0, 0);
		walk.PushBack({ 288,509,44,60 }, 0.4, 0, 0);
		walk.PushBack({ 335,509,44,60 }, 0.4, 0, 0);
		walk.PushBack({ 382,509,44,60 }, 0.4, 0, 0);
		walk.PushBack({ 431,509,44,60 }, 0.4, 0, 0);
		walk.PushBack({ 479,509,44,60 }, 0.4, 0, 0);
		walk.PushBack({ 526,509,44,60 }, 0.4, 0, 0);
		walk.PushBack({ 575,509,44,60 }, 0.4, 0, 0);
		walk.PushBack({ 624,509,44,60 }, 0.4, 0, 0);
		walk.PushBack({ 672,509,44,60 }, 0.4, 0, 0);
		walk.PushBack({ 720,509,44,60 }, 0.4, 0, 0);
		walk.PushBack({ 769,509,44,60 }, 0.4, 0, 0);
		walk.PushBack({ 816,509,44,60 }, 0.4, 0, 0);
		walk.PushBack({ 865,509,44,60 }, 0.4, 0, 0);
		walk.PushBack({ 914,509,44,60 }, 0.4, 0, 0);
		walk.PushBack({ 961,509,44,60 }, 0.4, 0, 0);
		walk.PushBack({ 1008,509,44,60 }, 0.4, 0, 0);
		walk.PushBack({ 1055,509,44,60 }, 0.4, 0, 0);
		walk.PushBack({ 1104,509,44,60 }, 0.4, 0, 0);
		walk.loop = true;
	
		//Sets the Attack Animation
		data_golem2.attack.PushBack({ 0,586,44,63 }, 0.3, 0, 0);
		data_golem2.attack.PushBack({ 53,586,45,63 }, 0.3, 0, 0);
		data_golem2.attack.PushBack({ 107,586,69,65 }, 0.3, 0, 0);
		data_golem2.attack.PushBack({ 183,586,65,61 }, 0.3, 0, 0);
		data_golem2.attack.PushBack({ 258,586,63,59 }, 0.3, 0, 0);
		data_golem2.attack.PushBack({ 331,586,65,59 }, 0.3, 0, 0);
		data_golem2.attack.PushBack({ 407,586,64,59 }, 0.3, 0, 0);
		data_golem2.attack.PushBack({ 483,586,64,59 }, 0.3, 0, 0);
		data_golem2.attack.PushBack({ 556,586,66,59 }, 0.3, 0, 0);
		data_golem2.attack.PushBack({ 631,586,62,61 }, 0.3, 0, 0);
		data_golem2.attack.PushBack({ 703,586,65,61 }, 0.3, 0, 0);
		data_golem2.attack.PushBack({ 776,586,66,61 }, 0.3, 0, 0);
		data_golem2.attack.loop = false;
	
		//Sets the Hurt Animation
		data_golem2.hurt.PushBack({ 0,663,42,64 }, 0.3, 0, 0);
		data_golem2.hurt.PushBack({ 52,663,45,63 }, 0.3, 0, 0);
		data_golem2.hurt.PushBack({ 107,663,44,62 }, 0.3, 0, 0);
		data_golem2.hurt.PushBack({ 161,663,45,64 }, 0.3, 0, 0);
		data_golem2.hurt.PushBack({ 216,663,46,64 }, 0.3, 0, 0);
		data_golem2.hurt.PushBack({ 269,663,50,63 }, 0.3, 0, 0);
		data_golem2.hurt.PushBack({ 323,663,52,61 }, 0.3, 0, 0);
		data_golem2.hurt.PushBack({ 377,663,46,63 }, 0.3, 0, 0);
		data_golem2.hurt.PushBack({ 431,663,45,65 }, 0.3, 0, 0);
		data_golem2.hurt.PushBack({ 484,663,45,64 }, 0.3, 0, 0);
		data_golem2.hurt.PushBack({ 538,663,43,64 }, 0.3, 0, 0);
		data_golem2.hurt.PushBack({ 591,663,43,62 }, 0.3, 0, 0);
		data_golem2.hurt.loop = false;
	
		//Sets the Death Animation
		data_golem2.death.PushBack({ 0,740,42,61 }, 0.3, 0, 0);
		data_golem2.death.PushBack({ 50,740,44,61 }, 0.3, 0, 1);
		data_golem2.death.PushBack({ 102,740,45,61 }, 0.3, 0, 2);
		data_golem2.death.PushBack({ 158,740,44,61 }, 0.3, 0, 4);
		data_golem2.death.PushBack({ 214,740,41,61 }, 0.3, 0, 8);
		data_golem2.death.PushBack({ 271,740,47,60 }, 0.3, 0, 10);
		data_golem2.death.PushBack({ 329,740,53,54 }, 0.3, 0, 15);
		data_golem2.death.PushBack({ 388,740,58,49 }, 0.3, 0, 20);
		data_golem2.death.PushBack({ 454,740,59,46 }, 0.3, 0, 20);
		data_golem2.death.PushBack({ 529,740,60,46 }, 0.3, 0, 20);
		data_golem2.death.PushBack({ 602,740,60,46 }, 0.3, 0, 20);
		data_golem2.death.PushBack({ 676,740,59,45 }, 0.3, 0, 20);
		data_golem2.death.PushBack({ 751,740,59,46 }, 0.3, 0, 20);
	
		data_golem2.death.loop = false;


}

void j1Golem2::CheckState(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		current_animation = &walk;
	}
	if (App->input->GetKey(SDL_SCANCODE_X) == KEY_REPEAT) {
		current_animation = &data_golem2.attack;
		data_golem2.attack.Reset();
	}
	if (App->input->GetKey(SDL_SCANCODE_V) == KEY_REPEAT) {
		current_animation = &data_golem2.death;
		data_golem2.death.Reset();

	}
	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT) {
		current_animation = &data_golem2.hurt;
		data_golem2.hurt.Reset();

	}

}

void j1Golem2::State(float dt) {

	if (current_stateE3 == IDLE3) {

		current_animation = &idle;
		fall.Reset();

	}

	if (current_stateE3 == WALK3) {

		current_animation = &walk;
		fall.Reset();

	}

	if (current_stateE3 == DEATH3) {

		die = true;	//Sets the die to true
		LOG("GLOBAL: %d", globaltime);
		LOG("PRE: %d", pretimer);
		if (die == true) {

			if (App->scene->current_map == "Map.tmx") {	//If player is in map 1



				if (PreTime(20)) {	//Do a timer to stop the game during the Death animation



					//Sets the Position that player goes when he dies
					position.x = 100;	//Set Player X	
					position.y = 300;	//Set Player Y
					data_golem2.death.Reset();
				}

			}

			else {	//If player is not in map 1 is in map 2



				if (PreTime(20)) {	//Do a timer to stop the game during the Death Animation
					current_animation = &data_golem2.death;	//Current Animation is Death
					App->audio->PlayFx(App->scene->death_FX);	//Sets the Death Audio
					//Sets the Position that player goes when he dies
					position.x = 55;	//Set Player X	
					position.y = 10;	//Set Player Y
					data_golem2.death.Reset();

				}

			}


		}
		die = false;

	}

}

void j1Golem2::OnCollision(Collider* c1, Collider* c2) {	//Check if the Player collides with something



	if (c1->type == ColliderType::COLLIDER_ENEMY && c2->type == ColliderType::COLLIDER_FLOOR) {	//If player collide with floor

		if (preposition.y < c2->rect.y || position.y == c2->rect.y - entity_colliders->rect.h) {	//Checks that player collider from above

			position.y = c2->rect.y - entity_colliders->rect.h;
			grounded = true;	//Sets that player is touching the floor

		}

		else if (preposition.y > (c2->rect.y + c2->rect.h)) {	//Checks that player collider from below

			position.y = c2->rect.y + c2->rect.h;
			

		}

		else if ((position.x < c2->rect.x + c2->rect.w && position.x > c2->rect.x) || (position.x + entity_colliders->rect.w < c2->rect.x + c2->rect.w && position.x + entity_colliders->rect.w > c2->rect.x)) {	//Checks that player collider from sides

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

		//PreTime = SDL_GetTicks();	//Sets the PreTime to death timer

		if (preposition.y < c2->rect.y || position.y == c2->rect.y - entity_colliders->rect.h) {	//Checks that player collider from above

			current_animation = &data_golem2.death;	//Current Animation is Death
			App->audio->PlayFx(App->scene->death_FX);	//Sets the Death Audio
			position.y = c2->rect.y - entity_colliders->rect.h;
			//current_state = DEATH2;	//Sets player to Death state

			grounded = true;	//Sets that player is touching the floor


		}

		else if (preposition.y > (c2->rect.y + c2->rect.h)) {	//Checks that player collider from below

			current_animation = &data_golem2.death;	//Current Animation is Death
			App->audio->PlayFx(App->scene->death_FX);	//Sets the Death Audio
			pretimer = SDL_GetTicks();	//Sets the PreTime to death timer

			//data_entity.position.y = c2->rect.y + c2->rect.h;
			current_stateE3 = DEATH3;	//Sets player to Death state

			grounded = true;	//Sets that player is touching the floor


		}

	}

	if (c1->type == ColliderType::COLLIDER_ENEMY && c2->type == ColliderType::COLLIDER_NEXT) {

		if (preposition.y < c2->rect.y || position.y == c2->rect.y - entity_colliders->rect.h) {	//Checks that player collider from above	
			App->scene->SecondMap();	//Pass to next map
		}

		else if (preposition.y > (c2->rect.y + c2->rect.h)) {	//Checks that player collider from below
			App->scene->SecondMap();	//Pass to next map
		}

		else if ((position.x < c2->rect.x + c2->rect.w && position.x > c2->rect.x) || (position.x + entity_colliders->rect.w < c2->rect.x + c2->rect.w && position.x + entity_colliders->rect.w > c2->rect.x)) {	//Checks that player collider from sides

			if ((position.x + entity_colliders->rect.w) < (c2->rect.x + c2->rect.w)) {		//Checks that player collides from left
				App->scene->SecondMap();	//Pass to next map
			}
			else if (position.x < (c2->rect.x + c2->rect.w)) {	//Checks that player collides from right
				App->scene->SecondMap();	//Pass to next map
			}
		}
	}

}



void j1Golem2::Reset() {	//Reset All Player Animations

	data_golem2.death.Reset();
	walk.Reset();
	idle.Reset();
	fall.Reset();
	die = false;
}

bool j1Golem2::PreTime(float sec)
{
	bool ret = false;
	pretimer++;
	if (pretimer >= sec) { ret = true; pretimer = 0; }
	return ret;
}