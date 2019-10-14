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

	data_player.vj.x = config.child("jump_velocity").attribute("x").as_int();
	data_player.vj.y = config.child("jump_velocity").attribute("y").as_int();

	data_player.v.x = config.child("velocity").attribute("x").as_int();
	data_player.v.y = config.child("velocity").attribute("y").as_int();

	data_player.walkFX = config.child("walkFX").attribute("source").as_string();
	data_player.deathFX = config.child("deathFX").attribute("source").as_string();

	data_player.colOffset.x = config.child("colOffset").attribute("x").as_int();
	data_player.colOffset.y = config.child("colOffset").attribute("y").as_int();

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

	
	data_player.injump = false;
	
	Pushbacks();
	data_player.current_animation = &data_player.idle;
	
	data_player.v = { 1,0 };

	App->audio->LoadFx(data_player.walkFX.GetString());
	App->audio->LoadFx(data_player.deathFX.GetString());

	data_player.Tex_Player = App->tex->Load(PATH(folder.GetString(), texture.GetString()));

	//Sets the player in the start position

	for (p2List_item<OBJG*>* obj = App->map->data.objects.start; obj; obj = obj->next)
	{
		if (obj->data->name == ("colisions")){

			for (p2List_item<MapObject*>* objdata = obj->data->objects.start; objdata; objdata = objdata->next){

				if (objdata->data->name == ("player")){

					data_player.col.h = objdata->data->height;
					data_player.col.w = objdata->data->width;
					data_player.col.x = objdata->data->x;
					data_player.col.y = objdata->data->y;
				}

				else if (objdata->data->name == ("startpoint")){

					data_player.position = { objdata->data->x, objdata->data->y };
					data_player.col.x = data_player.position.x + data_player.colOffset.x;
					data_player.col.y = data_player.position.y + data_player.colOffset.y;
				}
			}
		}
	}

	data_player.Tex_Player = App->tex->Load(PATH(folder.GetString(), texture.GetString()));
	
	return	true;

}

bool j1Player::PreUpdate() {

	return true;

}

bool j1Player::Update() {

	
	return true;

}

bool j1Player::PostUpdate() {

	
	Animation();
	//Camera_Player();
	return true;
}



//Called to start the player at initial pos
//void j1Player::Restart() {

//}

//Change the sound depending the animation
//void j1Player::SFX(int channel, int repeat) {

	

//}

//Puts the camera on player and follows




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

	data_player.vj.x = node.child("jump_velocity").attribute("x").as_int();
	data_player.vj.y = node.child("jump_velocity").attribute("y").as_int();

	data_player.v.x = node.child("velocity").attribute("x").as_int();
	data_player.v.y = node.child("velocity").attribute("y").as_int();

	return true;

}
bool j1Player::Save(pugi::xml_node& node) const {

	node.append_child("position").append_attribute("x") = data_player.position.x;
	node.child("position").attribute("y") = data_player.position.y;

	node.append_child("acceleration").append_attribute("x") = data_player.a.x;
	node.child("acceleration").attribute("y") = data_player.a.y;

	node.append_child("jump_velocity").append_attribute("x") = data_player.vj.x;
	node.child("jump_velocity").attribute("y") = data_player.vj.y;

	node.append_child("velocity").append_attribute("x") = data_player.v.x;
	node.child("velocity").attribute("y") = data_player.v.y;

	return true;

}


iPoint j1Player::Collider_Overlay(iPoint ivec)
{
	//data_player.grounded = false;

	SDL_Rect Collider;
	Collider = data_player.col;
	Collider.x += ivec.x;
	Collider.y += ivec.y;

	SDL_Rect result;

	iPoint nvec = ivec;
	for (p2List_item<OBJG*>* obj = App->map->data.objects.start; obj; obj = obj->next)
	{
		if (obj->data->name == ("Colisions"))
		{
			for (p2List_item<MapObject*>* objdata = obj->data->objects.start; objdata; objdata = objdata->next)
			{
				if (objdata->data->name == ("floor"))
				{
					if (SDL_IntersectRect(&Collider, &CreateRect_FromObjData(objdata->data), &result))
					{
						nvec = AvoidCollision(nvec, result, objdata);
					}
				}
				
			}
		}
	}

	return nvec;
}

