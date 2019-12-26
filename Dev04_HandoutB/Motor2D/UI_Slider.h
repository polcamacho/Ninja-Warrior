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

	UI_Slider(int x, int y, SDL_Rect left_button, SDL_Rect right_button, SDL_Rect scrollbar, SDL_Rect image_rect, iPoint slider_pos, UI_element* parent, j1Module* Observer);
	~UI_Slider();

	bool Draw();

	bool Update(float dt);

	bool SliderButtons();

	bool Button_Slider_Dragable();

public:

	iPoint new_mouse_pos;
	iPoint slider_position;

private:
	int min=0, max=10;
	SDL_Rect L_button;
	SDL_Rect R_button;
};

#endif //_!_UI_SLIDER_