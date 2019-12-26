#ifndef __UI_LABEL_H__
#define __UI_LABEL_H__

#include "UI_element.h"
#include "p2SString.h"
#include "p2List.h"
#include "SDL\include\SDL.h"

class UI_Label : public UI_element
{
public:

	UI_Label(int x, int y, UI_Type type, char* text_input, UI_element * parent, j1Module * Observer);
	~UI_Label();

	bool Update(float dt);
	bool CleanUp();

	bool SetLabelText(const char* text_input);

	bool Draw();

public:

	p2SString text;
	SDL_Texture* text_texture=nullptr;
};

#endif //_!_UI_LABEL__