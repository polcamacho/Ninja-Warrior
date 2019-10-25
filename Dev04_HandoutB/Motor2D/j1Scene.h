#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"

struct SDL_Texture;

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
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	p2List<p2SString> maps;
	p2SString current_map;
	
	void SecondMap();
	bool Load(pugi::xml_node &node);
	bool Save(pugi::xml_node &node)const;

private:

public:
	
	int walk_FX;
	

};

#endif // __j1SCENE_H__