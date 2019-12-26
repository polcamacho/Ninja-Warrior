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
#include "j1EntityManager.h"
#include "j1Entity.h"
#include "j1Pathfinding.h"
#include "j1Fonts.h"
#include "j1Image.h"
#include "j1FadeToBlack.h"
#include "UI_Slider.h"
#include "j1Gui.h"
#include "j1MainMenu.h"
#include "p2SString.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
	scene_change = false;
	is_changed = false;
	first_level = false;
	scene_change_timer = false;
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
		map_list.add(lvl->GetString());

	}

	return true;

}

// Called before the first frame

// Loads map and audio
bool j1Scene::Start()
{
	if (App->map->active==true) {

		LOG("LOADING MAP");
		current_map = map_list.start->data;
	
		App->map->Load(current_map.GetString());

		CreateEntities();

		//load audio from map 1
		if (current_map == "Map.tmx") {
			App->audio->PlayMusic("audio/music/map1_music.ogg");
			jump_FX= App->audio->LoadFx("audio/fx/Jump.wav");
			death_FX = App->audio->LoadFx("audio/fx/Death.wav");
		}

		//load audio from map 2
		else if(current_map=="map2.tmx") {
			App->audio->PlayMusic("audio/music/map2_music.ogg");
			jump_FX = App->audio->LoadFx("audio/fx/Jump.wav");
			death_FX = App->audio->LoadFx("audio/fx/Death.wav");
		}
	
	}

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate(float dt)
{
	// debug pathfing ------------------
	//static iPoint origin;
	//static bool origin_selected = false;
	
	/*int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y);
	p = App->map->WorldToMap(p.x, p.y);

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		
		if (origin_selected == true)
		{
			App->pathfinding->CreatePath(origin, p);
			origin_selected = false;
			LOG("%d,%d,%d,%d", origin.x, origin.y, p.x, p.y);
		}
		else
		{
			origin = p;
		
			origin_selected = true;
		}
	}*/

	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{



	int w, h;
	uchar* data = NULL;
	if (App->map->CreateWalkabilityMap(w, h, &data))
		App->pathfinding->SetMap(w, h, data);
	RELEASE_ARRAY(data);

	//save player position in every map
	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) {

		App->SaveGame();
	}

	//load player position in every map
	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) {
		App->LoadGame();
	}

	if (App->input->GetKey(SDL_SCANCODE_KP_PLUS) == KEY_DOWN) {
		App->audio->Change_Volume(0.1, 1);
	}
	else if (App->input->GetKey(SDL_SCANCODE_KP_MINUS) == KEY_DOWN) {
		App->audio->Change_Volume(0.1, 0);
	}

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) {

		first_level = true;
		FirstLevel();

	}

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) {

		SecondLevel();

	}

	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN) {

		RestartCurrentLevel();

	}

	/*p2List_item<p2SString>* i = maps.start;

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) {


		App->map->CleanUp();
		App->map->Load("Map.tmx");

		/*App->audio->PlayMusic("audio/music/map1_music.ogg");
		jump_FX = App->audio->LoadFx("audio/fx/Jump.wav");
		death_FX = App->audio->LoadFx("audio/fx/Death.wav");

		/*int w, h;
		uchar* data = NULL;
		if (App->map->CreateWalkabilityMap(w, h, &data))
			App->pathfinding->SetMap(w, h, data);
		RELEASE_ARRAY(data);

		App->entity->CleanEntity();
		CreateEntities();
		
	}

	i = i->next;

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) {


		App->map->CleanUp();
		App->map->Load("map2.tmx");

		/*App->audio->PlayMusic("audio/music/map2_music.ogg");						//load audio from map 1
		jump_FX = App->audio->LoadFx("audio/fx/Jump.wav");
		death_FX = App->audio->LoadFx("audio/fx/Death.wav");

		/*int w, h;
		uchar* data = NULL;
		if (App->map->CreateWalkabilityMap(w, h, &data))
			App->pathfinding->SetMap(w, h, data);
		RELEASE_ARRAY(data);

		App->entity->CleanEntity();
		CreateEntities();
		
		//App->map->Draw();

	}

	else {

		
		App->map->Draw();		//draws the correspondant map


	}

	//restart map and puts the player at the beginning of it
	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
	{
		App->entity->CleanEntity();
		CreateEntities();
		if (current_map == "Map.tmx") {
			App->map->Load("Map.tmx");
		}
		else if (current_map == "map2.tmx") {
			App->map->Load("map2.tmx");
		}
	}*/
	
	/*// Debug pathfinding ------------------------------
	int x = 0, y = 0;
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y);
	p = App->map->WorldToMap(p.x, p.y);
	p = App->map->MapToWorld(p.x, p.y);

	App->render->Blit(debug_tex, p.x, p.y);

	const p2DynArray<iPoint>* path = App->pathfinding->GetLastPath();
	//LOG("%d", path->Count());

	for (uint i = 0; i < path->Count(); ++i)
	{
		iPoint pos = App->map->MapToWorld(path->At(i)->x, path->At(i)->y);
		App->render->Blit(debug_tex, pos.x, pos.y);
	}*/
	
	App->map->Draw();

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate(float dt)
{
	bool ret = true;

	App->entity->Enable();
	App->audio->Enable();
	App->collider->Enable();
	App->pathfinding->Enable();

	if (cont==0){
		
		//load audio from map 1
		if (App->scene->current_map == "Map.tmx") {
			App->audio->PlayMusic("audio/music/map1_music.ogg");
			App->scene->jump_FX = App->audio->LoadFx("audio/fx/Jump.wav");
			App->scene->death_FX = App->audio->LoadFx("audio/fx/Death.wav");
		}

		//load audio from map 2
		else if (App->scene->current_map == "map2.tmx") {
			App->audio->PlayMusic("audio/music/map2_music.ogg");
			App->scene->jump_FX = App->audio->LoadFx("audio/fx/Jump.wav");
			App->scene->death_FX = App->audio->LoadFx("audio/fx/Death.wav");
		}
		
		cont++;

	}

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

//load function
bool j1Scene::Load(pugi::xml_node& data)
{
	LOG("Loading Scene state");
	current_map = data.child("scene").attribute("name").as_string();					//check which map have to be loaded and load it
	
	if (current_map == "Map.tmx") {
		
		first_level = true;
		FirstLevel();
	}
	
	else if (current_map == "map2.tmx") {
		
		SecondLevel();

	}

	/*App->map->CleanUp();
	App->collider->CleanUp();   //cleans the colliders and map and entities
	App->entity->CleanEntity();
	
	App->map->Load(current_map.GetString());
	App->collider->Start();
	CreateEntities();*/
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
	
	if (scene_change == true && is_changed == false) {
		FirstLevel();
		is_changed = true;
	}
	if (scene_change == false && is_changed == false) {
		SecondLevel();
		is_changed = true;
	}
	is_changed = false;

}

void j1Scene::RestartCurrentLevel() {

	if (current_map == "Map.tmx") {
		
		App->map->CleanUp();
		App->entity->CleanEntity();
		Map1Entities();
		
		if (current_map == "Map.tmx") {
			App->map->Load("Map.tmx");
		}
		else if (current_map == "map2.tmx") {
			App->map->Load("map2.tmx");
		}
		App->collider->Start();
	}

	else if (current_map == "map2.tmx") {
		
	
		App->map->CleanUp();
		App->entity->CleanEntity();
		Map2Entities();

		if (current_map == "Map.tmx") {
			App->map->Load("Map.tmx");
		}
		else if (current_map == "map2.tmx") {
			App->map->Load("map2.tmx");
		}
		App->collider->Start();
	}
	
}

void j1Scene::FirstLevel() {

	if (first_level == true) {

		App->map->CleanUp();
		current_map.create("Map.tmx");
		App->map->Load(current_map.GetString());
		App->collider->Start();
		App->entity->CleanEntity();
		CreateEntities();
		scene_change = false;
		first_level = false;

		int w, h;
		uchar* data = NULL;
		if (App->map->CreateWalkabilityMap(w, h, &data))
			App->pathfinding->SetMap(w, h, data);
		RELEASE_ARRAY(data);

		cont = 0;

	}

	else if (first_level == false) {

		if (scene_change == false) {

			scene_change_timer = SDL_GetTicks();
			App->fade->FadeInScene(2);
			scene_change = true;
			cont = 0;
		}

		if (SDL_GetTicks() - scene_change_timer > 1040) {

			App->map->CleanUp();
			current_map.create("Map.tmx");
			App->map->Load(current_map.GetString());
			App->entity->CleanEntity();
			App->collider->Start();
			CreateEntities();
			scene_change = false;
			scene_change_timer = false;
			cont = 0;
		}

		int w, h;
		uchar* data = NULL;
		if (App->map->CreateWalkabilityMap(w, h, &data))
			App->pathfinding->SetMap(w, h, data);
		RELEASE_ARRAY(data);
		cont = 0;
	}
}

void j1Scene::SecondLevel() {

	App->map->CleanUp();
	current_map.create("map2.tmx");
	App->map->Load(current_map.GetString());
	App->entity->CleanEntity();
	App->collider->Start();
	CreateEntities();
	scene_change = true;

	int w, h;
	uchar* data = NULL;
	if (App->map->CreateWalkabilityMap(w, h, &data))
		App->pathfinding->SetMap(w, h, data);
	RELEASE_ARRAY(data);
	cont = 0;
}

void j1Scene::CreateEntities() {

	if (current_map == "Map.tmx") {

		App->entity->DrawEntity(100, 500, j1Entity::entity_type::PLAYER);
		App->entity->DrawEntity(2550, 200, j1Entity::entity_type::GOLEM_GRASS_ENEMY);
		App->entity->DrawEntity(5250, 400, j1Entity::entity_type::GOLEM_GRASS_ENEMY);
		App->entity->DrawEntity(4000, 200, j1Entity::entity_type::BAT_ENEMY);
		App->entity->DrawEntity(700, 200, j1Entity::entity_type::BAT_ENEMY);
		App->entity->DrawEntity(200, 500, j1Entity::entity_type::HEART);
		App->entity->DrawEntity(200, 600, j1Entity::entity_type::COIN);


	}

	else if (current_map == "map2.tmx") {

		App->entity->DrawEntity(55, 100, j1Entity::entity_type::PLAYER);
		App->entity->DrawEntity(1500, 500, j1Entity::entity_type::GOLEM_ROCK_ENEMY);
		App->entity->DrawEntity(500, 100, j1Entity::entity_type::GOLEM_ROCK_ENEMY);
		App->entity->DrawEntity(700, 100, j1Entity::entity_type::BAT_ENEMY);
		App->entity->DrawEntity(2000, 1000, j1Entity::entity_type::BAT_ENEMY);
		
	}

}

void j1Scene::Map1Entities() {


	App->entity->CleanEntity();
	App->entity->DrawEntity(100, 500, j1Entity::entity_type::PLAYER);
	App->entity->DrawEntity(2550, 200, j1Entity::entity_type::GOLEM_GRASS_ENEMY);
	App->entity->DrawEntity(5250, 400, j1Entity::entity_type::GOLEM_GRASS_ENEMY);
	App->entity->DrawEntity(4000, 200, j1Entity::entity_type::BAT_ENEMY);
	App->entity->DrawEntity(700, 200, j1Entity::entity_type::BAT_ENEMY);
	App->entity->DrawEntity(200, 500, j1Entity::entity_type::HEART);
	App->entity->DrawEntity(200, 600, j1Entity::entity_type::COIN);
	App->collider->Start();
}

void j1Scene::Map2Entities() {
	

	App->entity->CleanEntity();
	App->entity->DrawEntity(55, 100, j1Entity::entity_type::PLAYER);
	App->entity->DrawEntity(1500, 500, j1Entity::entity_type::GOLEM_ROCK_ENEMY);
	App->entity->DrawEntity(500, 100, j1Entity::entity_type::GOLEM_ROCK_ENEMY);
	App->entity->DrawEntity(700, 100, j1Entity::entity_type::BAT_ENEMY);
	App->entity->DrawEntity(2000, 1000, j1Entity::entity_type::BAT_ENEMY);
	App->collider->Start();
}