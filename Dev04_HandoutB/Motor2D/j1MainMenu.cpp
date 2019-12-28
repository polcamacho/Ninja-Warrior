#include "j1App.h"
#include "j1MainMenu.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1FadeToBlack.h"
#include "j1Audio.h"
#include "j1Scene.h"
#include "j1Gui.h"
#include "UI_Button.h"
#include "UI_Slider.h"
#include "UI_Label.h"
#include "j1Image.h"
#include "p2Log.h"
#include "..//Brofiler/Brofiler.h"

j1MainMenu::j1MainMenu()
{
	name.create("gui");
}

j1MainMenu::~j1MainMenu()
{
}

bool j1MainMenu::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;
	node = conf;
	folder.create(node.child("folder").child_value());
	background_image = conf.child("background").attribute("file").as_string("");

	return ret;
}

bool j1MainMenu::Start()
{
	LOG("Loading background assets");
	bool ret = true;
	introTexture = App->tex->Load(PATH(folder.GetString(), background_image.GetString()));

	SDL_Rect rec;
	rec.x = 416;
	rec.y = 172;
	rec.w = 218;
	rec.h = 58;

	LOG("START MAIN MENU");

	App->audio->PlayMusic("audio/music/main_menu_music.ogg");

	App->render->camera.x = 0;
	App->render->camera.y = 0;

	
		

	LOG("MAIN MENU");


	return ret;
}

bool j1MainMenu::CleanUp()
{

	LOG("Unloading main menu");
	App->tex->UnLoad(graphics);
	App->tex->UnLoad(introTexture);
	App->gui->CleanUp();

	return true;
}

bool j1MainMenu::Update(float dt)
{
	BROFILER_CATEGORY("Update Main Menu", Profiler::Color::Pink);

	App->render->Blit(App->main_menu->introTexture, 0, 0);
	if (cont == 0) {

		if (is_menu == true) {
			App->gui->CleanUp();
			CreateMenu();
			cont++;
		}

		if (is_settings == true) {
			App->gui->CleanUp();
			CreateSettings();
			cont++;
		}

	}

	if (ret_m == false) {
		return false;
	}

	if (App->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN) {
		App->gui->Delete_Element(window);
	}
	
	return true;
}

bool j1MainMenu::PostUpdate(float dt)
{
	BROFILER_CATEGORY("Update Main Menu", Profiler::Color::HotPink);

	return true;
}

void j1MainMenu::CreateMenu() {
	
	
	LOG("MENU CREATED");
	App->gui->CreateImage(525, 225, Image, { 796, 9, 399, 488 }, NULL, this);
	App->gui->CreateButton(625, 350, Button_play, { 819,514,92,92 }, { 692,513,92,92 }, { 692, 395, 92, 92 }, NULL, this);
	App->gui->CreateButton(725, 350, Button_settings, { 935,512,96,98 }, { 576,513,96,97 }, { 572, 395, 97, 98 }, NULL, this);
	App->gui->CreateButton(625, 450, Button_info, { 1049,516,92,92 }, { 455,513,92,93 }, { 452, 397, 92, 92 }, NULL, this);
	App->gui->CreateButton(725, 450, Button_restart, { 0,60,92,92 }, { 115,58,91,91 }, { 228, 58, 91, 91 }, NULL, this);
	App->gui->CreateButton(775, 300, Button_close, { 0,0,44,44 }, { 69,0,44,44 }, { 141, 0, 44, 44 }, NULL, this);

	window = (j1Image*)App->gui->CreateImage(610, 225, Image, { 2, 278, 202, 58 }, NULL, this);

}

void j1MainMenu::CreateSettings() {

	LOG("SETTINGS CREATED");
	App->gui->CreateImage(525, 300, Image, { 2, 394, 381, 255 }, NULL, this);
	App->gui->CreateImage(610, 225, Image, { 2, 278, 202, 58 }, NULL, this);
	App->gui->CreateButton(845, 316, Button_close, { 0,0,44,44 }, { 69,0,44,44 }, { 141, 0, 44, 44 }, NULL, this);
	
	App->gui->CreateButton(560, 385, Button_slider_music_left, { 0,165,28,35 }, { 0,165,28,35 }, { 0,165,28,35 }, NULL, this);
	App->gui->CreateButton(830, 385, Button_slider_music_right, { 262,165,30,36 }, { 262,165,30,36 }, { 262,165,30,36 }, NULL, this);
	App->gui->CreateSlider(600, 390, Slider_music, { 38,169,214,24 }, { 125,221,34,36 }, 200, NULL, this);

	
	App->gui->CreateLabel(602, 340, Label, "Music Volume", NULL, this);

}
