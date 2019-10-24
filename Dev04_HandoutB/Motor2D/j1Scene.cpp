#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Player.h"
#include "j1Collider.h"

#include <string>

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	pugi::xml_node map;
	for (map= config.child("map"); map; map = map.next_sibling("map"))
	{
		p2SString* lvl = new p2SString();

		lvl->create(map.attribute("name").as_string());
		
		maps.add(lvl->GetString());
	}

	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	LOG("LOADING MAP");
	current_map = maps.start->data;
	App->map->Load(current_map.GetString());
	//App->audio->PlayMusic(); MUSIC NOT ENABLED
	
	
	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{

	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
		App->LoadGame();

	if (App->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN)
		App->SaveGame();

	/*if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y += 10;

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y -= 10;

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x += 10;

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x -= 10;*/
	
	p2List_item<p2SString>* i = maps.start;

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN){
		
		
		current_map.create("Map.tmx");
		
		App->map->CleanUp();
		App->audio->CleanUp();
		App->player->CleanUp();

		App->map->Load(current_map.GetString());
		App->player->Start();
		App->map->Draw();

		App->player->data_player.position.x = 100;
		App->player->data_player.position.y = 300;
		
	} 
	
	i = i->next;

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) {
		
		
		current_map.create("Map2.tmx");
				
		App->map->CleanUp();
		App->audio->CleanUp();
		App->player->CleanUp();

		App->map->Load(current_map.GetString());
		App->player->Start();
		App->map->Draw();

		App->player->data_player.position.x = 150;
		App->player->data_player.position.y = 10;
	}

	else{

	App->map->Draw();
	
	}
	// Show player and map colliders
	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
	{
		if (App->player->data_player.showcolliders == false)
		{
			App->player->data_player.showcolliders = true;
		}
		else if (App->player->data_player.showcolliders == true)
		{
			App->player->data_player.showcolliders = false;
		}
	}

	//int x = 0;
	//int y = 0;

	
	
	/*App->input->GetMousePosition(x, y);
	p2Point<uint> TilePos = App->map->data.GetTilePos(x, y);
	
	p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d Tile:%dx%d ",
		App->map->data.width, App->map->data.height,
		App->map->data.tile_width, App->map->data.tile_height,
		App->map->data.tilesets.count(),
		TilePos.x, TilePos.y);*/
	
	p2SString title("2D PLATFORMER");
	App->win->SetTitle(title.GetString());

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;


	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

/*bool j1Scene::Load(pugi::xml_node& data)
{
	LOG("Loading Scene state");
	App->map->CleanUp();
	sn.create(data.child("scene").attribute("name").as_string());
	App->map->Load(sn.GetString());
	
	return true;
}*/

/*bool j1Scene::Save(pugi::xml_node& data) const
{
	LOG("Saving Scene state");
	pugi::xml_node scene = data.append_child("scene");
	scene.append_attribute("name") = sn.GetString();
	

	return true;
}*/

void j1Scene::SecondMap() {
	
	App->map->CleanUp();
	App->audio->CleanUp();
	App->player->CleanUp();

	p2List_item<p2SString>* i;
	for (i = maps.start; i->data != current_map.GetString(); i = i->next) {
		LOG("%s  %s", i->data.GetString(), current_map.GetString());
	}

	if (i->next != NULL) { i = i->next; }
	else { i = maps.start; }
	current_map = i->data;
	App->map->Load(current_map.GetString());
	App->player->Start();
	App->map->Draw();

}