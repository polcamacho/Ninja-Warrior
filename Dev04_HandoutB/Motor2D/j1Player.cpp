#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Audio.h"
#include "j1Textures.h"
#include "j1Player.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Input.h"
#include "j1Collider.h"
#include "j1Scene.h"
#include <math.h>

j1Player::j1Player() : j1Module()
{
	name.create("player");
	
}

// Destructor
j1Player::~j1Player()
{}

bool j1Player::Awake(pugi::xml_node& config) {

	bool ret = true;

	//Load All Player Features from Config

	folder.create(config.child("folder").child_value());	
	texture = config.child("texture").attribute("source").as_string();

	data_player.position.x = config.child("position").attribute("x").as_int();
	data_player.position.y = config.child("position").attribute("y").as_int();

	data_player.jumpvel = config.child("jump_velocity").attribute("jumpvel").as_int();
	   
	data_player.v.x = config.child("velocity").attribute("x").as_int();
	
	data_player.colOffset.x = config.child("colOffset").attribute("x").as_int();
	data_player.colOffset.y = config.child("colOffset").attribute("y").as_int();
	
	data_player.gravity = config.child("gravity").attribute("grav").as_float();

	
	return ret;

}

void j1Player::DrawPlayer()
{
	

	if (data_player.player_flip) {
		App->render->Blit(data_player.Tex_Player, data_player.position.x, data_player.position.y, &(data_player.current_animation->GetCurrentFrame()), SDL_FLIP_HORIZONTAL, 0);	//Draw Player Flipped
	}
	else {
		App->render->Blit(data_player.Tex_Player, data_player.position.x, data_player.position.y, &(data_player.current_animation->GetCurrentFrame()), SDL_FLIP_NONE, 0);	//Draw Player Normal
	}


}

bool j1Player::Start() {

	pretimer = 0;
	//globaltime = SDL_GetTicks();	//Sets the Global time to the death timer
	
	Pushbacks();	//Call all the Pushback of animations
	
	data_player.Tex_Player = App->tex->Load(PATH(folder.GetString(), texture.GetString()));	//Load The Texture of player
	
	data_player.colliders = App->collider->AddCollider({ data_player.position.x, data_player.position.y, 39,53 }, COLLIDER_PLAYER, this);	//Sets The Collider Type and Dimensions to Player
	
	return	true;

}

bool j1Player::PreUpdate() {

	return true;

}

bool j1Player::Update(float dt) {

	
	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) {

		if (godmode == false)	//If godmode is false sets None Collider to player for he can fly around map and not collide
		{

			data_player.gravity = 0;	//Sets new gravity to player for he can move around map
			godmode = true;	

		}
		else if (godmode == true)
		{

			data_player.gravity = 20;	//Sets normal gravity to player 
			godmode = false;
			
		}
	}
	
	if (godmode == true) {	//Sets the Debug keys to player move when he cant die

		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
			data_player.position.y -= 20;

		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
			data_player.position.y += 20;

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
			data_player.position.x -= 20;

		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
			data_player.position.x += 20;
	}
	
	data_player.position.y += data_player.gravity;
	data_player.preposition = data_player.position;
	
	CheckState();	//Checks the state where is the player
	State();	//Set the animation relationed with the state that he is
	
	//Player Collider Draw
	data_player.colliders->SetPos(data_player.position.x, data_player.position.y);	//Sets the Player Collider Position


	//Player Draw
	if (data_player.player_flip) {
		App->render->Blit(data_player.Tex_Player, data_player.position.x, data_player.position.y, &(data_player.current_animation->GetCurrentFrame()), SDL_FLIP_HORIZONTAL, 1.0);	//Draw Player Flipped
	}
	else {
		App->render->Blit(data_player.Tex_Player, data_player.position.x, data_player.position.y, &(data_player.current_animation->GetCurrentFrame()), SDL_FLIP_NONE, 1.0);	//Draw Player Normal
	}

	if (data_player.grounded == false) {	//Sets that if the player is not touching the ground puts the fall animation

		current_state = JUMP_FALL;

	}
		
	return true;

}

bool j1Player::PostUpdate(float dt) {

	return true;
}

