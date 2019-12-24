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

	UI_Slider(int x, int y, SDL_Rect image_rect, UI_element* parent, j1Module* CallBack);
	~UI_Slider();

	bool Draw();

	bool Dragable();

public:

	SDL_Rect slider_ball;

private:
	iPoint new_mouse_pos=mouse_pos;
};

#endif //_!_UI_SLIDER_