#ifndef __UI_SLIDER__
#define __UI_SLIDER__

#include "UI_element.h"
#include "j1Textures.h"
#include "SDL/include/SDL.h"
#include "p2SString.h"

struct SDL_Texture;


class UI_Slider : public UI_element
{
public:

	UI_Slider(int x, int y, UI_Type type, SDL_Rect scrollbar, SDL_Rect button, UI_element* parent, j1Module* Observer);
	~UI_Slider();

	bool Draw();

	bool Update(float dt);

	bool SetSliderLimitValues();

	bool Mouse_Is_Moving();

public:

	iPoint new_mouse_pos;
	iPoint mouse_position_in_button;
	iPoint Slider_pos;
	int min, max;
	int movement;

private:
	
	SDL_Rect Button_Scrollbar;
	SDL_Rect Scrollbar;

};

#endif //_!_UI_SLIDER_