// Called before quitting
bool j1Player::CleanUp()
{
	LOG("Unloading player");

	App->tex->UnLoad(data_player.Tex_Player);	//Unload The Player texture
	App->collider->CleanUp();	//Unload the Player collider
	
	return true;
}

bool j1Player::Load(pugi::xml_node& node) {

	if (!node.child("position").empty())	//Load the valors if save_confign file is empty link
	{

		data_player.position.x = node.child("position").attribute("x").as_int();	//Load Player X
		data_player.position.y = node.child("position").attribute("y").as_int();	//Load Player Y
	}

	else {	
		data_player.position.x = data_player.position.x;	//Load Player X
		data_player.position.y = data_player.position.y;	//Load Player Y
	}
	return true;

}
bool j1Player::Save(pugi::xml_node& node) const {

	if (node.child("position").empty())	//Save The Position Valors in the save_config file
	{
		pugi::xml_node&  save = node.append_child("position");	
		save.append_attribute("x").set_value(data_player.position.x);	//Save Player X
		save.append_attribute("y").set_value(data_player.position.y);	//Save Player Y
	}
	else
	{
		node.child("position").attribute("x").set_value(data_player.position.x);	//Save Player X
		node.child("position").attribute("y").set_value(data_player.position.y);	//Save Player Y
	}
	
	return true;

}

void j1Player::Pushbacks() {

	//Sets the Idle Animation
	data_player.idle.PushBack({ 7,554,48,52 }, 0.1, 0, 0);
	data_player.idle.PushBack({ 84,554,41,52 }, 0.1, 10, 0);
	data_player.idle.PushBack({ 156,555,42,50 }, 0.2, 10, 0);
	data_player.idle.PushBack({ 232,557,41,48 }, 0.2, 10, 0);
	data_player.idle.PushBack({ 291,557,41,48 }, 0.2, 10, 0);
	data_player.idle.PushBack({ 351,557,40,48 }, 0.2, 10, 0);
	data_player.idle.PushBack({ 415,556,41,49 }, 0.2, 10, 0);
	data_player.idle.PushBack({ 474,556,42,49 }, 0.2, 10, 0);
	data_player.idle.PushBack({ 533,557,41,48 }, 0.2, 10, 0);
	data_player.idle.PushBack({ 591,554,41,51 }, 0.1, 0, 0);
	data_player.idle.PushBack({ 643,554,52,52 }, 0.1, -1, 0);
	data_player.idle.PushBack({ 705,555,44,50 }, 0.1, -6, 0);
	data_player.idle.PushBack({ 763,554,43,52 }, 0.2, -20, 0);
	data_player.idle.PushBack({ 819,554,45,52 }, 0.2, -20, 0);
	data_player.idle.PushBack({ 880,554,43,51 }, 0.2, -20, 0);
	data_player.idle.PushBack({ 938,555,43,51 }, 0.2, -20, 0);
	data_player.idle.PushBack({ 995,554,44,52 }, 0.2, -20, 0);
	data_player.idle.loop = true;

	//Sets the Walk Animation
	data_player.walk.PushBack({ 7,4,42,49 }, 0.2, 0, 0);
	data_player.walk.PushBack({ 65,4,41,49 }, 0.2, 0, 0);
	data_player.walk.PushBack({ 131,3,38,50 }, 0.2, 0, 0);
	data_player.walk.PushBack({ 201,3,36,51 }, 0.2, 0, 0);
	data_player.walk.PushBack({ 259,5,38,48 }, 0.2, 0, 0);
	data_player.walk.PushBack({ 317,4,44,50 }, 0.2, 0, 0);
	data_player.walk.loop = true;

	//Sets the Run Animation
	data_player.walk2.PushBack({ 7,4,42,49 }, 0.4, 0, 0);
	data_player.walk2.PushBack({ 65,4,41,49 }, 0.4, 0, 0);
	data_player.walk2.PushBack({ 131,3,38,50 }, 0.4, 0, 0);
	data_player.walk2.PushBack({ 201,3,36,51 }, 0.4, 0, 0);
	data_player.walk2.PushBack({ 259,5,38,48 }, 0.4, 0, 0);
	data_player.walk2.PushBack({ 317,4,44,50 }, 0.4, 0, 0);
	data_player.walk2.loop = true;

	//Sets the Jump Animation
	data_player.jump.PushBack({ 7,128,52,43 }, 0.6, 0, 0);
	data_player.jump.PushBack({ 81,114,47,57 }, 0.3, 0, 0);
	data_player.jump.PushBack({ 155,113,42,58 }, 0.3, 0, 0);
	data_player.jump.PushBack({ 221,118,52,53 }, 0.2, 0, 0);
	data_player.jump.PushBack({ 285,116,53,55 }, 0.15, 0, 0);
	data_player.jump.PushBack({ 348,119,56,52 }, 0.05, 0, 0);
	data_player.jump.loop = false;

	//Sets the Fall Animation
	data_player.fall.PushBack({ 348,119,56,52 }, 0.05, 0, 0);
	data_player.fall.PushBack({ 423,129,53,42 }, 0.5, 0, 0);
	data_player.fall.loop = true;

	//Sets the Attack Animation
	data_player.idle_attack.PushBack({ 7,759,58,53 }, 0.1, 0, 0);
	data_player.idle_attack.PushBack({ 70,758,60,54 }, 0.1, 0, 0);
	data_player.idle_attack.PushBack({ 130,763,61,50 }, 0.1, 0, 0);
	data_player.idle_attack.PushBack({ 201,763,59,51 }, 0.1, 0, 0);
	data_player.idle_attack.PushBack({ 270,758,56,54 }, 0.1, 0, 0);
	data_player.idle_attack.PushBack({ 333,757,39,54 }, 0.1, 0, 0);
	data_player.idle_attack.PushBack({ 397,759,38,53 }, 0.1, 0, 0);
	data_player.idle_attack.loop = false;

	//Sets the Death Animation
	data_player.death.PushBack({ 8,464,48,55 }, 0.2, 0, 0);
	data_player.death.PushBack({ 72,456,62,63 }, 0.2, 0, 0);
	data_player.death.PushBack({ 150,454,69,65 }, 0.2, 0, 0);
	data_player.death.PushBack({ 257,464,82,56 }, 0.2, 0, 0);
	data_player.death.PushBack({ 367,447,83,72 }, 0.2, 0, 0);
	data_player.death.PushBack({ 488,447,84,73 }, 0.2, 0, 0);
	data_player.death.PushBack({ 609,457,94,62 }, 0.2, 0, 0);
	data_player.death.PushBack({ 734,455,113,65 }, 0.2, 0, 0);
	data_player.death.PushBack({ 894,445,127,74 }, 0.2, 0, 0);
	data_player.death.PushBack({ 1054,433,116,87 }, 0.2, 0, 0);
	data_player.death.loop = false;

}

