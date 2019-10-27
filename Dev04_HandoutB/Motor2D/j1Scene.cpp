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
	if (current_map == "Map.tmx") {

		App->audio->PlayMusic("audio/music/map1_music.ogg");
		walk_grass_FX = App->audio->LoadFx("audio/fx/Walk_grass.wav");
		jump_FX= App->audio->LoadFx("audio/fx/Jump.wav");
		death_FX = App->audio->LoadFx("audio/fx/Death.wav");


	}
	else if(current_map=="map2.tmx") {
		
		App->audio->PlayMusic("audio/music/map2_music.ogg");
		walk_rock_FX = App->audio->LoadFx("audio/fx/Walk_rock.wav");
		jump_FX = App->audio->LoadFx("audio/fx/Jump.wav");
		death_FX = App->audio->LoadFx("audio/fx/Death.wav");
	}
	
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
	
	p2List_item<p2SString>* i = maps.start;

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) {


		current_map.create("Map.tmx");
		App->map->CleanUp();
		App->player->CleanUp();

		App->map->Load(current_map.GetString());
		App->player->Start();
		App->map->Draw();
		App->audio->PlayMusic("audio/music/map1_music.ogg");

		App->player->data_player.position.x = 100;
		App->player->data_player.position.y = 300;

	}

	i = i->next;

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) {


		current_map.create("map2.tmx");
		App->audio->PlayMusic("audio/music/map2_music.ogg");

		App->map->CleanUp();
		
		App->player->CleanUp();

		App->map->Load(current_map.GetString());
		App->player->Start();
		App->map->Draw();
		
		//charge map 2 position when F2 is pressed
		App->player->data_player.position.x = 50;
		App->player->data_player.position.y = 10;

	}

	else {


		App->map->Draw();

	}

	if (App->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN)
	{
		if(current_map=="Map.tmx"){
		App->player->data_player.position.x = 100;
		App->player->data_player.position.y = 300;
		App->player->data_player.player_flip = SDL_FLIP_NONE;
		}

		else {
			App->player->data_player.position.x = 50;
			App->player->data_player.position.y = 10;
			App->player->data_player.player_flip = SDL_FLIP_NONE;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) {
		
		App->SaveGame();

		App->player->data_player.position.y -= 20;
	}
		

	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
	App->LoadGame();


	
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
	//App->audio->CleanUp();
	return true;
}

bool j1Scene::Load(pugi::xml_node& data)
{
	LOG("Loading Scene state");
	App->map->CleanUp();
	App->collider->CleanUp();
	
	current_map.create(data.child("scene").attribute("name").as_string());
	App->map->Load(current_map.GetString());
	App->collider->Start();
	App->player->Start();
	//App->audio->Start();
	
	return true;
}

bool j1Scene::Save(pugi::xml_node& data) const
{
	LOG("Saving Scene state");
	pugi::xml_node scene = data.append_child("scene");
	scene.append_attribute("name") = current_map.GetString();

	return true;
}

void j1Scene::SecondMap() {
	
	App->map->CleanUp();
	App->collider->CleanUp();


	p2List_item<p2SString>* i;
	for (i = maps.start; i->data != current_map.GetString(); i = i->next) {
		LOG("%s  %s", i->data.GetString(), current_map.GetString());
	}

	if (i->next != NULL) { i = i->next; }
	else { i = maps.start; }
	current_map = i->data;

	App->map->Load(current_map.GetString());
	App->map->Draw();
	App->collider->Start();
	
	//charge map 1 position when initialize the game

	if (current_map == "Map.tmx") {

		App->player->data_player.position.x = 100;
		App->player->data_player.position.y = 500;
		
		walk_grass_FX = App->audio->LoadFx("audio/fx/Walk_grass.wav");
		App->audio->PlayMusic("audio/music/map1_music.ogg");
		jump_FX = App->audio->LoadFx("audio/fx/Jump.wav");
		death_FX = App->audio->LoadFx("audio/fx/Death.wav");

	}

	//charge map 2 position when player completes level 1
	else if(current_map=="map2.tmx"){

		App->player->data_player.position.x = 50;
		App->player->data_player.position.y = 50;

		App->audio->PlayMusic("audio/music/map2_music.ogg");
		jump_FX = App->audio->LoadFx("audio/fx/Jump.wav");
		walk_rock_FX = App->audio->LoadFx("audio/fx/Walk_rock.wav");
		death_FX = App->audio->LoadFx("audio/fx/Death.wav");

	}

	App->player->Start();
	App->player->Reset();
}