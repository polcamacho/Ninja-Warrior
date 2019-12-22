#pragma once
#include "UI_element.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Input.h"

#ifndef __UI_BUTTON__
#define __UI_BUTTON__

struct SDL_Texture;

class UI_Button : public UI_element
{
public:

	UI_Button();
	UI_Button(int x, int y, SDL_Rect idle, SDL_Rect hover, SDL_Rect click, UI_element* parent, j1Module* Observer);

	//Destructor
	~UI_Button() {}

	// Called each loop iteration
	bool Update(float dt);

	// Called each loop iteration
	bool Draw();

	// Called before quitting
	bool CleanUp();


public:

	//Rects for button

	//idle when is not intersecting
	SDL_Rect Button_idle;

	//hover when is intersecting
	SDL_Rect Button_hover;

	//click when is clicking
	SDL_Rect Button_click;


};


#endif //_!_UI_BUTTON