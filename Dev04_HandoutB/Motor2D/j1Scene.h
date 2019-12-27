#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"

struct SDL_Texture;
class j1Entity;
enum UI_Type;

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate(float dt);

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate(float dt);

	// Called before quitting
	bool CleanUp();

	//Debug
	void SecondMap();
	void FirstLevel();
	void SecondLevel();
	void RestartCurrentLevel();
	void CreateEntities();
	void Map1Entities();
	void Map2Entities();

	bool Load(pugi::xml_node &node);
	bool Save(pugi::xml_node &node)const;

	bool scene_change;
	bool is_changed;
	bool first_level;

	int scene_timer;
	bool scene_change_timer;

	p2SString current_map;

	int cont = 0;
	int cont2 = 0;
	bool ret_s = true;
	bool is_pause = false;

private:

	p2List<p2SString> map_list;

public:
	
	SDL_Texture* debug_tex;
	int jump_FX;
	int death_FX;
	j1Entity* coin;
	

};

#endif // __j1SCENE_H__