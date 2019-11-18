#ifndef __j1GOLEM2_H__
#define __j1GOLEM2_H__

#define LIMIT_TIMER 50

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"
#include "p2Animation.h"

struct MapObject;
struct Collider;

enum states_golem2 {
	IDLE3,
	WALK3,
	HURT3,
	ATTACK3,
	DEATH3,
	NONE3
};

struct Golem2Data
{

	Animation*		current_animation;
	Animation		idle;
	Animation		walk;
	Animation		hurt;
	Animation		death;
	Animation		attack;


	bool			grounded = false;
	bool			platformdrop;
	bool			platformdrop2;
	bool			move = false;

	int jumpCounter = 2;

	iPoint position;	//Position of the player (x,y)
	iPoint preposition;
	iPoint a;	//Acceleration (x,y)
	iPoint v;	//Velocity of the player (x,y)

	SDL_Rect		col;
	iPoint			colOffset;
	SDL_Texture*	Tex_Player;

	bool injump = false;	//Says that the player is jumping or not
	bool canjump = false;  //Says the player can jump or not
	int jumpvel;		//Jump velocity only in y
	int jumpenergy;		//energy while is jumping
	bool left = false;	//If left = true, jump running backward
	bool right = false;	//If right = true, jump running forward
	bool player_flip;
	bool TimeAttack = false;

	int gravity;

	float velrun;			//velocity while running

	Collider* colliders = nullptr;
	Collider* colliders2 = nullptr;
	bool showcolliders = false;

	//void Pushbacks();	//Load the frame animations

};

// ----------------------------------------------------
class j1Golem2 : public j1Module
{
public:

	//Constructor
	j1Golem2();

	// Destructor
	virtual ~j1Golem2();

	//Called at first
	bool Start();
	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);

	// Called before render is available

	bool Awake(pugi::xml_node& conf);


	// Called each loop iteration

	void DrawGolem2();

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	// Called before quitting
	bool CleanUp();

	//Called to start the player at initial pos
	//void Restart();

	//Change the sound depending the animation


	//Puts the camera on player and follows

	void CheckState(float dt);	//Load keys to check states
	void State(float dt);	//Check animations
	void Pushbacks();	//Load the frame animations
	void Reset();
	bool pretime(float sec);

	// Collisions
	void OnCollision(Collider* c1, Collider* c2);

private:


public:

	Golem2Data			data_golem2;
	int globaltime;
	int pretimer = 0;
	bool die = false;
	bool godmode = false;

private:


	states_golem2		current_state;
	p2SString			folder;
	p2SString			texture;


};



#endif // __j1GOLEM2_H__