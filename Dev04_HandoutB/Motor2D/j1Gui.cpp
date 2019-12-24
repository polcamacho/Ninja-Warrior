#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "j1Image.h"
#include "UI_Slider.h"
#include "UI_element.h"
#include "UI_Button.h"

j1Gui::j1Gui() : j1Module()
{
	name.create("gui");
}

// Destructor
j1Gui::~j1Gui()
{}

// Called before render is available
bool j1Gui::Awake(pugi::xml_node& config)
{
	LOG("Loading GUI atlas");
	bool ret = true;
	node = config;
	folder.create(node.child("folder").child_value());
	UI_file_name = config.child("button").attribute("file1").as_string();

	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	texture = App->tex->Load(PATH(folder.GetString(),UI_file_name.GetString()));

	return true;
}

// Update all guis
bool j1Gui::PreUpdate(float dt)
{
	return true;
}

bool j1Gui::Update(float dt) {

	for (int i = 0; i < ui_element.count(); i++) {

		if (ui_element.At(i) != nullptr) {

			ui_element.At(i)->data->Draw();
			ui_element.At(i)->data->Update(dt);

		}
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
SDL_Texture* j1Gui::GetAtlas() const { return texture; }

// class Gui

UI_element* j1Gui::CreateButton(int x, int y, SDL_Rect idle, SDL_Rect hover, SDL_Rect click, UI_element* parent, j1Module* Observer) {
	
	UI_Button* button = new UI_Button(x, y, idle, hover, click, parent, Observer);

	ui_element.add(button);

	return button;
}

UI_element* j1Gui::CreateImage(int x, int y, SDL_Rect rect, UI_element* parent, j1Module* CallBack)
{
	j1Image* image = new j1Image(x, y, rect, parent, CallBack);

	ui_element.add(image);

	return image;
}

UI_element* j1Gui::CreateSlider(int x, int y, SDL_Rect rect, UI_element* parent, j1Module* CallBack)
{
	UI_Slider* slider = new UI_Slider(x, y, rect, parent, CallBack);

	ui_element.add(slider);

	return slider;
}
