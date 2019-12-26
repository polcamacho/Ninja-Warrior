#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"
#include "p2List.h"
#include "p2Point.h"


#define CURSOR_WIDTH 2

struct SDL_Texture;
struct SDL_Rect;
class UI_element;
enum UI_Type;

// ---------------------------------------------------
class j1Gui : public j1Module
{
public:

	j1Gui();

	// Destructor
	~j1Gui();

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

	SDL_Texture* GetAtlas() const;

	// TODO 2: Create the factory methods

	// Gui creation functions
	UI_element* CreateButton(int x, int y, UI_Type type, SDL_Rect idle, SDL_Rect hover, SDL_Rect click, UI_element* parent, j1Module* Observer);
	UI_element* CreateImage(int x, int y, UI_Type type, SDL_Rect rect, UI_element* parent, j1Module* CallBack);
	UI_element* CreateSlider(int x, int y, UI_Type type, SDL_Rect scrollbar, SDL_Rect button, float width, UI_element* parent, j1Module* Observer);
	UI_element* CreateLabel(int x, int y, UI_Type type, char* text_input, UI_element* parent, j1Module* CallBack);


	/*void CreateImage(SDL_Rect& dimensions);
	void CreateText(SDL_Rect& dimensions);
	void CreateSlider(SDL_Rect& dimensions);*/


public:

	p2List<UI_element*> ui_element;

private:
	pugi::xml_node node;
	p2SString folder;
	SDL_Texture* texture;
	p2SString UI_file_name;

};

#endif // __j1GUI_H__