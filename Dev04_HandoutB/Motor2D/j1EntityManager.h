#ifndef _j1ENTITYMANAGER_H_
#define _j1ENTITYMANAGER_H_

#include "j1Module.h"
#include "p2List.h"
#include "PugiXml/src/pugixml.hpp"
#include "p2Point.h"
#include "p2DynArray.h"
#include "j1Entity.h"
#include "p2List.h"
#define LIMIT_TIMER 50
#define TIMER_DEAD 10000000

struct SDL_Texture;
class j1Player;
enum entity_type;
class j1EntityManager :public j1Module {

public:
	j1EntityManager();
	~j1EntityManager();

	bool Awake(pugi::xml_node& config);
	bool Start();
	bool PreUpdate(float dt);
	bool PostUpdate(float dt);
	bool Update(float dt);
	bool CleanUp();
	bool CleanEntity();
	void DeleteEntity();


	// Called each loop iteration
	bool Save(pugi::xml_node&) const;
	bool Load(pugi::xml_node&);
	j1Entity* GetPlayer();
	j1Entity* DrawEntity(int x, int y, j1Entity::entity_type type);
	

public:

	p2List<j1Entity*>	entities;
	SDL_Texture*	Tex_Player = nullptr;
	SDL_Texture*	Tex_Golems_Grass = nullptr;
	SDL_Texture*	Tex_Golems_Rock = nullptr;
	SDL_Texture*	Tex_Bat = nullptr;
	SDL_Texture*	Tex_Coin = nullptr;
	SDL_Texture*	Tex_Heart = nullptr;

	//bool for coins collected, lives collected / removed, score and timer
	bool is_coin = false;
	bool is_live_plus = false;
	bool is_live_minus = false;
	bool is_score = false;
	bool is_timer = false;

	//counters for coins and lives
	int coins;
	int lives;
	int score = 0;
	int seconds = 0;
	int minutes = 0;

	int timer = 0;

	pugi::xml_node node;

	p2SString		folder;
	p2SString		texture1;
	p2SString		texture2;
	p2SString		texture3;
	p2SString		texture4;
	p2SString		texture5;
	p2SString		texture6;

};


#endif // !_j1ENTITYMANAGER_H_