#pragma once
#include "j1Gui.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Input.h"

#ifndef __UI_ELEMENT__
#define __UI_ELEMENT__

struct SDL_Texture;

class UI_element 
{
public:

	UI_element();
	UI_element(int x, int y, UI_element* parent, j1Module* CallBack);

	//Destructor
	virtual ~UI_element() {}

	// Called each loop iteration
	virtual bool Update(float dt);

	// Called each loop iteration
	virtual bool Draw();

	// Called before quitting
	virtual bool CleanUp();


public:
	iPoint pos;
	SDL_Rect dimensions;
	int type;

	SDL_Texture* atlas = nullptr;
	UI_element*	element_UI;

	j1Module* callback = nullptr;
};


#endif //_!_UI_ELEMENT