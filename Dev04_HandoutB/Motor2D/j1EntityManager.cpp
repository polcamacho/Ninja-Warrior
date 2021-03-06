#include "p2defs.h"
#include "p2log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Audio.h"
#include "j1Textures.h"
#include "j1Player.h"
#include "j1Golem1.h"
#include "j1Golem2.h"
#include "j1Bat.h"
#include "j1Entity.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Heart.h"
#include "j1Coin.h"
#include "j1EntityManager.h"
#include "p2Log.h"
#include "j1Scene.h"
#include "p2Animation.h"
#include "j1Collider.h"
#include "..//Brofiler/Brofiler.h"
#include "p2List.h"
#include "j1Input.h"
#include "j1Map.h"


j1EntityManager::j1EntityManager()
{
	name.create("entitymanager");
}				

j1EntityManager::~j1EntityManager() {
}

bool j1EntityManager::Awake(pugi::xml_node& config)
{
	
	node = config;
	folder.create(node.child("folder").child_value());
	texture1 = node.child("texture").attribute("source1").as_string();
	texture2 = node.child("texture").attribute("source2").as_string();
	texture3 = node.child("texture").attribute("source3").as_string();
	texture4 = node.child("texture").attribute("source4").as_string();

	return true;
}

bool j1EntityManager::Start()
{
	Tex_Player = App->tex->Load(PATH(folder.GetString(), texture1.GetString()));	//Load The Texture of player
	Tex_Golems_Grass = App->tex->Load(PATH(folder.GetString(), texture2.GetString()));	//Load The Texture of golems grass
	Tex_Bat = App->tex->Load(PATH(folder.GetString(), texture3.GetString()));	//Load The Texture of bat
	Tex_Golems_Rock = App->tex->Load(PATH(folder.GetString(), texture2.GetString()));	//Load The Texture of golems rocks
	Tex_Coin = App->tex->Load(PATH(folder.GetString(), texture4.GetString()));	//Load The Texture of coin
	Tex_Heart = App->tex->Load(PATH(folder.GetString(), texture4.GetString()));	//Load The Texture of heart

	return true;
}


bool j1EntityManager::PreUpdate(float dt)
{
	BROFILER_CATEGORY("PreUpdate Entity Manager", Profiler::Color::Chocolate);
	p2List_item<j1Entity*>* item = entities.start;
	while (item!=nullptr) {
		item->data->PreUpdate(dt);
		item = item->next;
	}
	return true;
}

bool j1EntityManager::Update(float dt)
{
	BROFILER_CATEGORY("Update Entity Manager", Profiler::Color::Brown);
	p2List_item<j1Entity*>* item = entities.start;

	while (item != nullptr)
	{
		item->data->Update(dt);
		item = item->next;
	}

	DeleteEntity();

	return true;
}


bool j1EntityManager::PostUpdate(float dt) {

	BROFILER_CATEGORY("PostUpdate Entity Manager", Profiler::Color::SandyBrown);
	p2List_item<j1Entity*>* item = entities.start;
	while (item!=nullptr) {
		item->data->PostUpdate(dt);
		item = item->next;
	}
	return true;
}

bool j1EntityManager::CleanUp()
{
	App->tex->UnLoad(App->entity->Tex_Player);
	App->tex->UnLoad(App->entity->Tex_Golems_Grass);
	App->tex->UnLoad(App->entity->Tex_Golems_Rock);
	App->tex->UnLoad(App->entity->Tex_Bat);
	App->tex->UnLoad(App->entity->Tex_Heart);
	App->tex->UnLoad(App->entity->Tex_Coin);
	return true;
}

bool j1EntityManager::CleanEntity() {
	
	p2List_item<j1Entity*>* item = entities.start;
	while (item != nullptr)
	{
		delete item->data;
		entities.del(item);
		item = item->next;
	}
	return true;
}

j1Entity* j1EntityManager::DrawEntity(int x, int y, j1Entity::entity_type type)
{
	j1Entity* ret = nullptr;

	switch (type)
	{
		
		case j1Entity::entity_type::PLAYER:
		{
			ret = new j1Player(x, y);
			if (ret != nullptr) {
				entities.add(ret);
				entities.end->data->Awake(node);
				entities.end->data->Start();
			}
			break;
		}

		case j1Entity::entity_type::GOLEM_GRASS_ENEMY:
		{
			ret = new j1Golem1(x, y);
			if (ret != nullptr) {
				entities.add(ret);
				entities.end->data->Awake(node);
				entities.end->data->Start();
			}
			break;
		}
		
		case j1Entity::entity_type::GOLEM_ROCK_ENEMY:
		{
			ret = new j1Golem2(x, y);
			if (ret != nullptr) {
				entities.add(ret);
				entities.end->data->Awake(node);
				entities.end->data->Start();
			}
			break;
		}

		case j1Entity::entity_type::BAT_ENEMY:
		{
			ret = new j1Bat(x, y);
			if (ret != nullptr) {
				entities.add(ret);
				entities.end->data->Awake(node);
				entities.end->data->Start();
			}
			break;
		}

		case j1Entity::entity_type::COIN:
		{
			ret = new j1Coin(x, y);
			if (ret != nullptr) {
				entities.add(ret);
				entities.end->data->Awake(node);
				entities.end->data->Start();
			}
			break;
		}

		case j1Entity::entity_type::HEART:
		{
			ret = new j1Heart(x, y);
			if (ret != nullptr) {
				entities.add(ret);
				entities.end->data->Awake(node);
				entities.end->data->Start();
			}
			break;
		}

	}

	return ret;
}

bool j1EntityManager::Load(pugi::xml_node& data) {

	p2List_item<j1Entity*>* item = entities.start;
	while (item != nullptr) {

		item->data->Load(data);
		item = item->next;

	}
	return true;
}

bool j1EntityManager::Save(pugi::xml_node& data) const {

	p2List_item<j1Entity*>* item = entities.start;
	while (item!=nullptr) {
		
			item->data->Save(data);
			item = item->next;

	}
	return true;
}

void j1EntityManager::DeleteEntity() {
	
	p2List_item<j1Entity*>* e = entities.start;
	
	while (e!=nullptr) {
		
		if (e->data->destroy==true) {

			entities.del(e);
		}
			e = e->next;
	}
}

j1Entity* j1EntityManager::GetPlayer() {

	p2List_item<j1Entity*>* entities_list = entities.start;
	while (entities_list != nullptr) {

		if (entities_list->data->type == j1Entity::entity_type::PLAYER) {
			return entities_list->data;
		}
		entities_list = entities_list->next;
	}

}


