#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"

struct SDL_Texture;
class j1Entity;
enum UI_Type;
class j1Image;
class UI_Slider;
class UI_Button;
class UI_Label;

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

	//Create the menu settings from pause menu
	void CreateSettings();

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
	int time = 0;
	bool coins_earned = true;
	bool lives_earned = true;
	p2SString lives_char;

private:

	p2List<p2SString> map_list;

public:
	
	SDL_Texture* debug_tex;
	int jump_FX;
	int death_FX;

	j1Image* window;
	j1Image* title_win;
	UI_Button* slider_left_volume;
	UI_Button* slider_right_volume;
	UI_Button* slider_left_fx;
	UI_Button* slider_right_fx;
	UI_Button* menu;
	UI_Button* close_game;
	UI_Slider* slider_fx;
	UI_Slider* slider_volume;
	UI_Button* close;
	UI_Label* title;
	UI_Label* music_volume;
	UI_Label* fx_volume;
	UI_Label* Player_lives;
	UI_Label* Player_coins;

};

#endif // __j1SCENE_H__