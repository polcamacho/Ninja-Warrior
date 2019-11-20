#include "p2defs.h"
#include "p2log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Audio.h"
#include "j1Textures.h"
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

bool j1EntityManager::Awake(pugi::xml_node& conf)
{
	bool ret = true;
	return ret;
}

bool j1EntityManager::Start()
{
	return true;
}

bool j1EntityManager::PreUpdate(float dt)
{
	BROFILER_CATEGORY("PreUpdate Entity Manager", Profiler::Color::Chartreuse);

		p2List_item<j1Entity*>* item = entities.start;
	while (item != nullptr)
	{
		/*if (item->data->to_destroy)
		{
			delete item->data;
			entities.del(item);
		}*/
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

/*bool j1EntityManager::DrawEntity(int x, int y, entity_type type)
{
	bool ret = false;

	switch (type)
	{
	case PLAYER:
	{
		player = new Player(x, y, PLAYER);
		entities.add(player);
		ret = true;
		break;
	}

	case BLACKBANDIT:
	{
		BlackBandit* bandit = new BlackBandit(x, y, BLACKBANDIT);
		entities.add(bandit);
		ret = true;
		break;
	}

	case BIGBAT:
	{
		BigBat* bat = new BigBat(x, y, BIGBAT);
		entities.add(bat);
		ret = true;
		break;
	}

	case COIN:
	{
		Coin* coin = new Coin(x, y, COIN);
		entities.add(coin);
		ret = true;
		break;
	}


	default:
	{
		break;
	}

	}

	return ret;
}*/
