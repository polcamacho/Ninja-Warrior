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
#include "j1EntityManager.h"
#include <math.h>
#include "..//Brofiler/Brofiler.h"


j1Player::j1Player(int x, int y) : j1Entity(entity_type::PLAYER)
{
	data_player.ipos.x = x;
	data_player.ipos.y = y;
}

// Destructor
j1Player::~j1Player()
{}

bool j1Player::Awake(pugi::xml_node& config) {

	//Load All Player Features from Config
	data_player.jumpvel = config.child("jump_velocity").attribute("jumpvel").as_int(-24);
	 
	v.x = config.child("velocity").attribute("x").as_int(5);
	data_player.velrun = config.child("velrun").attribute("x").as_float(3);
	
	data_player.colOffset.x = config.child("colOffset").attribute("x").as_int();
	data_player.colOffset.y = config.child("colOffset").attribute("y").as_int();
	
	gravity = config.child("gravity").attribute("grav").as_int(20);
	return true;

}

bool j1Player::Start() {

	pretimer = 0;
	globaltime = SDL_GetTicks();	//Sets the Global time to the death timer

	position.x = data_player.ipos.x;
	position.y = data_player.ipos.y;

	Pushbacks();	//Call all the Pushback of animations
	
	SDL_Rect c;
	c.x = position.x;
	c.y = position.y;
	c.w = 39;
	c.h = 53;

	entity_colliders = App->collider->AddCollider(&c, COLLIDER_PLAYER, this);	//Sets The Collider Type and Dimensions to Player
	
	return	true;

}

bool j1Player::PreUpdate(float dt) {

	BROFILER_CATEGORY("PreUpdatePlayer", Profiler::Color::Gray);

	return true;

}

bool j1Player::Update(float dt) {

	BROFILER_CATEGORY("Update player", Profiler::Color::Red);
	
	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) {

		if (godmode == false)	//If godmode is false sets None Collider to player for he can fly around map and not collide
		{

			gravity = 0;	//Sets new gravity to player for he can move around map
			godmode = true;	

		}
		else if (godmode == true)
		{

			gravity = 20;	//Sets normal gravity to player 
			godmode = false;
			
		}
	}
	
	if (godmode == true) {	//Sets the Debug keys to player move when he cant die

		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
			position.y -= (int)(20 * dt * LIMIT_TIMER);

		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
			position.y += (int)(20 * dt * LIMIT_TIMER);

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
			position.x -= (int)(20 * dt * LIMIT_TIMER);

		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
			position.x += (int)(20 * dt * LIMIT_TIMER);
	}
	
	position.y += gravity;
	preposition = position;
	
	Camera();
	CheckState(dt);	//Checks the state where is the player
	State(dt);	//Set the animation relationed with the state that he is
	DrawCollider();

	//Player Draw
	if (flip) {
		App->render->Blit(App->entity->Tex_Player, position.x, position.y, &(current_animation->GetCurrentFrame()), SDL_FLIP_HORIZONTAL, 1.0);	//Draw Player Flipped
	}
	else {
		App->render->Blit(App->entity->Tex_Player, position.x, position.y, &(current_animation->GetCurrentFrame()), SDL_FLIP_NONE, 1.0);	//Draw Player Normal
	}

	if (grounded == false) {	//Sets that if the player is not touching the ground puts the fall animation

		current_stateP = JUMP_FALL1;

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
	j1Entity::CleanUp();
		
	return true;
}

