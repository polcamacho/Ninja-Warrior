#include "UI_Label.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Gui.h"
#include "j1Fonts.h"
#include "UI_element.h"
#include <string.h>

UI_Label::UI_Label(int x, int y, UI_Type type, char* text_input, UI_element * parent, j1Module * Observer) : UI_element(x, y, type, parent, Observer)
{

}

UI_Label::~UI_Label()
{
}

bool UI_Label::Update(float dt)
{
	return true;
}

bool UI_Label::CleanUp()
{
	App->tex->UnLoad(texture);
	return true;
}

bool UI_Label::SetLabelText(const char * text_input)
{
	bool ret = false;

	text_texture = App->fonts->Print(text_input, { 255,255,255,0 }, App->fonts->fonts.start->data);
	
	if (text_texture != nullptr)
	{
		uint w = 0, h = 0;

		App->tex->GetSize(text_texture, w, h);

		SDL_Rect rect = {0,0,(int)w,(int)h};

		texture = text_texture;

		this->text = text_input;
		this->dimensions = rect;

		ret = true;
	}

	return ret;
}

void UI_Label::SetTextFromNum(int value)
{
	std::string s = std::to_string(value);

	text = s.c_str();

	App->tex->UnLoad(text_texture);

	text_texture = App->fonts->Print(text.GetString(), { 255,255,255,0 }, App->fonts->default);

	int width = 0, height = 0;
	App->fonts->CalcSize(this->text.GetString(), width, height, App->fonts->default);
	dimensions.w = width;
	dimensions.h = height;
}

bool UI_Label::Draw()
{
	if (texture != nullptr) {
		App->render->Blit_UI(texture, pos.x, pos.y, &dimensions, SDL_FLIP_NONE, 0.0f);
	}

	if (App->gui->debug_UI == true) {

		SDL_Rect lbl{ pos.x, pos.y, dimensions.w, dimensions.h };

		App->render->DrawQuad(lbl, 255, 0, 0, 255, false, 0.0f);
	}

	return true;
}
