#include "p2Log.h"
#include "j1Audio.h"
#include "j1Gui.h"
#include "UI_Button.h"
#include "UI_Slider.h"
#include "j1Render.h"

UI_Slider::UI_Slider(int x, int y, UI_Type type, SDL_Rect scrollbar, SDL_Rect button, float width, UI_element* parent, j1Module* Observer) : UI_element(x, y, type, parent, Observer)
{
	texture = App->gui->GetAtlas();
	
	initial_point = x;
	Scrollbar = scrollbar;
	max_point = initial_point + (Scrollbar.w+34);
	current_point = x + 89;
	
	Button_Scrollbar = button;
	mouse_position_in_button = -1;
	dimensions.w = button.w + 150;
	dimensions.h = button.h + 150;

	App->audio->Change_Volume_Music(get_value());


}

UI_Slider::~UI_Slider() {
}

bool UI_Slider::Update(float dt)
{

	if (IsIntersection() == true && App->input->GetMouseButtonDown(1)) {

		if (mouse_position_in_button == -1) {
			App->input->GetMousePosition(new_mouse_pos.x, new_mouse_pos.y);
			mouse_position_in_button = new_mouse_pos.x;
			
		}

		Mouse_Is_Moving();
		App->audio->Change_Volume_Music(get_value());
	}
	else {

		mouse_position_in_button = -1;
		
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