void j1Player::CheckState()
{
	
	if(godmode==false){
		
		data_player.velrun = (data_player.v.x)+0.25;
		
		//if "D" is pressed animation walk forward 
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && data_player.canjump == true) {		

			current_state = WALK;
			data_player.position.x += data_player.v.x;
			data_player.player_flip = false;

			//if  "SPACE" is pressed when "D" is pressed, the player jumps forward
			if (App->input->GetKey(SDL_SCANCODE_SPACE)==KEY_DOWN) {		

				current_state = JUMP_WALK;
				App->audio->PlayFx(App->scene->jump_FX);

				
					data_player.jumpenergy = data_player.jumpvel;
				
				//decrease the jump counter
				data_player.jumpCounter--;					

				//when the character do the double jump, reset jump animation
				if (data_player.jumpCounter == 0) {			
					data_player.jump.Reset();
				}

			}

			//if  "LSHIFT" is pressed when "D" is pressed, the player runs forward
			if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) {		
				
				current_state = RUN;
				data_player.position.x += data_player.velrun;
				data_player.player_flip = false;

				//if "SPACE" is pressed when "LSHIFT" is pressed, and when "D" is pressed, the player jumps running forward
				if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {		
					
					data_player.jumpCounter = 2;					//reset counter. If we dont do it, character cannot do the double jump while running

					data_player.right = true;						//player can jump forward
					data_player.left = false;
					App->audio->PlayFx(App->scene->jump_FX);


					current_state = JUMP_RUN;

					//decrease the jump counter
					data_player.jumpCounter--;				

					//when the character do the double jump, reset jump animation
					if (data_player.jumpCounter == 0) {
						data_player.jump.Reset();
					}
				}

			}
		}

		//if "A" is pressed animation walk backward actives flips to the Blit
		else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && data_player.canjump == true) {		

			current_state = WALK;
			data_player.position.x -= data_player.v.x;
			data_player.player_flip = true;

			
			//if "SPACE" is pressed when "A" is pressed, the player jumps backward
			if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {		

				current_state = JUMP_WALK;

				data_player.jumpenergy = data_player.jumpvel;
				
				data_player.jumpCounter--;
				App->audio->PlayFx(App->scene->jump_FX);

				if (data_player.jumpCounter == 0) {
					data_player.jump.Reset();
				}
			}

			//if  "LSHIFT" is pressed when "A" is pressed, the player runs backward
			if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) {
				
				current_state = RUN;
				data_player.position.x -= data_player.velrun;
			
				data_player.player_flip = true;

				//if "SPACE" is pressed when "LSHIFT" is pressed, and when "A" is pressed, the player jumps running backward
				if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {		 

					data_player.jumpCounter = 2;			//reset counter. If we dont do it, character cannot do the double jump while running
					data_player.left = true;				//player can jump backward
					data_player.right = false;

					current_state = JUMP_RUN;

					data_player.jumpCounter--;
					App->audio->PlayFx(App->scene->jump_FX);

					if (data_player.jumpCounter == 0) {
						data_player.jump.Reset();
					}
				}
			}
		}

		//if "SPACE" is pressed the player jumps
		else if ((App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) && (data_player.jumpCounter!=0)) {		//counter needs to be !=0, if not cannot do double jump
			
				current_state = JUMP_UP;
				data_player.player_flip = false;
				data_player.jumpenergy = data_player.jumpvel;
				data_player.jumpCounter--;
				App->audio->PlayFx(App->scene->jump_FX);

				
				if(data_player.jumpCounter==0){
					data_player.jump.Reset();
				}

		}


		if ((App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN) && data_player.canjump == true || (data_player.TimeAttack == false)) {
						
			current_state = IDLE_ATTACK;
			
		}

		
		//If any key pressed animation idle
		else if(data_player.canjump==true && App->input->GetKey(SDL_SCANCODE_SPACE) == NULL && App->input->GetKey(SDL_SCANCODE_A) == NULL && App->input->GetKey(SDL_SCANCODE_D) == NULL && App->input->GetKey(SDL_SCANCODE_P)==NULL){	
			
			current_state = IDLE;
			data_player.player_flip = false;

		}
	}

}

