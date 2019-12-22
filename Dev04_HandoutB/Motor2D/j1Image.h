#ifndef __j1IMAGE_H__
#define __j1IMAGE_H__

#include "UI_element.h"
#include "j1Textures.h"
#include "SDL/include/SDL.h"
#include "p2SString.h"

struct SDL_Texture;


class j1Image : public UI_element
{
public:

	j1Image(int x, int y, SDL_Rect image_rect, UI_element* parent, j1Module* CallBack);
	~j1Image();

	bool Draw();


public:
};

#endif