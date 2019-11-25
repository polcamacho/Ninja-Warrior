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
#include "..//Brofiler/Brofiler.h"


j1EntityManager::j1EntityManager()
{
	name.create("entitymanager");
}

j1EntityManager::~j1EntityManager() {}

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
	Tex_Golems = App->tex->Load(PATH(folder.GetString(), texture2.GetString()));	//Load The Texture of player

	return true;
}

bool j1EntityManager::PreUpdate(float dt)
{
	BROFILER_CATEGORY("PreUpdate Entity Manager", Profiler::Color::Chartreuse);

		p2List_item<j1Entity*>* item = entities.start;
	while (item != nullptr)
	{
		
			delete item->data;
			entities.del(item);
		
		item = item->next;
	}
	return true;
}

bool j1EntityManager::Update(float dt)
{
	BROFILER_CATEGORY("PreUpdate Entity Manager", Profiler::Color::Coral);

		for (int i = 0; i < entities.count(); ++i)
		{
			if (entities.At(i) != nullptr)
			{
				entities.At(i)->data->Update(dt);
			}
		}
	for (int i = 0; i < entities.count(); ++i)
	{
		if (entities.At(i) != nullptr)
		{
			//entities.At(i)->data->Draw();
		}
	}
	return true;
}

bool j1EntityManager::CleanUp()
{
	p2List_item<j1Entity*>* item = entities.start;
	while (item != nullptr)
	{
		delete item->data;
		entities.del(item);
		item = item->next;

	}
	return true;
}

j1Entity* j1EntityManager::DrawEntity(int x, int y, entity_type type)
{
	j1Entity* e = nullptr;

	switch (type)
	{
	case entity_type::PLAYER:
		{
			e = new j1Player(x, y, type);
			entities.add(e);
			break;
		}

		/*case GOLEM_GRASS_ENEMY:
		{
			golem1 = new j1Golem1(x, y, GOLEM_GRASS_ENEMY);
			entities.add(golem1);
			ret = true;
			break;
		}

		case GOLEM_ROCK_ENEMY:
		{
			BigBat* bat = new BigBat(x, y, BIGBAT);
			entities.add(bat);
			ret = true;
			break;
		}

		case FLYING_EYE_ENEMY:
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
		}*/

		default:
		{
			break;
		}

	}

	return e;
}
