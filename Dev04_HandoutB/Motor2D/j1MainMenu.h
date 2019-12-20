#pragma once
#ifndef __J1MAINMENU_H__
#define __J1MAINMENU_H__

#include "j1Module.h"
#include "p2Point.h"
struct SDL_Texture;


class j1MainMenu : public j1Module
{
public:
	j1MainMenu();
	~j1MainMenu();

	bool Start();
	bool Awake(pugi::xml_node&);
	bool Update(float dt);
	bool CleanUp();

public:

	SDL_Texture* graphics = nullptr;
	SDL_Texture* introTexture = nullptr;

private:
	p2SString background_image;

};

#endif // __J1MAINMENU_H__