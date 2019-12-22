#pragma once
#include "j1Module.h"
#include "p2List.h"

#ifndef __j1GUI_H__
#define __j1GUI_H__

#define CURSOR_WIDTH 2

struct SDL_Texture;
struct SDL_Rect;
class UI_element;

// TODO 1: Create your structure of classes


// ---------------------------------------------------
class j1Gui : public j1Module
{
public:

	j1Gui();

	// Destructor
	virtual ~j1Gui();

	// Called when before render is available
	bool Awake(pugi::xml_node&);

	// Call before first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate(float dt);

	bool Update(float dt);

	// Called after all Updates
	bool PostUpdate(float dt);

	// Called before quitting
	bool CleanUp();

	// TODO 2: Create the factory methods

	// Gui creation functions
	UI_element* CreateButton(int x, int y, SDL_Rect idle, SDL_Rect hover, SDL_Rect click, UI_element* parent, j1Module* Observer);
	/*void CreateImage(SDL_Rect& dimensions);
	void CreateText(SDL_Rect& dimensions);
	void CreateSlider(SDL_Rect& dimensions);*/

	SDL_Texture* GetTexture() const;

public:
	
	p2List<UI_element*> ui_element;
	j1Module* observer = nullptr;

private:

	SDL_Texture* texture;
	p2SString UI_file_name;
};

#endif // __j1GUI_H__