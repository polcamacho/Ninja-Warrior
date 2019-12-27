#include "p2Log.h"
#include "j1Audio.h"
#include "j1Gui.h"
#include "UI_Button.h"
#include "UI_Slider.h"
#include "j1Render.h"

UI_Slider::UI_Slider(int x, int y, UI_Type type, SDL_Rect scrollbar, SDL_Rect button, SDL_Rect left_button, float width, UI_element* parent, j1Module* Observer) : UI_element(x, y, type, parent, Observer)
{
	texture = App->gui->GetAtlas();
	
	initial_point = x;
	Scrollbar = scrollbar;
	max_point = initial_point + (Scrollbar.w+34);
	current_point = x + 89;

	Button_Scrollbar = button;
	L_Button = left_button;

	mouse_position_in_button = -1;
	dimensions.w = button.w + 150;
	dimensions.h = button.h + 150;

	App->audio->Change_Volume_Music(get_value());


}

UI_Slider::~UI_Slider() {
}

bool UI_Slider::Update(float dt)
{

	if (IsIntersection() == true && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT)==KEY_REPEAT) {

		if (mouse_position_in_button == -1) {
			App->input->GetMousePosition(new_mouse_pos.x, new_mouse_pos.y);
			mouse_position_in_button = new_mouse_pos.x;
			
		}

		Mouse_Is_Moving();
		App->audio->Change_Volume_Music(get_value());
	}
	else {

		mouse_position_in_button = -1;
		Slider_volume_buttons();
	}
	
	
	//LOG("%f", current_point);
		
	return true;
}

bool UI_Slider::Mouse_Is_Moving() {

	App->input->GetMousePosition(new_mouse_pos.x, new_mouse_pos.y);

	if (new_mouse_pos.x < initial_point) {
		current_point = initial_point;
		return false;
	}

	if (new_mouse_pos.x > max_point) {

		current_point = max_point;
		return false;
	}

	current_point = new_mouse_pos.x;
	App->input->GetMouseMotion(last_mouse_pos.x, last_mouse_pos.y);

	if (current_point < last_mouse_pos.x) {

		//current_point = last_mouse_pos.x;
		
	}
	if (current_point > last_mouse_pos.x) {

		//current_point = last_mouse_pos.x;
		App->audio->Change_Volume(get_value(), 1);
	}

}

bool UI_Slider::Draw() {

	texture = App->gui->GetAtlas();

	App->render->Blit(texture, pos.x, pos.y, &Scrollbar, SDL_FLIP_NONE, 1.0f);
	App->render->Blit(texture, current_point, pos.y-4, &Button_Scrollbar, SDL_FLIP_NONE, 1.0f);
	App->render->Blit(texture, pos.x-30, pos.y - 4, &L_Button, SDL_FLIP_NONE, 1.0f);

	return true;

}

float UI_Slider::get_value()
{
	int _100 = max_point - initial_point;
	float _1 = _100 / 100.0f;
	float current;

	current = current_point - initial_point;

	float ret = current / _1;

	//LOG("%f", ret);

	return ret;

}

void UI_Slider::Slider_volume_buttons()
{

	UI_Button* left = (UI_Button*)(pos.x, pos.y, Button_slider_music_left, L_Button, L_Button, L_Button, NULL, this);

	if (left->t==Button_slider_music_left && IsIntersection() == true && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN) {
		LOG("%s", left->t);
		App->audio->Change_Volume(0.5, 0);
	}

}

