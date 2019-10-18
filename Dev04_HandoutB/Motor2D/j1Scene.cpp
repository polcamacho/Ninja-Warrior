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

#include <string>

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}


//texture = App->tex->Load("maps/tmw_desert_spacing.png");
// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");

	

	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	pugi::xml_document	config_file;
	pugi::xml_node* node = &App->LoadConfig(config_file);
	node = &node->child("map").child("scene");
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
	if(App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		App->LoadGame();

	if(App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		App->SaveGame();

	if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y += 1;

	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y -= 1;

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x += 1;

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x -= 1;

	App->map->Draw();
	
	//Draw Player
	App->player->DrawPlayer();

	int m_x; int m_y;
	App->input->GetMousePosition(m_x, m_y);
	iPoint p = App->map->WorldToMap(m_x, m_y);

	p2SString title("NINJA TREE - Map:%dx%d Tiles:%dx%d Tilesets:%d || Coords in Map: X->%i Y->%i || Coords in World: X->%i Y->%i",
		App->map->data.width, App->map->data.height,
		App->map->data.tile_width, App->map->data.tile_height,
		App->map->data.tilesets.count(), p.x, p.y, m_x, m_y);

	return true;
}

void j1Scene::putPlayerToSpawn()
{
	//App->player->GetPosition()->position.x = App->map->spawn.x;
	//App->player->GetPosition()->position.y = App->map->spawn.y;

	App->map->spawn.x = 100;
	App->map->spawn.x = 638;
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
