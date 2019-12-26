#include "UI_Slider.h"
#include "p2Log.h"
#include "j1Audio.h"
#include "UI_Button.h"
#include "j1Gui.h"
#include "j1Render.h"

UI_Slider::UI_Slider(int x, int y, SDL_Rect left_button, SDL_Rect right_button, SDL_Rect scrollbar, SDL_Rect button_slider, iPoint slider_pos, UI_element* parent, j1Module* CallBack) : UI_element(x, y, parent, CallBack)
{
	texture = App->gui->GetAtlas();
	dimensions = button_slider;
	L_button = left_button;
	R_button = right_button;
	slider_position.x = slider_pos.x + pos.x;

	Scrollbar = scrollbar;

	movement = 0;

	App->input->GetMousePosition(mouse_pos.x, mouse_pos.y);
}

UI_Slider::~UI_Slider() {}

bool UI_Slider::Update(float dt)
{

	SliderButtons();
	Button_Slider_Dragable();
	return true;
}

bool UI_Slider::SliderButtons()
{
	/*UI_Button* left_button = new UI_Button(x,y, { 0,163,33,37 }, { 0,163,33,37 },{ 0,163,33,37 },&left_button,this);
	ui_element.add(left_button);
	if (IsIntersection() == true) {
		App->audio->Change_Volume(0.1, 0);
	}*/

	return true;
}

bool UI_Slider::Button_Slider_Dragable() {

	App->input->GetMousePosition(new_mouse_pos.x, new_mouse_pos.y);

	bool ret=false;

		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT) {

			if (IsIntersection() == true) {

				if (new_mouse_pos.x >= mouse_pos.x && new_mouse_pos.x <= 10) {

					App->audio->Change_Volume(0.1, 1);
					movement = new_mouse_pos.x - mouse_pos.x;
					slider_position.x += movement;
				}
				if (new_mouse_pos.x <= mouse_pos.x && new_mouse_pos.x >= 0) {

					App->audio->Change_Volume(0.1, 0);
					movement = new_mouse_pos.x - mouse_pos.x;
					slider_position.x += movement;
				}

				ret = true;
				LOG("%i %i", new_mouse_pos.x, new_mouse_pos.y);
				LOG("%i", slider_position.x);
			}
		}

	App->input->GetMousePosition(mouse_pos.x, mouse_pos.y);
	
	return ret;
}

bool UI_Slider::Draw() {

	texture = App->gui->GetAtlas();

	//iPoint new_slider_pos=App->render->ScreenToWorld(slider_position.x, slider_position.y);

	if (texture != nullptr)
	{
		App->render->Blit(texture, pos.x-20, pos.y-4, &L_button, SDL_FLIP_NONE, 1.0f);
		App->render->Blit(texture, pos.x+225, pos.y-7, &R_button, SDL_FLIP_NONE, 1.0f);
		App->render->Blit(texture, pos.x, pos.y, &Scrollbar, SDL_FLIP_NONE, 1.0f);
		App->render->Blit(texture, pos.x, pos.y, &dimensions, SDL_FLIP_NONE, 1.0f);
	}
	return true;

}