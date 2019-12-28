#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Audio.h"
#include "j1Textures.h"
#include "j1Coin.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Input.h"
#include "j1Collider.h"
#include "j1Scene.h"
#include "j1EntityManager.h"
#include <math.h>
#include "..//Brofiler/Brofiler.h"

j1Coin::j1Coin(int x, int y) : j1Entity(entity_type::COIN)
{
	data_coin.ipos.x = x;
	data_coin.ipos.y = y;

}
// Destructor
j1Coin::~j1Coin()
{}

bool j1Coin::Awake(pugi::xml_node& config) {

	bool ret = true;

	//Load All Player Features from Config
	//v.x = config.child("velocity").attribute("x").as_int(2);

	return ret;

}

bool j1Coin::Start() {

	pretimer = 0;
	//globaltime = SDL_GetTicks();	//Sets the Global time to the death timer
	position.x = data_coin.ipos.x;
	position.y = data_coin.ipos.y;

	Pushbacks();	//Call all the Pushback of animations

	current_animation = &idle;

	SDL_Rect c;
	c.x = position.x;
	c.y = position.y;
	c.w = 28;
	c.h = 28;

	entity_colliders = App->collider->AddCollider(&c, COLLIDER_COIN, this);	//Sets The Collider Type and Dimensions to Coin

	return	true;

}

bool j1Coin::PreUpdate(float dt) {

	BROFILER_CATEGORY("PreUpdate Coin", Profiler::Color::Red);

	return true;

}

bool j1Coin::Update(float dt) {

	BROFILER_CATEGORY("Update Coin", Profiler::Color::DarkRed);

	current_animation = &idle;

	preposition = position;

	State(dt);	//Set the animation relationed with the state that it is

	DrawCollider();

	//Coin Draw
	if (flip) {
		App->render->Blit(App->entity->Tex_Coin, position.x, position.y, &(current_animation->GetCurrentFrame()), SDL_FLIP_HORIZONTAL, 1.0, 2);	//Draw Player Flipped
	}
	else {
		App->render->Blit(App->entity->Tex_Coin, position.x, position.y, &(current_animation->GetCurrentFrame()), SDL_FLIP_NONE, 1.0, 2);	//Draw Player Normal
	}

	return true;

}

bool j1Coin::PostUpdate(float dt) {

	BROFILER_CATEGORY("PostUpdate Coin", Profiler::Color::IndianRed);

	return true;
}
// Called before quitting
bool j1Coin::CleanUpBat()
{
	LOG("Unloading player");
	j1Entity::CleanUp();

	return true;
}

bool j1Coin::Load(pugi::xml_node& node) {

	if (!node.child("heartposition").empty())	//Load the valors if save_confign file is empty link
	{

		position.x = node.child("heartposition").attribute("x").as_int();	//Load Player X
		position.y = node.child("heartposition").attribute("y").as_int();	//Load Player Y
	}

	else {
		position.x = position.x;	//Load Player X
		position.y = position.y;	//Load Player Y
	}
	return true;

}

bool j1Coin::Save(pugi::xml_node& node) const {

	if (node.child("batposition").empty())	//Save The Position Valors in the save_config file
	{
		pugi::xml_node&  save = node.append_child("heartposition");
		save.append_attribute("x").set_value(position.x);	//Save Player X
		save.append_attribute("y").set_value(position.y);	//Save Player Y
	}
	else
	{
		node.child("heartposition").attribute("x").set_value(position.x);	//Save Player X
		node.child("heartposition").attribute("y").set_value(position.y);	//Save Player Y
	}

	return true;

}

void j1Coin::Pushbacks() {

	//Sets the Idle Animation
	idle.PushBack({ 0, 30,35,35 }, 0.5, 0, 0);
	idle.PushBack({ 39,30,35,35 }, 0.5, 0, 0);
	idle.PushBack({ 79,30,35,35 }, 0.5, 0, 0);
	idle.loop = true;

}

void j1Coin::State(float dt) {

	if (current_stateE5 == IDLE6) {

		current_animation = &idle;

	}

}

void j1Coin::OnCollision(Collider* c1, Collider* c2) {	//Check if the Coin collides with something

	if (c1->type == ColliderType::COLLIDER_COIN && c2->type == ColliderType::COLLIDER_PLAYER) {		//Checks that Coin collides with something that it can be collected

		this->destroy = true;
		this->entity_colliders->to_delete = true;
	}

}

void j1Coin::Reset() {	//Reset All Coin Animations

	idle.Reset();

}

bool j1Coin::PreTime(float sec)
{
	bool ret = false;
	pretimer++;
	if (pretimer >= sec) { ret = true; pretimer = 0; }
	return ret;
}