iPoint j1Player::AvoidCollision(iPoint nvec, const SDL_Rect result, p2List_item<MapObject*>* objdata)
{
	//Checks to determine the position of the player and the other collider. 
	//The speed is adjousted using the resultant Rect from the collision
	if (nvec.y > 0)
	{
		if (data_player.position.y + data_player.col.h + data_player.colOffset.y <= objdata->data->y)
		{
			if (nvec.x > 0)
			{
				if (result.h <= result.w || data_player.position.x + data_player.col.w + data_player.colOffset.x >= objdata->data->x)
					nvec.y -= result.h;
				else
					nvec.x -= result.w;
			}
			else if (nvec.x < 0)
			{
				if (result.h <= result.w || data_player.position.x + data_player.colOffset.x >= objdata->data->x + objdata->data->width)
					nvec.y -= result.h;
				else
					nvec.x += result.w;
			}
			
		}
		else
		{
			if (nvec.x > 0)
				nvec.x -= result.w;
			else
				nvec.x += result.w;
		}

	}
	else if (nvec.y < 0)
	{
		if (data_player.position.y + data_player.colOffset.y >= objdata->data->y + objdata->data->height)
		{
			if (nvec.x > 0)
			{
				if (result.h <= result.w || data_player.position.x + data_player.col.w + data_player.colOffset.x >= objdata->data->x)
					nvec.y += result.h;
				else
					nvec.x -= result.w;
			}
			else if (nvec.x < 0)
			{
				if (result.h <= result.w || data_player.position.x + data_player.colOffset.x <= objdata->data->x + objdata->data->width)
					nvec.y += result.h;
				else
					nvec.x += result.w;
			}
			else
				nvec.y += result.h;
		}
		else
		{
			if (nvec.x > 0)
				nvec.x -= result.w;
			else if (nvec.x < 0)
				nvec.x += result.w;
			else
				nvec.y += result.h;
		}
	}
	else
	{
		if (nvec.x > 0)
			nvec.x -= result.w;
		else if (nvec.x < 0)
			nvec.x += result.w;
	}
	return nvec;
}

SDL_Rect j1Player::CreateRect_FromObjData(MapObject* data)
{
	SDL_Rect ret;
	ret.x = data->x;
	ret.y = data->y;
	ret.h = data->height;
	ret.w = data->width;
	return ret;
}

void j1Player::Pushbacks() {

	data_player.idle.PushBack({ 7,554,48,52 });
	data_player.idle.PushBack({ 84,554,41,52 });
	data_player.idle.PushBack({ 156,555,42,50 });
	data_player.idle.PushBack({ 232,557,41,48 });
	data_player.idle.PushBack({ 291,557,41,48 });
	data_player.idle.PushBack({ 351,557,40,48 });
	data_player.idle.PushBack({ 415,556,41,49 });
	data_player.idle.PushBack({ 474,556,42,49 });
	data_player.idle.PushBack({ 533,557,41,48 });
	data_player.idle.PushBack({ 591,554,41,51 });
	data_player.idle.PushBack({ 643,554,52,52 });
	data_player.idle.PushBack({ 705,555,44,50 });
	data_player.idle.PushBack({ 763,554,43,52 });
	data_player.idle.PushBack({ 819,554,45,52 });
	data_player.idle.PushBack({ 880,554,43,51 });
	data_player.idle.PushBack({ 938,555,43,51 });
	data_player.idle.PushBack({ 995,554,44,52 });
	data_player.idle.loop = true;
	data_player.idle.speed = 0.3f;

	data_player.walk.PushBack({ 7,4,42,49 });
	data_player.walk.PushBack({ 65,4,41,49 });
	data_player.walk.PushBack({ 131,3,38,50 });
	data_player.walk.PushBack({ 201,3,36,51 });
	data_player.walk.PushBack({ 259,5,38,48 });
	data_player.walk.PushBack({ 317,4,44,50 });
	data_player.walk.loop = true;
	data_player.walk.speed = 0.3f;

	data_player.walk2.PushBack({ 7,4,42,49 });
	data_player.walk2.PushBack({ 65,4,41,49 });
	data_player.walk2.PushBack({ 131,3,38,50 });
	data_player.walk2.PushBack({ 201,3,36,51 });
	data_player.walk2.PushBack({ 259,5,38,48 });
	data_player.walk2.PushBack({ 317,4,44,50 });
	data_player.walk2.loop = true;
	data_player.walk2.speed = 0.6f;

	data_player.jump.PushBack({ 7,128,52,43 });
	data_player.jump.PushBack({ 81,114,47,57 });
	data_player.jump.PushBack({ 155,113,42,58 });
	data_player.jump.PushBack({ 221,118,52,53 });
	data_player.jump.PushBack({ 285,116,53,55 });
	data_player.jump.PushBack({ 348,119,56,52 });
	data_player.jump.PushBack({ 423,129,53,42 });
	data_player.jump.loop = true;
	data_player.jump.speed = 0.5f;

	data_player.death.PushBack({ 8,464,48,55 });
	data_player.death.PushBack({ 72,456,62,63 });
	data_player.death.PushBack({ 150,454,69,65 });
	data_player.death.PushBack({ 257,464,82,56 });
	data_player.death.PushBack({ 367,447,83,72 });
	data_player.death.PushBack({ 488,447,84,73 });
	data_player.death.PushBack({ 609,457,94,62 });
	data_player.death.PushBack({ 734,455,113,65 });
	data_player.death.PushBack({ 894,445,127,74 });
	data_player.death.PushBack({ 1054,433,116,87 });
	data_player.death.loop = true;
	data_player.death.speed = 1.0f;


}

