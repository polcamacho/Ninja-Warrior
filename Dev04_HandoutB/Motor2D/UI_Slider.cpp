#include "p2Log.h"
#include "j1Audio.h"
#include "j1Gui.h"
#include "UI_Button.h"
#include "UI_Slider.h"
#include "j1Render.h"

UI_Slider::UI_Slider(int x, int y, UI_Type type, SDL_Rect scrollbar, SDL_Rect button, SDL_Rect left_button, float width, UI_element* parent, j1Module* Observer) : UI_element(x, y, type, parent, Observer)
{
	texture = App->gui->GetAtlas();
	
	initial = x;
	Scrollbar = scrollbar;

	max = initial + (Scrollbar.w+34);
	actual_pos = x + 89;
	
	Button_Scrollbar = button;
	L_Button = left_button;

	mouse_position_in_button = -1;
	dimensions.w = button.w + 150;
	dimensions.h = button.h + 150;

	App->audio->Change_Volume_Music(get_valors());

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
		App->audio->Change_Volume_Music(get_valors());
	}
	else {

		mouse_position_in_button = -1;

		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN) {
			
			if (actual_pos > 100) {
				
				App->audio->Change_Volume(0.3, 0);
				actual_pos -= 10;
			}
			
		}

		else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN) {
			
			if (actual_pos < 278) {
				
				actual_pos += 10;
				App->audio->Change_Volume(0.3, 1);

			}

		}

	}
	
	
	
	
	//LOG("%f", current_point);
		
	return true;
}

bool UI_Slider::Mouse_Is_Moving() {

	App->input->GetMousePosition(new_mouse_pos.x, new_mouse_pos.y);

	if (new_mouse_pos.x < actual_pos) {
		actual_pos = initial;
		return false;
	}

	if (new_mouse_pos.x > max) {

		actual_pos = max;
		return false;
	}

	actual_pos = new_mouse_pos.x;
	App->input->GetMouseMotion(last_mouse_pos.x, last_mouse_pos.y);


}

bool UI_Slider::Draw() {

	texture = App->gui->GetAtlas();

	App->render->Blit(texture, pos.x, pos.y, &Scrollbar, SDL_FLIP_NONE, 1.0f);
	App->render->Blit(texture, actual_pos, pos.y-4, &Button_Scrollbar, SDL_FLIP_NONE, 1.0f);
	App->render->Blit(texture, pos.x-30, pos.y - 4, &L_Button, SDL_FLIP_NONE, 1.0f);
	return true;

}

float UI_Slider::get_valors()
{
	int max_valor = max - initial;
	float min_valor = max_valor / 100.0f;
	float current;

	current = actual_pos - initial;

	float ret = current / min_valor;

	//LOG("%f", ret);

	return ret;

}


