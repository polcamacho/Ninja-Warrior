#include "UI_element.h"

UI_element::UI_element()
{

}

UI_element::UI_element(int x, int y, UI_element * parent, j1Module * Observer)
{

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
