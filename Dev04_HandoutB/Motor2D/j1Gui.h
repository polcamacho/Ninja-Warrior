#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"

#define CURSOR_WIDTH 2

// TODO 1: Create your structure of classes

enum Type_GUI {
	NONE,
	BUTTON,
	ST_IMG,
	ST_TXT,
	ET_TXT,
};

struct GUI {
	GUI(SDL_Rect& rect);
	virtual ~GUI() {};
	iPoint pos;
	SDL_Rect dimensions;
	int type;
};

struct button : GUI {
	button(SDL_Rect& rect);
	bool Pushed();
	void UpdateButton();
	
};

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
	void AddElement(Type_GUI type, SDL_Rect& dimensions);
	const SDL_Texture* GetAtlas() const;
	SDL_Texture* atlas = nullptr;
private:


	p2SString atlas_file_name;
	p2List<GUI*> type_list;
};

#endif // __j1GUI_H__