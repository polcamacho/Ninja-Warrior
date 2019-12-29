#include "UI_Label.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Gui.h"
#include "j1Fonts.h"
#include "UI_element.h"
#include <string.h>

UI_Label::UI_Label(int x, int y, UI_Type type, char* text_input, UI_element * parent, j1Module * Observer, int* counter) : UI_element(x, y, type, parent, Observer)
{
	t = type;
	counter_path = counter;
}

UI_Label::~UI_Label()
{
}

bool UI_Label::Update(float dt)
{
	if (t == Label_timer) {
		char text[15] = "Time:         ";
		int aux_path = *counter_path;
		int divisor = 100000;
		for (int i = 7; i < 13; i++) {
			text[i] = (char)(aux_path / divisor + '0');
			aux_path = aux_path % divisor;
			divisor = divisor / 10;
		}
		SetTextTimer(text);
	}
	return true;
}

bool UI_Label::CleanUp()
{
	App->tex->UnLoad(texture);
	return true;
}

bool UI_Label::SetLabelText(const char* text_input)
{
	bool ret = false;

	text_texture = App->fonts->Print(text_input, { 255,255,255,255 }, App->fonts->fonts.start->data);
	
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

void UI_Label::SetTextTimer(const char* text)
{
	App->tex->UnLoad(texture);
	texture = App->fonts->Print(text, { 255,255,255,255 }, App->fonts->fonts.start->data);
	App->fonts->CalcSize(text, dimensions.w, dimensions.h);
}

bool UI_Label::Draw()
{
	if (texture != nullptr) {
		App->render->Blit_UI(texture, pos.x, pos.y, &dimensions, SDL_FLIP_NONE, 0.0f);
	}

	if (App->gui->debug_UI == true) {

		SDL_Rect lbl{ pos.x, pos.y, dimensions.w, dimensions.h };

		App->render->DrawQuad(lbl, 255, 0, 0, 255, false);
	}

	return true;
}
