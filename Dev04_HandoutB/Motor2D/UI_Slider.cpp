#include "UI_Slider.h"
#include "p2Log.h"
#include "j1Audio.h"
#include "UI_Button.h"
#include "j1Gui.h"
#include "j1Render.h"

UI_Slider::UI_Slider(int x, int y, SDL_Rect scrollbar, SDL_Rect button, float width, UI_element* parent, j1Module* Observer) : UI_element(x, y, parent, Observer)
{
	texture = App->gui->GetAtlas();
	
	initial_point = x;
	max_point = initial_point + width;
	current_point = x;

	Scrollbar = scrollbar;
	Button_Scrollbar = button;
	mouse_position_in_button = -1;
	dimensions = button;
}

UI_Slider::~UI_Slider() {}

bool UI_Slider::Update(float dt)
{

	if (IsIntersection() == true && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT) {

		if (mouse_position_in_button == -1) {
			iPoint posi;
			App->input->GetMousePosition(posi.x, posi.y);
			mouse_position_in_button = posi.x;
		}

		Mouse_Is_Moving();
	}
	else {

		mouse_position_in_button = -1;
	
	}
		
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

	iPoint posi;
	App->input->GetMousePosition(posi.x, posi.y);

	if (posi.x < initial_point) {
		current_point = initial_point;
		return false;
	}

	if (posi.x > max_point) {

		current_point = max_point;
		return false;
	}

	current_point = posi.x;

}

bool UI_Slider::Draw() {

	texture = App->gui->GetAtlas();

	App->render->Blit(texture, pos.x, pos.y, &Scrollbar, SDL_FLIP_NONE, 1.0f);
	App->render->Blit(texture, current_point, pos.y, &Button_Scrollbar, SDL_FLIP_NONE, 1.0f);

	return true;

}