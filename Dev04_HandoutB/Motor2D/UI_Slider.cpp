#include "UI_Slider.h"
#include "p2Log.h"

UI_Slider::UI_Slider(int x, int y, SDL_Rect ball, UI_element* parent, j1Module* CallBack) : UI_element(x, y, parent, CallBack)
{
	slider_ball = ball;

}

UI_Slider::~UI_Slider() {}

bool UI_Slider::Draw() {

	texture = App->gui->GetAtlas();

	if (texture != nullptr)
	{
		App->render->Blit(texture, pos.x, pos.y, &slider_ball, SDL_FLIP_NONE, 1.0f);
	}
	return true;
}

bool UI_Slider::Dragable() {

	bool ret=false;
	if (App->input->GetMouseButtonDown(1) && (new_mouse_pos.x == mouse_pos.x + 1 || new_mouse_pos.x == mouse_pos.x - 1)) {
		ret = true;
		LOG("%i %i %i", mouse_pos.x, mouse_pos.y);
	}
	return ret;
}