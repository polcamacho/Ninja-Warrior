#include "UI_Slider.h"
#include "p2Log.h"

UI_Slider::UI_Slider(int x, int y, SDL_Rect slider, UI_element* parent, j1Module* CallBack) : UI_element(x, y, parent, CallBack)
{
	slider_ball = slider;
	
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

/*bool UI_Slider::Dragable() {

	/*new_mouse_pos = App->input->GetMousePosition(mouse_pos);
	new_mouse_pos.y = App->input->GetMousePosition(mouse_pos.y);

	bool ret=false;

	if (IsIntersection() == true) {

		/*if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT && new_mouse_pos) {
			slider_ball.x+=
			ret = true;
			LOG("%i %i %i", mouse_pos.x, mouse_pos.y);
		}

	}
	
	return ret;
}*/