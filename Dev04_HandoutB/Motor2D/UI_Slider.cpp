#include "UI_Slider.h"
#include "p2Log.h"
#include "j1Audio.h"
#include "UI_Button.h"
#include "j1Gui.h"
#include "j1Render.h"

UI_Slider::UI_Slider(int x, int y, SDL_Rect scrollbar, SDL_Rect button, UI_element* parent, j1Module* Observer) : UI_element(x, y, parent, Observer)
{
	texture = App->gui->GetAtlas();
	Scrollbar = scrollbar;
	Button_Scrollbar = button;
	mouse_position_in_button = { -1,-1 };
	movement = x;
}

UI_Slider::~UI_Slider() {}

bool UI_Slider::Update(float dt)
{
	SetSliderLimitValues();

	if (IsIntersection() == true && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT) {

		if (mouse_position_in_button.x == -1) {
			App->input->GetMousePosition(new_mouse_pos.x, new_mouse_pos.y);
			mouse_position_in_button.x = new_mouse_pos.x;
		}

		Mouse_Is_Moving();
	}
		
	return true;
}

bool UI_Slider::SetSliderLimitValues()
{

	min = Scrollbar.x + 3;
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

	if (new_mouse_pos.x < mouse_position_in_button.x) {
		movement = mouse_position_in_button.x;
		return false;
	}

	/*if (posi.y > max_point) {
		current_point = max_point;
		return false;
	}*/
	movement = new_mouse_pos.x;

	//UI_Button* button;
	//button->dimensions = Button_Scrollbar;
	//bool ret = false;

	//	if (new_mouse_pos.x >= mouse_position_in_button.x) {

	//		//App->audio->Change_Volume(0.1, 1);
	//		movement = mouse_position_in_button.x - new_mouse_pos.x;
	//	}
	//	if (new_mouse_pos.x <= mouse_position_in_button.x) {

	//		//App->audio->Change_Volume(0.1, 0);
	//		movement = mouse_position_in_button.x - new_mouse_pos.x;
	//	}

	//	ret = true;
	//	LOG("%i %i", new_mouse_pos.x, mouse_position_in_button.x);

	//App->input->GetMousePosition(new_mouse_pos.x, new_mouse_pos.y);
	return true;
}

bool UI_Slider::Draw() {

	texture = App->gui->GetAtlas();

	//iPoint new_slider_pos=App->render->ScreenToWorld(slider_position.x, slider_position.y);

	if (texture != nullptr)
	{
		App->render->Blit(texture, pos.x, pos.y, &Scrollbar, SDL_FLIP_NONE, 1.0f);
		App->render->Blit(texture, pos.x, pos.y, &Button_Scrollbar, SDL_FLIP_NONE, 1.0f);
	}
	return true;

}