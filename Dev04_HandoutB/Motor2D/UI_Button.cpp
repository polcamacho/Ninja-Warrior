#include "UI_Button.h"

UI_Button::UI_Button()
{
	
}

UI_Button::UI_Button(int x, int y, SDL_Rect idle, SDL_Rect hover, SDL_Rect click, UI_element * parent, j1Module * Observer)
{
	pos.x = x;
	pos.x = x;
	Button_idle = idle;
	Button_hover = hover;
	Button_click = click;
}

bool UI_Button::Update(float dt)
{

	if (IsIntersection() == true) {
		dimensions = Button_hover;

		if (App->input->GetMouseButtonDown(1)) {
			dimensions =Button_click;
		}
	}

	else {
		dimensions = Button_idle;
	}
	return true;
}

bool UI_Button::Draw()
{
	if (texture != nullptr)
	{
		App->render->Blit(texture, pos.x, pos.y, &dimensions, SDL_FLIP_NONE, 0.0f);
	}
	return true;
}

bool UI_Button::CleanUp()
{
	return true;
}
