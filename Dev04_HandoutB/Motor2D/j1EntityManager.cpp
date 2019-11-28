#include "p2defs.h"
#include "p2log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Audio.h"
#include "j1Textures.h"
#include "j1Player.h"
#include "j1Entity.h"
#include "j1Window.h"
#include "j1Map.h"
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
	folder.create(config.child("folder").child_value());
	texture1 = config.child("texture").attribute("source1").as_string();
	texture2 = config.child("texture").attribute("source2").as_string();

	bool ret = true;
	return ret;
}

bool j1EntityManager::Start()
{
	Tex_Player = App->tex->Load(PATH(folder.GetString(), texture1.GetString()));	//Load The Texture of player
	Tex_Golems = App->tex->Load(PATH(folder.GetString(), texture2.GetString()));	//Load The Texture of golems

	return true;
}

bool j1EntityManager::Update(float dt)
{
	BROFILER_CATEGORY("PreUpdate Entity Manager", Profiler::Color::Coral);
	p2List_item<j1Entity*>* item = entities.start;

	while (item != nullptr)
	{
		item->data->Update(dt);
		item = item->next;
	}

	DeleteEntity();
	return true;
}

bool j1EntityManager::PreUpdate(float dt)
{
	return true;
}

bool j1EntityManager::PostUpdate(float dt) {
	return true;
}

bool j1EntityManager::CleanUp()
{
	App->tex->UnLoad(App->entity->Tex_Player);
	App->tex->UnLoad(App->entity->Tex_Golems);

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
			}
			break;
		}

		/*case GOLEM_GRASS_ENEMY:
		{
			ret = new j1Golem1(x, y);
			entities.add(ret);
			break;
		}

		case GOLEM_ROCK_ENEMY:
		{
			ret = new Golem2(x, y);
			entities.add(ret);
			break;
		}*/

		/*case FLYING_EYE_ENEMY:
		{
			Coin* coin = new Coin(x, y, COIN);
			entities.add(coin);
			ret = true;
			break;
		}

		case BAT_ENEMY:
		{
			Coin* coin = new Coin(x, y, COIN);
			entities.add(coin);
			ret = true;
			break;
		}
		*/

		default:
		{
			break;
		}

	}

	return ret;
}

void j1EntityManager::DeleteEntity() {

	p2List_item<j1Entity*>* item = entities.start;
	while (item != nullptr) {
		if (item->data->destroy == true) {
			delete item->data;
			entities.del(item);
		}
		item = item->next;
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


