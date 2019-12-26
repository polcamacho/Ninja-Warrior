#include "UI_Slider.h"
#include "p2Log.h"
#include "j1Audio.h"
#include "UI_Button.h"

UI_Slider::UI_Slider(int x, int y, SDL_Rect left_button, SDL_Rect right_button, SDL_Rect scrollbar, SDL_Rect button_slider, iPoint slider_pos, UI_element* parent, j1Module* CallBack) : UI_element(x, y, parent, CallBack)
{
	texture = App->gui->GetAtlas();
	dimensions = button_slider;
	App->input->GetMousePosition(mouse_pos.x, mouse_pos.y);
	L_button = left_button;
	R_button = right_button;
	slider_position = slider_pos;
	slider_position = pos;
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
	/*UI_Button* left_button = new UI_Button(left_button,);*/
	return true;
}

bool UI_Slider::Button_Slider_Dragable() {

	App->input->GetMousePosition(new_mouse_pos.x, new_mouse_pos.y);

	bool ret=false;

	if (IsIntersection() == true) {

		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT) {

			if (new_mouse_pos.x >= mouse_pos.x && new_mouse_pos.x <= 10) {

				App->audio->Change_Volume(0.1, 1);
				slider_position.x+=mouse_pos.x;
			}
			else if (new_mouse_pos.x <= mouse_pos.x && new_mouse_pos.x >= 0){

				App->audio->Change_Volume(0.1, 0);
				slider_position.x -= mouse_pos.x;
			}
			
			ret = true;
			LOG("%i %i", new_mouse_pos.x, new_mouse_pos.y);
			LOG("%i", slider_position.x);

		}

	}

	//App->input->GetMousePosition(mouse_pos.x, mouse_pos.y);
	
	return ret;
}

bool UI_Slider::Draw() {

	texture = App->gui->GetAtlas();

	if (texture != nullptr)
	{
		App->render->Blit(texture, pos.x, pos.y, &dimensions, SDL_FLIP_NONE, 1.0f);
	}
	return true;
}