#include "UI_Label.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Gui.h"
#include "j1Fonts.h"

UI_Label::UI_Label(int x, int y, UI_element * parent, j1Module * Observer) : UI_element(x, y, parent, Observer)
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

	if (texture != nullptr)
	{
		App->tex->UnLoad(texture);
	}

	tex = App->fonts->Print(text_input, { 255,255,255,255 }, App->fonts->fonts.start->data);
	texture = tex;
	/*if (tex != nullptr)
	{
		uint w = 0, h = 0;
		App->tex->GetSize(tex, w, h);
		SDL_Rect rect = debug_UI = { 0, 0, (int)w, (int)h };

		texture = tex;

		this->text = text;
		this->rect = rect;

		ret = true;
	}*/
	return ret;
}

bool UI_Label::Draw()
{
	if (texture != nullptr) {
		App->render->Blit(texture, pos.x, pos.y, &dimensions, SDL_FLIP_NONE, 1.0f);
	}
	return true;
}