bool j1Player::Load(pugi::xml_node& node) {

	if (!node.child("playerposition").empty())	//Load the valors if save_confign file is empty link
	{

		position.x = node.child("playerposition").attribute("x").as_int();	//Load Player X
		position.y = node.child("playerposition").attribute("y").as_int();	//Load Player Y
	}

	else {	
		position.x = position.x;	//Load Player X
		position.y = position.y;	//Load Player Y
	}
	return true;

}
bool j1Player::Save(pugi::xml_node& node) const {

	if (node.child("playerposition").empty())	//Save The Position Valors in the save_config file
	{
		pugi::xml_node&  save = node.append_child("playerposition");	
		save.append_attribute("x").set_value(position.x);	//Save Player X
		save.append_attribute("y").set_value(position.y);	//Save Player Y
	}
	else
	{
		node.child("playerposition").attribute("x").set_value(position.x);	//Save Player X
		node.child("playerposition").attribute("y").set_value(position.y);	//Save Player Y
	}
	
	return true;

}

void j1Player::Pushbacks() {

	//Sets the Idle Animation
	idle.PushBack({ 7,554,48,52 }, 0.1, 0, 0);
	idle.PushBack({ 84,554,41,52 }, 0.1, 10, 0);
	idle.PushBack({ 156,555,42,50 }, 0.2, 10, 0);
	idle.PushBack({ 232,557,41,48 }, 0.2, 10, 0);
	idle.PushBack({ 291,557,41,48 }, 0.2, 10, 0);
	idle.PushBack({ 351,557,40,48 }, 0.2, 10, 0);
	idle.PushBack({ 415,556,41,49 }, 0.2, 10, 0);
	idle.PushBack({ 474,556,42,49 }, 0.2, 10, 0);
	idle.PushBack({ 533,557,41,48 }, 0.2, 10, 0);
	idle.PushBack({ 591,554,41,51 }, 0.1, 0, 0);
	idle.PushBack({ 643,554,52,52 }, 0.1, -1, 0);
	idle.PushBack({ 705,555,44,50 }, 0.1, -6, 0);
	idle.PushBack({ 763,554,43,52 }, 0.2, -20, 0);
	idle.PushBack({ 819,554,45,52 }, 0.2, -20, 0);
	idle.PushBack({ 880,554,43,51 }, 0.2, -20, 0);
	idle.PushBack({ 938,555,43,51 }, 0.2, -20, 0);
	idle.PushBack({ 995,554,44,52 }, 0.2, -20, 0);
	idle.loop = true;

	//Sets the Walk Animation
	walk.PushBack({ 7,4,42,49 }, 0.2, 0, 0);
	walk.PushBack({ 65,4,41,49 }, 0.2, 0, 0);
	walk.PushBack({ 131,3,38,50 }, 0.2, 0, 0);
	walk.PushBack({ 201,3,36,51 }, 0.2, 0, 0);
	walk.PushBack({ 259,5,38,48 }, 0.2, 0, 0);
	walk.PushBack({ 317,4,44,50 }, 0.2, 0, 0);
	walk.loop = true;

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
	fall.PushBack({ 348,119,56,52 }, 0.05, 0, 0);
	fall.PushBack({ 423,129,53,42 }, 0.5, 0, 0);
	fall.loop = true;

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
	death.PushBack({ 8,464,48,55 }, 0.2, 0, 0);
	death.PushBack({ 72,456,62,63 }, 0.2, 0, 0);
	death.PushBack({ 150,454,69,65 }, 0.2, 0, 0);
	death.PushBack({ 257,464,82,56 }, 0.2, 0, 0);
	death.PushBack({ 367,447,83,72 }, 0.2, 0, 0);
	death.PushBack({ 488,447,84,73 }, 0.2, 0, 0);
	death.PushBack({ 609,457,94,62 }, 0.2, 0, 0);
	death.PushBack({ 734,455,113,65 }, 0.2, 0, 0);
	death.PushBack({ 894,445,127,74 }, 0.2, 0, 0);
	death.PushBack({ 1054,433,116,87 }, 0.2, 0, 0);
	death.loop = false;

}

