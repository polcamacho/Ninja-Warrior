#include "UI_element.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Gui.h"

UI_element::UI_element()
{

}

UI_element::UI_element(int x, int y, UI_element * parent, j1Module * Observer)
{
	texture = nullptr;
	observer = nullptr;
	destroy = false;

	observer = Observer;
	this->element_UI = parent;
}

bool UI_element::Update(float dt)
{
	return false;
}

bool UI_element::CleanUp()
{
	return false;
}

bool UI_element::DrawUI()
{
	if (texture != nullptr) {
		App->render->Blit(texture, pos.x, pos.y, &dimensions);
	}
	return false;
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