void j1Player::Animation()
{

	data_player.current_animation = &data_player.idle;		//If any key pressed animation idle

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {		//if "D" is pressed animation walk forward 


		data_player.player_flip = false;
		data_player.current_animation = &data_player.walk;
		App->render->camera.x += 1;
		data_player.position.x += 6;

	}

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {		//if "A" is pressed animation walk backward actives flips to the Blit

		data_player.player_flip = true;
		data_player.current_animation = &data_player.walk;
		App->render->camera.x -= 1;
		data_player.position.x -= 6;
	}

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) {		//if "A" and "SHIFT" are pressed animation walk backward actives flips to the Blit

		data_player.player_flip = true;
		data_player.current_animation = &data_player.walk2;
		App->render->camera.x -= 1;
		data_player.position.x -= 11;
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) {

		data_player.player_flip = false;
		data_player.current_animation = &data_player.walk2;
		App->render->camera.x += 1;
		data_player.position.x += 11;
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN || (data_player.injump == true)) {		//if "SPACE" is pressed 

		/*data_player.injump= true;
		data_player.player_flip = false;
		data_player.current_animation = &data_player.jump;
		data_player.initialposy = data_player.position.y;

		data_player.position.y -= data_player.jumpspeed;
		data_player.position.x += data_player.jumpspeedx;
		if(data_player.position.y < data_player.position.y + 20)

		{
			data_player.jumpspeed += 2;
			if (data_player.jumpspeed < 0)
			{
				data_player.jumpspeed = -4;
				data_player.jumpspeedx = 4;
			}
		}
			//data_player.position.y = data_player.initialposy;
	}
	data_player.injump = false;*/
	
	/*if (SDL_GetTicks() - jumpf_timer > JUMPF_TIME && data_player.position.y == 220)
	{
		inputs.Push(IN_JUMPF_FINISH);
		jumpf_timer = 0;

		position.y = 220;
		jumpspeed = 6;
		animdone = true;
	}*/


		data_player.current_animation = &data_player.jump;
		data_player.injump = true;
		float jumpspeed = 6;
		data_player.position.y -= jumpspeed;
		jumpspeed -= 0.2;

		if (jumpspeed < -7)
		{
			data_player.injump = false;
			data_player.position.y = 220;
			jumpspeed = 6;
		}

	}

}

/*void j1Player::Camera_Player() {

	App->render->camera.x = data_player.position.x + App->win->width;
	App->render->camera.y = -data_player.position.y + App->win->height/1.2;
}*/
