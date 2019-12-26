#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Audio.h"
#include "j1Textures.h"
#include "j1Bat.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Input.h"
#include "j1Collider.h"
#include "j1Scene.h"
#include "j1EntityManager.h"
#include "j1Pathfinding.h"
#include <math.h>
#include "..//Brofiler/Brofiler.h"


j1Bat::j1Bat(int x, int y) : j1Entity(entity_type::BAT_ENEMY)
{
	name.create("bat");
	data_bat.ipos.x = x;
	data_bat.ipos.y = y;

}

// Destructor
j1Bat::~j1Bat()
{}

bool j1Bat::Awake(pugi::xml_node& config) {

	bool ret = true;

	//Load All Player Features from Config
	pugi::xml_document	config_file;
	config = App->LoadConfig(config_file);
	config = config.child("bat");

	folder.create(config.child("folder").child_value());

	v.x = config.child("velocity").attribute("x").as_int(2);

	LOG("%d", v.x);
	return ret;

}

bool j1Bat::Start() {

	pretimer = 0;
	//globaltime = SDL_GetTicks();	//Sets the Global time to the death timer
	position.x = data_bat.ipos.x;
	position.y = data_bat.ipos.y;

	Pushbacks();	//Call all the Pushback of animations

	current_animation = &idle;

	SDL_Rect c;
	c.x = position.x;
	c.y = position.y;
	c.w = 37;
	c.h = 28;

	entity_colliders = App->collider->AddCollider(&c, COLLIDER_ENEMY, this);	//Sets The Collider Type and Dimensions to Player

	return	true;

}

bool j1Bat::PreUpdate(float dt) {

	BROFILER_CATEGORY("PreUpdateGolem", Profiler::Color::Aquamarine);

	return true;

}

bool j1Bat::Update(float dt) {

	current_animation = &idle;

	preposition = position;

	CheckState(dt);	//Checks the state where is the player
	State(dt);	//Set the animation relationed with the state that he is


	if (App->entity->GetPlayer()->position.x > position.x - 500 && App->entity->GetPlayer()->position.x < position.x + 500 && App->entity->GetPlayer()->position.y + 150 && App->entity->GetPlayer()->position.y - 150) {

		const p2DynArray<iPoint>* path = App->pathfinding->GetLastPath();

		if (App->collider->debug == true)
		{
			for (uint i = 0; i < path->Count(); ++i)
			{
				iPoint next_position = App->map->MapToWorld(path->At(i)->x, path->At(i)->y);
				App->render->Blit(App->scene->debug_tex, next_position.x, next_position.y);

			}

		}

		if (data_bat.pathfinding == true) {

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
					current_stateE3 == WALK4;
					position.x -= 4 * LIMIT_TIMER * dt;
					flip = true;
				}

				if (path_position.x > origin.x) {
					current_stateE3 == WALK4;
					position.x += 4 * LIMIT_TIMER * dt;
					flip = false;
				}

				if (path_position.y < origin.y) {
					current_stateE3 == WALK4;
					position.y -= 4 * LIMIT_TIMER * dt;
					
				}

				if (path_position.y > origin.y) {
					current_stateE3 == WALK4;
					position.y += 4 * LIMIT_TIMER * dt;
					
				}

			}
		}
	}

	DrawCollider();
	//Player Draw
	if (flip) {
		App->render->Blit(App->entity->Tex_Bat, position.x, position.y, &(current_animation->GetCurrentFrame()), SDL_FLIP_HORIZONTAL, 1.0, 2);	//Draw Player Flipped
	}
	else {
		App->render->Blit(App->entity->Tex_Bat, position.x, position.y, &(current_animation->GetCurrentFrame()), SDL_FLIP_NONE, 1.0, 2);	//Draw Player Normal
	}

	if (grounded == false) {	//Sets that if the player is not touching the ground puts the fall animation

		//current_state = JUMP_FALL2;

	}

	return true;

}

bool j1Bat::PostUpdate(float dt) {

	return true;
}
// Called before quitting
bool j1Bat::CleanUpBat()
{
	LOG("Unloading player");
	j1Entity::CleanUp();

	return true;
}


