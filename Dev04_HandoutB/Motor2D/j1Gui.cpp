#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Gui.h"

j1Gui::j1Gui() : j1Module()
{
	name.create("gui");
}


// Destructor
j1Gui::~j1Gui()
{}

// Called before render is available
bool j1Gui::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	atlas_file_name = conf.child("atlas").attribute("file").as_string("");

	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	atlas = App->tex->Load(atlas_file_name.GetString());
	SDL_Rect rec;
	rec.x = 416;
	rec.y = 172;
	rec.w = 218;
	rec.h = 58;
	//AddElement(BUTTON, rec);

	return true;
}

// Update all guis
bool j1Gui::PreUpdate(float dt)
{
	return true;
}

bool j1Gui::Update(float dt) {

	return true;
}

// Called after all Updates
bool j1Gui::PostUpdate(float dt)
{
	
	return true;
}


// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");
	type_list.clear();

	return true;
}

// const getter for atlas
const SDL_Texture* j1Gui::GetAtlas() const { return atlas; }

// class Gui ---------------------------------------------------

void j1Gui::CreateButton(SDL_Rect& measures) {
	/*button* but = new button(measures);
	switch (type) {
	case NONE:
		break;
	case BUTTON:
		type_list.add(but);
		but->UpdateButton();
		break;
	case ST_IMG:

		break;
	case ST_TXT:

		break;
	case ET_TXT:

		break;
	}*/
}

//button::button(SDL_Rect& rect) : GUI(rect) {
//	type = BUTTON;
//};
//
//bool button::Pushed() {
//	iPoint posi;
//	int aux;
//	App->input->GetMousePosition(posi.x, posi.y);
//	if (posi.x >= pos.x && posi.x <= pos.x + dimensions.w && posi.y >= pos.y && posi.y <= pos.y + dimensions.h && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == 2) {
//		return true;
//	}
//	return false;
//}
//
//void button::UpdateButton() {
//	if (Pushed() == false) {
//		dimensions.x = 416;
//		App->render->Blit(App->gui->atlas, pos.x, pos.y, &dimensions);
//		LOG("GGGGGGGG");
//	}
//	else {
//		dimensions.x = 647;
//		App->render->Blit(App->gui->atlas, pos.x, pos.y, &dimensions);
//	}
//}