void j1Player::CheckState(float dt)
{
	if (godmode == false) {

		//current_stateP = IDLE1;
		//data_player.velrun = v.x + 0.25;
		
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && data_player.canjump == true) {

			current_stateP= WALK1;
			position.x += v.x*dt*LIMIT_TIMER;
			flip = false;

			//if  "SPACE" is pressed when "D" is pressed, the player jumps forward
			if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
				

				current_stateP = JUMP_WALK1;
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

				current_stateP = RUN1;
				position.x += data_player.velrun*dt*LIMIT_TIMER;
				flip = false;

				//if "SPACE" is pressed when "LSHIFT" is pressed, and when "D" is pressed, the player jumps running forward
				if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
					
					
					data_player.jumpCounter = 2;					//reset counter. If we dont do it, character cannot do the double jump while running

					data_player.right = true;						//player can jump forward
					data_player.left = false;
					App->audio->PlayFx(App->scene->jump_FX);


					current_stateP = JUMP_RUN1;

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

			current_stateP = WALK1;
			position.x -= v.x*dt*LIMIT_TIMER;
			flip = true;


			//if "SPACE" is pressed when "A" is pressed, the player jumps backward
			if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
				
				current_stateP = JUMP_WALK1;

				data_player.jumpenergy = data_player.jumpvel;
				data_player.jumpCounter--;
				App->audio->PlayFx(App->scene->jump_FX);

				if (data_player.jumpCounter == 0) {
					data_player.jump.Reset();
				}
			}

			//if  "LSHIFT" is pressed when "A" is pressed, the player runs backward
			if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) {

				current_stateP = RUN1;
				position.x -= data_player.velrun*dt*LIMIT_TIMER;

				flip = true;

				//if "SPACE" is pressed when "LSHIFT" is pressed, and when "A" is pressed, the player jumps running backward
				if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
				
					data_player.jumpCounter = 2;			//reset counter. If we dont do it, character cannot do the double jump while running
					data_player.left = true;				//player can jump backward
					data_player.right = false;

					current_stateP = JUMP_RUN1;

					data_player.jumpCounter--;
					App->audio->PlayFx(App->scene->jump_FX);

					if (data_player.jumpCounter == 0) {
						data_player.jump.Reset();
					}
				}
			}
		}

		//if "SPACE" is pressed the player jumps
		else if ((App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) && (data_player.jumpCounter != 0)) {		//counter needs to be !=0, if not cannot do double jump
			
			

			current_stateP = JUMP_UP1;
			flip = false;
			data_player.jumpenergy = data_player.jumpvel;
			data_player.jumpCounter--;
			App->audio->PlayFx(App->scene->jump_FX);


			if (data_player.jumpCounter == 0) {
				data_player.jump.Reset();
			}

		}
		
		else if ((App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN) && data_player.canjump == true || (data_player.TimeAttack == false)) {

			current_stateP = IDLE_ATTACK1;

		}

		else if (data_player.canjump == true && App->input->GetKey(SDL_SCANCODE_SPACE) == NULL && App->input->GetKey(SDL_SCANCODE_A) == NULL && App->input->GetKey(SDL_SCANCODE_D&& App->input->GetKey(SDL_SCANCODE_P) == NULL) == NULL) {
			current_stateP = IDLE1;
			
		}
	}

}

