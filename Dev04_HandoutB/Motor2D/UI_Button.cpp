#include "p2Log.h"
#include "UI_Button.h"
#include "j1Gui.h"
#include "UI_element.h"
#include "j1Render.h"
#include "j1Audio.h"

UI_Button::UI_Button(int x, int y, UI_Type type, SDL_Rect idle, SDL_Rect hover, SDL_Rect click, UI_element * parent, j1Module * Observer) : UI_element(x, y, type, parent, Observer)
{
	this->pos.x = x;
	this->pos.y = y;
	this->Button_idle = idle;
	this->Button_hover = hover;
	this->Button_click = click;

	dimensions = idle;
	click_fx = App->audio->LoadFx("audio/fx/Button_click.wav");
	hover_fx = App->audio->LoadFx("audio/fx/Button_hover.wav");
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
	counter = 0;

	if (IsIntersection() == true) {

		counter++;
		dimensions = Button_hover;
		
		if (counter == 1) {

			App->audio->PlayFx(hover_fx, 0);
		}

		if (App->input->GetMouseButtonDown(1)) {

			dimensions = Button_click;
			
			LOG("%i", counter);
			if (counter == 1) {
				App->audio->PlayFx(click_fx, 0);
			}

			if (observer) {
				observer->Callback(this);
			}
		}
		counter = 0;
	}

	else {
		dimensions = Button_idle;
	}


	return true;

}