void j1Player::State() {

	if (current_state == IDLE) {
		
		data_player.current_animation = &data_player.idle;				
		data_player.jump.Reset();
		data_player.fall.Reset();
		data_player.idle_attack.Reset();
	}

	if (current_state == WALK) {

		data_player.current_animation = &data_player.walk;		
		data_player.jump.Reset();
		data_player.fall.Reset();
		data_player.idle_attack.Reset();
	}
	if(current_state==JUMP_WALK){

		current_state = JUMP_UP;

	}

	if (current_state == JUMP_RUN) {
		
		//If left = true, jump running backward
		if (data_player.left == true) {
			
			data_player.position.x -= data_player.velrun;
		}

		//If right = true, jump running forward
		else if (data_player.right == true) {
			
			data_player.position.x += data_player.velrun;
		}

		data_player.canjump = false;
		data_player.injump = true;
		data_player.current_animation = &data_player.jump;

		if (data_player.jumpenergy <= data_player.gravity) {								//character will jump up until it do not accomplish this condition 
			
			data_player.jumpenergy += 0.5;													// jump up increments 0.5 each time
			data_player.position.y = data_player.position.y + data_player.jumpenergy;		// y position increments 0.5 each time

		}

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			data_player.position.x -= data_player.v.x;
			data_player.player_flip = true;
		}
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			data_player.position.x += data_player.v.x;
			data_player.player_flip = false;
		}

	}
		
	if (current_state == RUN) {
		data_player.current_animation = &data_player.walk2;		
		data_player.jump.Reset();
		data_player.fall.Reset();
		data_player.idle_attack.Reset();
	}

	if (current_state == JUMP_UP) {

		data_player.canjump = false;
		data_player.injump = true;
		data_player.current_animation = &data_player.jump;

		if (data_player.jumpenergy <= data_player.gravity) {									//character will jump up until it do not accomplish this condition 
			
			data_player.jumpenergy += 0.5;														// jump up increments 0.5 each time
			data_player.position.y = data_player.position.y + data_player.jumpenergy;			// y position increments 0.5 each time

		}

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			data_player.position.x -= data_player.v.x;
			data_player.player_flip = true;
		}
		
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			data_player.position.x += data_player.v.x;
			data_player.player_flip = false;
		}
		
	}

	if (current_state == JUMP_FALL) {

		data_player.current_animation = &data_player.fall;
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			data_player.position.x -= data_player.v.x;
			data_player.player_flip = true;
		}
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			data_player.position.x += data_player.v.x;
			data_player.player_flip = false;
		}
	}

	if (current_state == IDLE_ATTACK) {

		data_player.current_animation = &data_player.idle_attack;
		//LOG("%i", data_player.canjump);
		data_player.TimeAttack = true;
		if (data_player.current_animation->AnimFinished() == true) {
			
			data_player.TimeAttack = false;
			
		}
		

	}

	if (current_state == DEATH) {	
		
		die = true;	//Sets the die to true
		LOG("GLOBAL: %d", globaltime);
		LOG("PRE: %d", pretimer);
		if (die == true) {

			if (App->scene->current_map == "Map.tmx") {	//If player is in map 1
				
				

				if (pretime (20)) {	//Do a timer to stop the game during the Death animation
					

					
					//Sets the Position that player goes when he dies
					data_player.position.x = 100;	//Set Player X	
					data_player.position.y = 300;	//Set Player Y
					current_state = JUMP_FALL;	//Sets the Animation when he reapears
					data_player.death.Reset();
				}

			}

			else {	//If player is not in map 1 is in map 2

				

				if (pretime (20)) {	//Do a timer to stop the game during the Death Animation
					data_player.current_animation = &data_player.death;	//Current Animation is Death
					App->audio->PlayFx(App->scene->death_FX);	//Sets the Death Audio
					//Sets the Position that player goes when he dies
					data_player.position.x = 55;	//Set Player X	
					data_player.position.y = 10;	//Set Player Y
					current_state = JUMP_FALL;	//Sets the Animation when he reapears
					data_player.death.Reset();

				}

			}

					
		}
		die = false;

	}
	
}

