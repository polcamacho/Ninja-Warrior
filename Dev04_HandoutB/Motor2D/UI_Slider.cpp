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
	current_point = x;

	Button_Scrollbar = button;
	mouse_position_in_button = -1;
	dimensions.w = button.w + 150;
	dimensions.h = button.h + 150;
	dimensions.x = button.x + 150;
	dimensions.y = button.y + 150;
}

UI_Slider::~UI_Slider() {}

bool UI_Slider::Update(float dt)
{

	if (IsIntersection() == true && App->input->GetMouseButtonDown(1)) {

		if (mouse_position_in_button == -1) {
			App->input->GetMousePosition(new_mouse_pos.x, new_mouse_pos.y);
			mouse_position_in_button = new_mouse_pos.x;
		}

		Mouse_Is_Moving();
	}
	else {

		mouse_position_in_button = -1;
	
	}

	LOG("%f", current_point);
		
	return true;
}

bool UI_Slider::SetSliderLimitValues()
{

	/*min = Scrollbar.x + 3;
	max = Scrollbar.w - 3;

	/*UI_Button* left_button = new UI_Button(x,y, { 0,163,33,37 }, { 0,163,33,37 },{ 0,163,33,37 },&left_button,this);
	ui_element.add(left_button);
	if (IsIntersection() == true) {
		App->audio->Change_Volume(0.1, 0);
	}*/

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
		App->audio->Change_Volume(0.05, 0);
	}
	if (current_point > last_mouse_pos.x) {

		//current_point = last_mouse_pos.x;
		App->audio->Change_Volume(0.05, 1);
	}




}

bool UI_Slider::Draw() {

	texture = App->gui->GetAtlas();

	App->render->Blit(texture, pos.x, pos.y, &Scrollbar, SDL_FLIP_NONE, 1.0f);
	App->render->Blit(texture, current_point, pos.y-4, &Button_Scrollbar, SDL_FLIP_NONE, 1.0f);

	return true;

}