bool j1Bat::Load(pugi::xml_node& node) {

	if (!node.child("batposition").empty())	//Load the valors if save_confign file is empty link
	{

		position.x = node.child("batposition").attribute("x").as_int();	//Load Player X
		position.y = node.child("batposition").attribute("y").as_int();	//Load Player Y
	}

	else {
		position.x = position.x;	//Load Player X
		position.y = position.y;	//Load Player Y
	}
	return true;

}
bool j1Bat::Save(pugi::xml_node& node) const {

	if (node.child("batposition").empty())	//Save The Position Valors in the save_config file
	{
		pugi::xml_node&  save = node.append_child("batposition");
		save.append_attribute("x").set_value(position.x);	//Save Player X
		save.append_attribute("y").set_value(position.y);	//Save Player Y
	}
	else
	{
		node.child("batposition").attribute("x").set_value(position.x);	//Save Player X
		node.child("batposition").attribute("y").set_value(position.y);	//Save Player Y
	}

	return true;

}

void j1Bat::Pushbacks() {

	//Sets the Idle Animation
		idle.PushBack({ 41,6,37,28 }, 0.4, 0, 0);
		idle.PushBack({ 82,6,39,20 }, 0.4, -2, 0);
		idle.PushBack({ 124,0,39,27 }, 0.4, -2, 0);
		idle.loop = true;
	
		//Sets the Walk Animation
		walk.PushBack({ 49,42,24,28 }, 0.4, 0, 0);
		walk.PushBack({ 91,48,24,21 }, 0.4, 0, 0);
		walk.PushBack({ 135,49,22,29 }, 0.4, 0, 0);
		walk.loop = true;
	
		//Sets the Death Animation
		data_bat.death.PushBack({ 124,0,39,27 }, 0.3, 0, 0);
		data_bat.death.PushBack({ 41,6,37,28 }, 0.3, 0, 1);
		data_bat.death.PushBack({ 0,25,32,14 }, 0.3, 0, 2);
		data_bat.death.loop = false;


}

void j1Bat::CheckState(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		current_animation = &walk;
	}
	if (App->input->GetKey(SDL_SCANCODE_X) == KEY_REPEAT) {
		//current_animation = &data_bat.attack;
		//data_bat.attack.Reset();
	}
	if (App->input->GetKey(SDL_SCANCODE_V) == KEY_REPEAT) {
		current_animation = &data_bat.death;
		data_bat.death.Reset();

	}
	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT) {
		//current_animation = &data_bat.hurt;
		//data_bat.hurt.Reset();

	}

}

void j1Bat::State(float dt) {

	if (current_stateE3 == IDLE4) {

		current_animation = &idle;
		fall.Reset();

	}

	if (current_stateE3 == WALK4) {

		current_animation = &walk;
		fall.Reset();

	}

	if (current_stateE3 == DEATH4) {

		if (App->scene->current_map == "Map.tmx") {	//If player is in map 1

			current_animation = &data_bat.death;	//Current Animation is Death
		}

		else {	//If player is not in map 1 is in map 2

			current_animation = &data_bat.death;	//Current Animation is Death
		}

	}

}

void j1Bat::OnCollision(Collider* c1, Collider* c2) {	//Check if the Player collides with something

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

		if (preposition.y < c2->rect.y || position.y == c2->rect.y - entity_colliders->rect.h) {	//Checks that player collider from above

			pretimer = SDL_GetTicks();	//Sets the PreTime to death timer
			data_bat.dead = true;
			data_bat.pathfinding = false;
			current_stateE3 = DEATH4;
		}

		else if (preposition.y > (c2->rect.y + c2->rect.h)) {	//Checks that player collider from below
			current_stateE3 = DEATH4;
			pretimer = SDL_GetTicks();	//Sets the PreTime to death timer
			data_bat.dead = true;
			data_bat.pathfinding = false;
			current_stateE3 = DEATH4;
		}

	}

	if (c1->type == ColliderType::COLLIDER_ENEMY && c2->type == ColliderType::COLLIDER_PLAYER) {		//Checks that player collides with something that he can die

		if (preposition.y < (c2->rect.y + c2->rect.h) && App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN) {	//Checks that player collider from above

			pretimer = SDL_GetTicks();	//Sets the PreTime to death timer
			current_stateE3 = DEATH4;
			data_bat.pathfinding = false;
			data_bat.dead = true;

		}

	}

}



void j1Bat::Reset() {	//Reset All Player Animations

	data_bat.death.Reset();
	walk.Reset();
	idle.Reset();
	fall.Reset();
	die = false;
}

bool j1Bat::PreTime(float sec)
{
	bool ret = false;
	pretimer++;
	if (pretimer >= sec) { ret = true; pretimer = 0; }
	return ret;
}