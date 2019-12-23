#include "UI_Button.h"
#include "j1Gui.h"
#include "UI_element.h"
#include "j1Render.h"

UI_Button::UI_Button(int x, int y, SDL_Rect idle, SDL_Rect hover, SDL_Rect click, UI_element * parent, j1Module * Observer) : UI_element(x, y, parent, Observer)
{
	this->pos.x = x;
	this->pos.y = y;
	this->Button_idle = idle;
	this->Button_hover = hover;
	this->Button_click = click;

	dimensions = idle;

}

bool UI_Button::CleanUp()
{
	return true;
}

bool UI_Button::Draw()
{
	texture = App->gui->GetAtlas();

	if (texture != nullptr)
	{
		App->render->Blit(texture, pos.x, pos.y, &dimensions, SDL_FLIP_NONE, 1.0f);
	}

	return true;
}

bool UI_Button::Update(float dt)
{
	if (IsIntersection() == true) {
		dimensions = Button_hover;

		if (App->input->GetMouseButtonDown(1)) {
			dimensions = Button_click;
			if (observer) {
				observer->Callback(this);
			}
		}
	}

	else {
		dimensions = Button_idle;
	}

	return true;

}