void j1Player::OnCollision(Collider* c1, Collider* c2) {	//Check if the Player collides with something

	if(godmode==false){

		if (c1->type == ColliderType::COLLIDER_PLAYER && c2->type == ColliderType::COLLIDER_FLOOR) {	//If player collide with floor

			if (data_player.preposition.y < c2->rect.y || data_player.position.y == c2->rect.y - data_player.colliders->rect.h) {	//Checks that player collider from above

				data_player.position.y = c2->rect.y - data_player.colliders->rect.h;
				data_player.grounded = true;	//Sets that player is touching the floor
				data_player.canjump = true;		//Sets tha player can jump

				if (data_player.injump == true) {
					data_player.jump.Reset();
				}
				if (data_player.grounded == true) {
					data_player.jumpCounter = 2;
				}

			}

			else if (data_player.preposition.y > (c2->rect.y + c2->rect.h)) {	//Checks that player collider from below
			
				data_player.position.y = c2->rect.y + c2->rect.h;
				current_state = JUMP_FALL;	//Sets the animation 

			}
		
			else if ((data_player.position.x < c2->rect.x + c2->rect.w && data_player.position.x > c2->rect.x) ||(data_player.position.x + data_player.colliders->rect.w < c2->rect.x + c2->rect.w && data_player.position.x + data_player.colliders->rect.w > c2->rect.x)) {	//Checks that player collider from sides
			
				if ((data_player.position.x + data_player.colliders->rect.w) < (c2->rect.x + c2->rect.w)) { //Checks that player collides from left
				
					data_player.position.x = c2->rect.x - data_player.colliders->rect.w;

				}
				else if (data_player.position.x < (c2->rect.x + c2->rect.w)) {	//Checks that player collides from right
				
					data_player.position.x = c2->rect.x + c2->rect.w;

				}
			}

		}

		if (c1->type == ColliderType::COLLIDER_PLAYER && c2->type == ColliderType::COLLIDER_PLATFORM) {		//Checks that player collides with platform

			if (data_player.preposition.y < c2->rect.y || data_player.position.y == c2->rect.y - data_player.colliders->rect.h) {	//Checks that player collider from above

				data_player.position.y = c2->rect.y - data_player.colliders->rect.h;
				data_player.grounded = true;	//Sets that player is touching the floor
				data_player.canjump = true;		//Sets tha player can jump

				if (data_player.grounded == true) {
				
					data_player.jumpCounter = 2;

				}

			}
			else if ((data_player.position.y >= data_player.preposition.y) && (data_player.preposition.y + data_player.colliders->rect.h) < c2->rect.y) {	//Checks that player collider from below
			
				data_player.position.y = c2->rect.y - data_player.colliders->rect.h;

			}

		}

		if (c1->type == ColliderType::COLLIDER_PLAYER && c2->type == ColliderType::COLLIDER_DEAD) {		//Checks that player collides with something that he can die

			//pretime = SDL_GetTicks();	//Sets the pretime to death timer
		
			if (data_player.preposition.y < c2->rect.y || data_player.position.y == c2->rect.y - data_player.colliders->rect.h) {	//Checks that player collider from above
			
				data_player.current_animation = &data_player.death;	//Current Animation is Death
				App->audio->PlayFx(App->scene->death_FX);	//Sets the Death Audio
				data_player.position.y = c2->rect.y - data_player.colliders->rect.h;
				current_state = DEATH;	//Sets player to Death state
			
				data_player.grounded = true;	//Sets that player is touching the floor
				data_player.canjump = false;	//Sets tha player can jump
				//die = true;	//Sets die bool to true for timer start

			}

			else if (data_player.preposition.y > (c2->rect.y + c2->rect.h)) {	//Checks that player collider from below
			
				data_player.current_animation = &data_player.death;	//Current Animation is Death
				App->audio->PlayFx(App->scene->death_FX);	//Sets the Death Audio
				//pretime = SDL_GetTicks();	//Sets the pretime to death timer

				data_player.position.y = c2->rect.y + c2->rect.h;
				current_state = DEATH;	//Sets player to Death state

				data_player.grounded = true;	//Sets that player is touching the floor
				data_player.canjump = false;	//Sets tha player can jump
				//die = true;	//Sets die bool to true for timer start

			}

		}

		if (c1->type == ColliderType::COLLIDER_PLAYER && c2->type == ColliderType::COLLIDER_NEXT) {

			if (data_player.preposition.y < c2->rect.y || data_player.position.y == c2->rect.y - data_player.colliders->rect.h) {	//Checks that player collider from above	
				App->scene->SecondMap();	//Pass to next map
			}
		
			else if (data_player.preposition.y > (c2->rect.y + c2->rect.h)) {	//Checks that player collider from below
				App->scene->SecondMap();	//Pass to next map
			}
		
			else if ((data_player.position.x < c2->rect.x + c2->rect.w && data_player.position.x > c2->rect.x) || (data_player.position.x + data_player.colliders->rect.w < c2->rect.x + c2->rect.w && data_player.position.x + data_player.colliders->rect.w > c2->rect.x)) {	//Checks that player collider from sides
			
				if ((data_player.position.x + data_player.colliders->rect.w) < (c2->rect.x + c2->rect.w)) {		//Checks that player collides from left
					App->scene->SecondMap();	//Pass to next map
				}
				else if (data_player.position.x < (c2->rect.x + c2->rect.w)) {	//Checks that player collides from right
					App->scene->SecondMap();	//Pass to next map
				}
			}
		}

	}
	else {

	}

}

void j1Player::Reset() {	//Reset All Player Animations

	data_player.death.Reset();
	data_player.jump.Reset();
	data_player.walk.Reset();
	data_player.walk2.Reset();
	data_player.idle.Reset();
	data_player.fall.Reset();
	data_player.running.Reset();
	die = false;
}

bool j1Player::pretime(float sec)
{
	bool ret = false;
	pretimer++;
	if (pretimer >= sec) { ret = true; pretimer = 0; }
	return ret;
}