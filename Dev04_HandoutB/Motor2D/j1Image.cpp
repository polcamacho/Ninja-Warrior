#include "j1Image.h"

j1Image::j1Image(int x, int y, UI_Type type, SDL_Rect image_rect, UI_element* parent, j1Module* Observer) : UI_element(x, y, type, parent, Observer)
{
	dimensions = image_rect;
}

j1Image::~j1Image() {}

bool j1Image::Draw() {
	
	texture = App->gui->GetAtlas();

	if (texture != nullptr)
	{
		App->render->Blit(texture, pos.x, pos.y, &dimensions, SDL_FLIP_NONE, 1.0f);
	}

	if (App->gui->debug_UI == true) {

		SDL_Rect img{ pos.x, pos.y, dimensions.w, dimensions.h };

		App->render->DrawQuad(img, 255, 0, 0, 255, false);
	}

	return true;
}