void j1Player::State(float dt) {

	if (current_stateP == IDLE1) {

		current_animation = &idle;
		data_player.jump.Reset();
		fall.Reset();
		
	}

	if (current_stateP == WALK1) {

		current_animation = &walk;
		data_player.jump.Reset();
		fall.Reset();


	}
	if (current_stateP == JUMP_WALK1) {

		current_stateP = JUMP_UP1;

	}

	if (current_stateP == JUMP_RUN1) {

		//If left = true, jump running backward
		if (data_player.left == true) {

			position.x -= data_player.velrun*dt*LIMIT_TIMER;
		}

		//If right = true, jump running forward
		else if (data_player.right == true) {

			position.x += data_player.velrun*dt*LIMIT_TIMER;
		}

		data_player.canjump = false;
		data_player.injump = true;
		current_animation = &data_player.jump;

		if (data_player.jumpenergy <= gravity) {								//character will jump up until it do not accomplish this condition 

			data_player.jumpenergy += 0.5;													// jump up increments 0.5 each time
			position.y = position.y + data_player.jumpenergy*dt*LIMIT_TIMER;		// y position increments 0.5 each time

		}

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			position.x -= v.x*dt*LIMIT_TIMER;
			flip = true;
		}
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			position.x += v.x *dt*LIMIT_TIMER;
			flip = false;
		}

	}

	if (current_stateP == RUN1) {
		current_animation = &data_player.walk2;
		data_player.jump.Reset();
		fall.Reset();
		data_player.idle_attack.Reset();
	}

	if (current_stateP == JUMP_UP1) {

		data_player.canjump = false;
		data_player.injump = true;
		current_animation = &data_player.jump;

		if (data_player.jumpenergy <= gravity) {									//character will jump up until it do not accomplish this condition 

			data_player.jumpenergy += 0.5;														// jump up increments 0.5 each time
			position.y = position.y + data_player.jumpenergy*dt*LIMIT_TIMER;			// y position increments 0.5 each time

		}

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			position.x -= v.x*dt*LIMIT_TIMER;
			flip = true;
		}

		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			position.x += v.x*dt*LIMIT_TIMER;
			flip = false;
		}

	}

	if (current_stateP == JUMP_FALL1) {

		current_animation = &fall;
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			position.x -= v.x*dt*LIMIT_TIMER;
			flip = true;
		}
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			position.x += v.x*dt*LIMIT_TIMER;
			flip = false;
		}
	}

	if (current_stateP == IDLE_ATTACK1) {

		current_animation = &data_player.idle_attack;
		data_player.TimeAttack = true;
		if (current_animation->AnimFinished() == true) {

			data_player.TimeAttack = false;

		}


	}

	if (current_stateP == DEATH1) {

		if (App->scene->current_map == "Map.tmx") {	//If player is in map 1

				if (PreTime(20)) {	//Do a timer to stop the game during the Death animation
					
					current_animation = &death;	//Current Animation is Death
					App->audio->PlayFx(App->scene->death_FX);	//Sets the Death Audio
					
					death.Reset();
				}

		}

		else {	//If player is not in map 1 is in map 2

				if (PreTime(20)) {	//Do a timer to stop the game during the Death Animation
					
					current_animation = &death;	//Current Animation is Death
					App->audio->PlayFx(App->scene->death_FX);	//Sets the Death Audio
					
					death.Reset();

				}

		}


	}
	

}
	


