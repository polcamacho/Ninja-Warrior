#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "UI_element.h"
#include "UI_Button.h"

j1Gui::j1Gui() : j1Module()
{
	name.create("gui");
	UI_texture = nullptr;
}

// Destructor
j1Gui::~j1Gui()
{}

// Called before render is available
bool j1Gui::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	UI_file_name = conf.child("button").attribute("file1").as_string("");

	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	UI_texture = App->tex->Load(UI_file_name.GetString());
	SDL_Rect rec;
	rec.x = 416;
	rec.y = 172;
	rec.w = 218;
	rec.h = 58;

	return true;
}

// Update all guis
bool j1Gui::PreUpdate(float dt)
{
	return true;
}

bool j1Gui::Update(float dt) {

	p2List_item<UI_element*>* element = ui_element.start;

	while (element != nullptr) {
		element->data->Update(dt);
		element->data->Draw();
		element = element->next;
	}

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
	
	p2List_item<UI_element*>* element = ui_element.start;
	
	while (element != nullptr)
	{
		ui_element.del(element);
		delete element->data;
		element = element->next;
	} 

	return true;
}

// const getter for atlas
/*const SDL_Texture* j1Gui::GetAtlas() const { /*return atlas; }*/

// class Gui

UI_element* j1Gui::CreateButton(int x, int y, SDL_Rect idle, SDL_Rect hover, SDL_Rect click, UI_element* parent, j1Module* Observer) {
	
	UI_Button* button = new UI_Button(x, y, idle, hover, click, parent, Observer);

	ui_element.add(button);

	return button;
}
/*UI_element* CreateButton(int x, int y, SDL_Rect& dimensions, j1Module* Observer) {
	*button* but = new button(measures);
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
	}

}*/

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