#include "p2Log.h"
#include "UI_Button.h"
#include "j1Gui.h"
#include "UI_element.h"
#include "j1Render.h"
#include "j1Audio.h"
#include "j1FadeToBlack.h"
#include "j1MainMenu.h"
#include "j1Scene.h"

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

	if (App->gui->debug_UI == true) {

		SDL_Rect b{ pos.x, pos.y, Button_click.w, Button_click.h };

		App->render->DrawQuad(b, 255, 0, 0, 255, false);
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
		//LOG("%i", counter);

		//only plays fx once
		if (counter == 1) {

			App->audio->PlayFx(hover_fx, 0);
		}

		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT)==KEY_DOWN) {

			//rect is button pressed
			dimensions = Button_click;
			
			//only plays fx once
			App->audio->PlayFx(click_fx, 0);

			if (observer) {
				observer->Callback(this);
			}

			if (t == Button_slider_music_left) {
				App->gui->volume_up = 1;
			}

			else if (t == Button_slider_music_right) {
				App->gui->volume_up = 2;
			}

			if (t == Button_play) {
				App->fade->FadeToBlack(App->main_menu, App->scene, 2);
			}

			if (t == Button_close) {
				LOG("HOLA");
				
				//App->scene->ret_s = false;
				if (App->main_menu->is_menu == true) {
					App->main_menu->ret_m = false;
					App->main_menu->is_settings = false;
				}
				if (App->main_menu->is_settings == true) {
					
					LOG("HOLAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
					App->main_menu->is_settings = false;
					App->main_menu->is_menu = true;
					App->main_menu->cont = 0;

				}
				
			}

			if (t == Button_info) {
				ShellExecuteA(NULL, "open", "https://polcamacho.github.io/Ninja-Warrior", NULL, NULL, SW_SHOWNORMAL);
			}

			if (t == Button_restart) {
				
				if (App->main_menu->is_menu == true) {
					App->fade->FadeToBlack(App->main_menu, App->scene, 2);
					App->main_menu->continue_lvl = true;
				}

				if (App->scene->is_pause) {

				}

			}

			if (t == Button_settings) {

				App->main_menu->is_menu = false;
				App->main_menu->is_settings = true;
				App->main_menu->cont = 0;

			}

			//LOG("%i", App->gui->volume_up);

		}
		else {
			
			App->gui->volume_up = 0;

		}

	}

	else {
		dimensions = Button_idle;
		counter = 0;
	}

	return true;

}
