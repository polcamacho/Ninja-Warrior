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

	t = type;

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
	//if cursor is inside button rectangle
	if (IsIntersection() == true) {
		
		counter++;

		//rect is button hover
		dimensions = Button_hover;
		LOG("%i", counter);

		//only plays fx once
		if (counter == 1) {

			App->audio->PlayFx(hover_fx, 0);
		}

		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT)==KEY_DOWN) {

			//rect is button pressed
			dimensions = Button_click;
			
			App->audio->PlayFx(click_fx, 0);

			if (t == Button_slider_music_left) {

				App->audio->Change_Volume(0.5, 0);

			}

			if (observer) {
				observer->Callback(this);
			}
		}
		
	}

	else {
		dimensions = Button_idle;
		counter = 0;
	}

	if (IsIntersection()==true && App->input->GetMouseButtonDown(1) && t == Button_slider_music_right) {

		App->audio->Change_Volume(0.5, 1);
	}

	return true;

}
