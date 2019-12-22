#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Audio.h"
#include "j1Textures.h"
#include "j1Heart.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Input.h"
#include "j1Collider.h"
#include "j1Scene.h"
#include "j1EntityManager.h"
#include <math.h>
#include "..//Brofiler/Brofiler.h"

j1Heart::j1Heart(int x, int y) : j1Entity(entity_type::HEART)
{
	data_heart.ipos.x = x;
	data_heart.ipos.y = y;

}
// Destructor
j1Heart::~j1Heart()
{}

bool j1Heart::Awake(pugi::xml_node& config) {

	bool ret = true;

	//Load All Player Features from Config
	//v.x = config.child("velocity").attribute("x").as_int(2);

	return ret;

}

bool j1Heart::Start() {

	pretimer = 0;
	//globaltime = SDL_GetTicks();	//Sets the Global time to the death timer
	position.x = data_heart.ipos.x;
	position.y = data_heart.ipos.y;

	Pushbacks();	//Call all the Pushback of animations

	current_animation = &idle;

	SDL_Rect c;
	c.x = position.x;
	c.y = position.y;
	c.w = 28;
	c.h = 28;

	entity_colliders = App->collider->AddCollider(&c, COLLIDER_HEART, this);	//Sets The Collider Type and Dimensions to Player

	return	true;

}

bool j1Heart::PreUpdate(float dt) {

	BROFILER_CATEGORY("PreUpdateHeart", Profiler::Color::Coral);

	return true;

}

bool j1Heart::Update(float dt) {

	current_animation = &idle;

	preposition = position;

	State(dt);	//Set the animation relationed with the state that he is

	DrawCollider();
	
	//Heart Draw
	if (flip) {
		App->render->Blit(App->entity->Tex_Heart, position.x, position.y, &(current_animation->GetCurrentFrame()), SDL_FLIP_HORIZONTAL, 1.0, 2);	//Draw Player Flipped
	}
	else {
		App->render->Blit(App->entity->Tex_Heart, position.x, position.y, &(current_animation->GetCurrentFrame()), SDL_FLIP_NONE, 1.0, 2);	//Draw Player Normal
	}

	return true;

}

bool j1Heart::PostUpdate(float dt) {

	return true;
}
// Called before quitting
bool j1Heart::CleanUpBat()
{
	LOG("Unloading player");
	j1Entity::CleanUp();

	return true;
}

bool j1Heart::Load(pugi::xml_node& node) {

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

bool j1Heart::Save(pugi::xml_node& node) const {

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

void j1Heart::Pushbacks() {

		//Sets the Idle Animation
		idle.PushBack({ 0, 0,28,27 }, 0.5, 0, 0);
		idle.PushBack({ 31,0,28,27 }, 0.25, 0, 0);
		idle.PushBack({ 62,0,28,27 }, 0.15, 0, 0);
		idle.loop = true;

}

void j1Heart::State(float dt) {

	if (current_stateE4 == IDLE5) {

		current_animation = &idle;
		
	}

}

void j1Heart::OnCollision(Collider* c1, Collider* c2) {	//Check if the Player collides with something

	if (c1->type == ColliderType::COLLIDER_HEART && c2->type == ColliderType::COLLIDER_PLAYER) {		//Checks that player collides with something that he can die

		if (preposition.y < (c2->rect.y + c2->rect.h) && App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN) {	//Checks that player collider from above

			pretimer = SDL_GetTicks();	//Sets the PreTime to death timer

		}

	}

}

void j1Heart::Reset() {	//Reset All Player Animations

	idle.Reset();

}

bool j1Heart::PreTime(float sec)
{
	bool ret = false;
	pretimer++;
	if (pretimer >= sec) { ret = true; pretimer = 0; }
	return ret;
}