#pragma once
#ifndef __J1MAINMENU_H__
#define __J1MAINMENU_H__

#include "j1Module.h"
#include "p2Point.h"
struct SDL_Texture;
enum UI_Type;
class j1Image;

class j1MainMenu : public j1Module
{
public:
	j1MainMenu();
	~j1MainMenu();

	bool Start();
	bool Awake(pugi::xml_node&);
	bool Update(float dt);
	bool PostUpdate(float dt);
	bool CleanUp();
	
	void CreateMenu();
	void CreateSettings();
	
	bool ret_m = true;
	bool is_menu = true;
	bool is_settings = false;
	bool continue_lvl = false;
	int cont = 0;

public:

	SDL_Texture* graphics = nullptr;
	SDL_Texture* introTexture = nullptr;
	UI_Type t;


private:
	p2SString background_image;
	pugi::xml_node node;
	p2SString folder;
};

#endif // __J1MAINMENU_H__