void j1Player::OnCollision(Collider* c1, Collider* c2) {	//Check if the Player collides with something

	if(godmode==false){

		if (c1->type == ColliderType::COLLIDER_PLAYER && c2->type == ColliderType::COLLIDER_FLOOR) {	//If player collide with floor

			if (preposition.y < c2->rect.y || position.y == c2->rect.y - entity_colliders->rect.h) {	//Checks that player collider from above

				position.y = c2->rect.y - entity_colliders->rect.h;
				grounded = true;	//Sets that player is touching the floor
				data_player.canjump = true;		//Sets tha player can jump
				
				if (data_player.injump == true) {
					data_player.jump.Reset();
				}

				if (grounded == true) {
					data_player.jumpCounter = 2;
				}

			}

			else if (preposition.y > (c2->rect.y + c2->rect.h)) {	//Checks that player collider from below
			
				position.y = c2->rect.y + c2->rect.h;
				//grounded = true;	//Sets that player is touching the floor
				current_stateP = JUMP_FALL1;	//Sets the animation 

			}
		
			else if ((position.x < c2->rect.x + c2->rect.w && position.x > c2->rect.x) ||(position.x + entity_colliders->rect.w < c2->rect.x + c2->rect.w && position.x + entity_colliders->rect.w > c2->rect.x)) {	//Checks that player collider from sides
			
				if ((position.x + entity_colliders->rect.w) < (c2->rect.x + c2->rect.w)) { //Checks that player collides from left
				
					position.x = c2->rect.x - entity_colliders->rect.w;
					

				}
				else if (position.x < (c2->rect.x + c2->rect.w)) {	//Checks that player collides from right
				
					position.x = c2->rect.x + c2->rect.w;

				}
			}

		}

		if (c1->type == ColliderType::COLLIDER_PLAYER && c2->type == ColliderType::COLLIDER_PLATFORM) {		//Checks that player collides with platform

			if (preposition.y < c2->rect.y || position.y == c2->rect.y - entity_colliders->rect.h) {	//Checks that player collider from above

				position.y = c2->rect.y - entity_colliders->rect.h;
				grounded = true;	//Sets that player is touching the floor
				data_player.canjump = true;		//Sets tha player can jump

				if (grounded == true) {
				
					data_player.jumpCounter = 2;

				}

			}
			else if ((position.y >= preposition.y) && (preposition.y + entity_colliders->rect.h) < c2->rect.y) {	//Checks that player collider from below
			
				position.y = c2->rect.y - entity_colliders->rect.h;

			}

		}

		if (c1->type == ColliderType::COLLIDER_PLAYER && c2->type == ColliderType::COLLIDER_DEAD) {		//Checks that player collides with something that he can die

			
			if (preposition.y < c2->rect.y || position.y == c2->rect.y - entity_colliders->rect.h) {	//Checks that player collider from above
			
				position.y = c2->rect.y - entity_colliders->rect.h;
				grounded = true;	//Sets that player is touching the floor
				data_player.canjump = false;	//Sets tha player can jump

				//TIMER

				{
					if (App->scene->current_map == "Map.tmx") {	//If player is in map 1
						
						current_animation = &death;	//Current Animation is Death
						if (PreTime(40)) {	//Do a timer to stop the game during the Death animation
														
							App->audio->PlayFx(App->scene->death_FX);	//Sets the Death Audio
							
								if (data_player.checkpoint == true) {
							
									App->LoadGame();
									LOG("%i %i", position.x, position.y);
								}

								else if (data_player.checkpoint == false) {

									//Sets the Position that player goes when he dies
									position.x = 100;	//Set Player X	
									position.y = 300;	//Set Player Y
									current_stateP = JUMP_FALL1;	//Sets the Animation when he reapears

								}
							

							death.Reset();
						}

					}

					else {	//If player is not in map 1 is in map 2

						current_animation = &death;	//Current Animation is Death

						if (PreTime(40)) {	//Do a timer to stop the game during the Death Animation

							current_animation = &death;	//Current Animation is Death
							App->audio->PlayFx(App->scene->death_FX);	//Sets the Death Audio

							if (data_player.checkpoint == true) {
			
								App->LoadGame();
							}

							else if (data_player.checkpoint == false) {

								//Sets the Position that player goes when he dies
								position.x = 55;	//Set Player X	
								position.y = 10;	//Set Player Y
								current_stateP = JUMP_FALL1;	//Sets the Animation when he reapears

							}

							death.Reset();

						}

					}
				}

			}

			else if (preposition.y > (c2->rect.y + c2->rect.h)) {	//Checks that player collider from below
			
				position.y = c2->rect.y + c2->rect.h;
				grounded = true;	//Sets that player is touching the floor
				//current_stateP = DEATH1;
				data_player.canjump = false;	//Sets tha player can jump
				current_animation = &death;	//Current Animation is Death
				App->audio->PlayFx(App->scene->death_FX);	//Sets the Death Audio
				//TIMER
				{
					if (App->scene->current_map == "Map.tmx") {	//If player is in map 1

						if (PreTime(40)) {	//Do a timer to stop the game during the Death animation

							current_animation = &death;	//Current Animation is Death
							App->audio->PlayFx(App->scene->death_FX);	//Sets the Death Audio

							//Sets the Position that player goes when he dies
							position.x = 100;	//Set Player X	
							position.y = 300;	//Set Player Y
							current_stateP = JUMP_FALL1;	//Sets the Animation when he reapears
							death.Reset();
						}

					}

					else {	//If player is not in map 1 is in map 2

						if (PreTime(20)) {	//Do a timer to stop the game during the Death Animation

							current_animation = &death;	//Current Animation is Death
							App->audio->PlayFx(App->scene->death_FX);	//Sets the Death Audio

																			//Sets the Position that player goes when he dies
							position.x = 55;	//Set Player X	
							position.y = 10;	//Set Player Y
							current_stateP = JUMP_FALL1;	//Sets the Animation when he reapears
							death.Reset();

						}

					}

				}

			}

		}

		if (c1->type == ColliderType::COLLIDER_PLAYER && c2->type == ColliderType::COLLIDER_NEXT) {

			if (preposition.y < c2->rect.y || position.y == c2->rect.y - entity_colliders->rect.h) {	//Checks that player collider from above	
				App->scene->SecondMap();	//Pass to next map
			}
		
			else if (preposition.y > (c2->rect.y + c2->rect.h)) {	//Checks that player collider from below
				App->scene->SecondMap();	//Pass to next map
			}
		
			else if ((position.x < c2->rect.x + c2->rect.w && position.x > c2->rect.x) || (position.x + entity_colliders->rect.w < c2->rect.x + c2->rect.w && position.x + entity_colliders->rect.w > c2->rect.x)) {	//Checks that player collider from sides
			
				if ((position.x + entity_colliders->rect.w) < (c2->rect.x + c2->rect.w)) {		//Checks that player collides from left
					App->scene->SecondMap();	//Pass to next map
				}
				else if (position.x < (c2->rect.x + c2->rect.w)) {	//Checks that player collides from right
					App->scene->SecondMap();	//Pass to next map
				}
			}
		}

		if (c1->type == ColliderType::COLLIDER_PLAYER && c2->type == ColliderType::COLLIDER_CHECKPOINT) {
			
			data_player.checkpoint = true;

			if (preposition.y < c2->rect.y || position.y == c2->rect.y - entity_colliders->rect.h) {	//Checks that player collider from above	
				
				App->SaveGame();
			}

			else if ((position.x < c2->rect.x + c2->rect.w && position.x > c2->rect.x) || (position.x + entity_colliders->rect.w < c2->rect.x + c2->rect.w && position.x + entity_colliders->rect.w > c2->rect.x)) {	//Checks that player collider from sides

				if ((position.x + entity_colliders->rect.w) < (c2->rect.x + c2->rect.w)) {		//Checks that player collides from left
					
					App->SaveGame();
				}
				else if (position.x < (c2->rect.x + c2->rect.w)) {	//Checks that player collides from right
					
					App->SaveGame();
				}
			}
		}

		if (c1->type == ColliderType::COLLIDER_PLAYER && c2->type == ColliderType::COLLIDER_ENEMY) {

			if (preposition.y < c2->rect.y || position.y == c2->rect.y - entity_colliders->rect.h) {	//Checks that player collider from above	
				//TIMER
				{
					if (App->scene->current_map == "Map.tmx") {	//If player is in map 1

						if (PreTime(40)) {	//Do a timer to stop the game during the Death animation

							current_animation = &death;	//Current Animation is Death
							App->audio->PlayFx(App->scene->death_FX);	//Sets the Death Audio

							//Sets the Position that player goes when he dies
							position.x = 100;	//Set Player X	
							position.y = 300;	//Set Player Y
							current_stateP = JUMP_FALL1;	//Sets the Animation when he reapears
							death.Reset();
						}

					}

					else {	//If player is not in map 1 is in map 2

						if (PreTime(20)) {	//Do a timer to stop the game during the Death Animation

							current_animation = &death;	//Current Animation is Death
							App->audio->PlayFx(App->scene->death_FX);	//Sets the Death Audio

																			//Sets the Position that player goes when he dies
							position.x = 55;	//Set Player X	
							position.y = 10;	//Set Player Y
							current_stateP = JUMP_FALL1;	//Sets the Animation when he reapears
							death.Reset();

						}

					}

				}
			}

			else if ((position.x < c2->rect.x + c2->rect.w && position.x > c2->rect.x) || (position.x + entity_colliders->rect.w < c2->rect.x + c2->rect.w && position.x + entity_colliders->rect.w > c2->rect.x)) {	//Checks that player collider from sides

				if ((position.x + entity_colliders->rect.w) < (c2->rect.x + c2->rect.w)) {		//Checks that player collides from left
					//TIMER
					{
						if (App->scene->current_map == "Map.tmx") {	//If player is in map 1

							if (PreTime(40)) {	//Do a timer to stop the game during the Death animation

								current_animation = &death;	//Current Animation is Death
								App->audio->PlayFx(App->scene->death_FX);	//Sets the Death Audio

								//Sets the Position that player goes when he dies
								position.x = 100;	//Set Player X	
								position.y = 300;	//Set Player Y
								current_stateP = JUMP_FALL1;	//Sets the Animation when he reapears
								death.Reset();
							}

						}

						else {	//If player is not in map 1 is in map 2

							if (PreTime(20)) {	//Do a timer to stop the game during the Death Animation

								current_animation = &death;	//Current Animation is Death
								App->audio->PlayFx(App->scene->death_FX);	//Sets the Death Audio

																				//Sets the Position that player goes when he dies
								position.x = 55;	//Set Player X	
								position.y = 10;	//Set Player Y
								current_stateP = JUMP_FALL1;	//Sets the Animation when he reapears
								death.Reset();

							}

						}

					}
				}
				else if (position.x < (c2->rect.x + c2->rect.w)) {	//Checks that player collides from right
					//TIMER
					{
						if (App->scene->current_map == "Map.tmx") {	//If player is in map 1

							if (PreTime(40)) {	//Do a timer to stop the game during the Death animation

								current_animation = &death;	//Current Animation is Death
								App->audio->PlayFx(App->scene->death_FX);	//Sets the Death Audio

								//Sets the Position that player goes when he dies
								position.x = 100;	//Set Player X	
								position.y = 300;	//Set Player Y
								current_stateP = JUMP_FALL1;	//Sets the Animation when he reapears
								death.Reset();
							}

						}

						else {	//If player is not in map 1 is in map 2

							if (PreTime(20)) {	//Do a timer to stop the game during the Death Animation

								current_animation = &death;	//Current Animation is Death
								App->audio->PlayFx(App->scene->death_FX);	//Sets the Death Audio

																				//Sets the Position that player goes when he dies
								position.x = 55;	//Set Player X	
								position.y = 10;	//Set Player Y
								current_stateP = JUMP_FALL1;	//Sets the Animation when he reapears
								death.Reset();

							}

						}

					}
				}
			}
		}

	}

	else {

	}

}

void j1Player::Reset() {	//Reset All Player Animations

	death.Reset();
	data_player.jump.Reset();
	walk.Reset();
	data_player.walk2.Reset();
	idle.Reset();
	fall.Reset();
	data_player.running.Reset();
	die = false;
}

bool j1Entity::PreTime(float sec)
{
	pretimer++;
	if (pretimer >= sec) { pretimer = 0; }
	return true;
}

void j1Player::Camera() {

	if (App->scene->current_map == "Map.tmx") {

		App->render->camera.x = -(position.x) + (App->win->width / 2);
		App->render->camera.y = -(position.y) + (App->win->height / 1.40);
	}
	else {

		App->render->camera.x = -(position.x) + (App->win->width / 2);
		App->render->camera.y = -(position.y) + (App->win->height / 1.9);
	}

	//Set Camera Limits
	if (App->render->camera.x > 0) {	//Left

		App->render->camera.x = 0;
	}

	if (App->render->camera.y > 0) {	//Above

		App->render->camera.y = 0;
	}
}
