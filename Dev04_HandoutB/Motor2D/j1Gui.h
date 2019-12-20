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

//enum UI_type {
//	NONE,
//	BUTTON,
//	ST_IMG,
//	ST_TXT,
//	ET_TXT,
//};

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
	UI_element* CreateButton(int x, int y, SDL_Rect& dimensions, j1Module* Observer);
	/*void CreateImage(SDL_Rect& dimensions);
	void CreateText(SDL_Rect& dimensions);
	void CreateSlider(SDL_Rect& dimensions);*/

	const SDL_Texture* GetAtlas() const;

public:

	UI_element* element;

private:

	SDL_Texture* atlas = nullptr;
	p2SString atlas_file_name;
	p2List<j1Gui*> type_list;
};

#endif // __j1GUI_H__