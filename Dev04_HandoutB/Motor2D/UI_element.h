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
	UI_element(int x, int y, UI_element* parent, j1Module* Observer);

	//Destructor
	virtual ~UI_element() {}

	// Called each loop iteration
	virtual bool Update(float dt);

	// Called each loop iteration
	virtual bool DrawUI();

	// Called before quitting
	virtual bool CleanUp();

	bool IsIntersection();


public:

	//UI elements position and dimensions
	iPoint pos;
	SDL_Rect dimensions;

	iPoint mouse_pos;
	//int type;


	SDL_Texture* atlas = nullptr;

	j1Module* Observer = nullptr;

	bool destroy = false;

	//pointer to an element
	UI_element*	element_UI;
};


#endif //_!_UI_ELEMENT