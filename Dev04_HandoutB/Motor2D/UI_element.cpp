#include "UI_element.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Gui.h"
#include "..//Brofiler/Brofiler.h"


UI_element::UI_element(int x, int y, UI_Type type, UI_element * parent, j1Module * Observer) : pos(x, y)
{
	texture = App->gui->GetAtlas();
	this->element_UI = parent;
	observer = Observer;

}

bool UI_element::Update(float dt)
{
	BROFILER_CATEGORY("Update UI_element", Profiler::Color::OrangeRed);
	return true;
}

bool UI_element::CleanUp()
{
	return true;
}

bool UI_element::Draw()
{
	if (texture != nullptr) {
		App->render->Blit(texture, pos.x, pos.y, &dimensions, SDL_FLIP_NONE);
	}
	return true;
}

bool UI_element::IsIntersection() {

	bool ret;

	App->input->GetMousePosition(mouse_pos.x, mouse_pos.y);

	if (mouse_pos.x > pos.x && mouse_pos.x<pos.x + dimensions.w && mouse_pos.y > pos.y && mouse_pos.y < pos.y + dimensions.h) {

		ret = true;
	}
	
	else {

		ret = false;

	}

	return ret;
}
