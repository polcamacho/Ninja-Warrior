#include "j1App.h"
#include "j1Input.h"
#include "j1Collider.h"
#include "p2Log.h"
#include "..//Brofiler/Brofiler.h"


j1Collider::j1Collider()
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
		colliders[i] = nullptr;

	//Set the Collider Events of Floor
	matrix[COLLIDER_FLOOR][COLLIDER_FLOOR] = false;
	matrix[COLLIDER_FLOOR][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_FLOOR][COLLIDER_DEAD] = false;
	matrix[COLLIDER_FLOOR][COLLIDER_PLATFORM] = false;
	matrix[COLLIDER_FLOOR][COLLIDER_PLATFORM] = false;
	matrix[COLLIDER_FLOOR][COLLIDER_NEXT] = false;
	matrix[COLLIDER_FLOOR][COLLIDER_ENEMY] = true;
	matrix[COLLIDER_FLOOR][COLLIDER_CHECKPOINT] = false;
	matrix[COLLIDER_FLOOR][COLLIDER_COIN] = false;
	matrix[COLLIDER_FLOOR][COLLIDER_HEART] = false;

	//Set the Collider Events of Player
	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_FLOOR] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_DEAD] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_PLATFORM] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_NEXT] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_ENEMY] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_CHECKPOINT] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_COIN] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_HEART] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_TIMER] = true;


	//Set the Collider Events of Enemy
	matrix[COLLIDER_ENEMY][COLLIDER_ENEMY] = false;
	matrix[COLLIDER_ENEMY][COLLIDER_FLOOR] = true;
	matrix[COLLIDER_ENEMY][COLLIDER_DEAD] = true;
	matrix[COLLIDER_ENEMY][COLLIDER_PLATFORM] = true;
	matrix[COLLIDER_ENEMY][COLLIDER_NEXT] = true;
	matrix[COLLIDER_ENEMY][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_ENEMY][COLLIDER_CHECKPOINT] = false;
	matrix[COLLIDER_ENEMY][COLLIDER_COIN] = false;
	matrix[COLLIDER_ENEMY][COLLIDER_HEART] = false;

	//Set the Collider Events of Dead
	matrix[COLLIDER_DEAD][COLLIDER_DEAD] = false;
	matrix[COLLIDER_DEAD][COLLIDER_FLOOR] = false;
	matrix[COLLIDER_DEAD][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_DEAD][COLLIDER_PLATFORM] = false;
	matrix[COLLIDER_DEAD][COLLIDER_NEXT] = false;
	matrix[COLLIDER_DEAD][COLLIDER_ENEMY] = true;
	matrix[COLLIDER_DEAD][COLLIDER_CHECKPOINT] = false;
	matrix[COLLIDER_DEAD][COLLIDER_COIN] = false;
	matrix[COLLIDER_DEAD][COLLIDER_HEART] = false;

	//Set the Collider Events of Next
	matrix[COLLIDER_NEXT][COLLIDER_NEXT] = false;
	matrix[COLLIDER_NEXT][COLLIDER_DEAD] = false;
	matrix[COLLIDER_NEXT][COLLIDER_FLOOR] = false;
	matrix[COLLIDER_NEXT][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_NEXT][COLLIDER_PLATFORM] = false;
	matrix[COLLIDER_NEXT][COLLIDER_ENEMY] = false;
	matrix[COLLIDER_NEXT][COLLIDER_CHECKPOINT] = false;
	matrix[COLLIDER_NEXT][COLLIDER_COIN] = false;
	matrix[COLLIDER_NEXT][COLLIDER_HEART] = false;

	//Set the Collider Events of Checkpoint
	matrix[COLLIDER_CHECKPOINT][COLLIDER_CHECKPOINT] = false;
	matrix[COLLIDER_CHECKPOINT][COLLIDER_DEAD] = false;
	matrix[COLLIDER_CHECKPOINT][COLLIDER_FLOOR] = false;
	matrix[COLLIDER_CHECKPOINT][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_CHECKPOINT][COLLIDER_PLATFORM] = false;
	matrix[COLLIDER_CHECKPOINT][COLLIDER_ENEMY] = false;
	matrix[COLLIDER_CHECKPOINT][COLLIDER_NEXT] = false;
	matrix[COLLIDER_CHECKPOINT][COLLIDER_COIN] = false;
	matrix[COLLIDER_CHECKPOINT][COLLIDER_HEART] = false;

	//Set the Collider Events of HEART
	matrix[COLLIDER_HEART][COLLIDER_CHECKPOINT] = false;
	matrix[COLLIDER_HEART][COLLIDER_DEAD] = false;
	matrix[COLLIDER_HEART][COLLIDER_FLOOR] = false;
	matrix[COLLIDER_HEART][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_HEART][COLLIDER_PLATFORM] = false;
	matrix[COLLIDER_HEART][COLLIDER_ENEMY] = false;
	matrix[COLLIDER_HEART][COLLIDER_NEXT] = false;

	//Set the Collider Events of Coin
	matrix[COLLIDER_COIN][COLLIDER_CHECKPOINT] = false;
	matrix[COLLIDER_COIN][COLLIDER_DEAD] = false;
	matrix[COLLIDER_COIN][COLLIDER_FLOOR] = false;
	matrix[COLLIDER_COIN][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_COIN][COLLIDER_PLATFORM] = false;
	matrix[COLLIDER_COIN][COLLIDER_ENEMY] = false;
	matrix[COLLIDER_COIN][COLLIDER_NEXT] = false;
	matrix[COLLIDER_COIN][COLLIDER_HEART] = false;
	
}

// Destructor
j1Collider::~j1Collider()
{}

bool j1Collider::PreUpdate(float dt)
{
	BROFILER_CATEGORY("PreUpdate Collider", Profiler::Color::Green);

	// Remove all colliders scheduled for deletion
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr && colliders[i]->to_delete == true)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	Collider* c1;
	Collider* c2;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		// skip empty colliders
		if (colliders[i] == nullptr)
			continue;

		c1 = colliders[i];
		if (c1->Enabled == false)
		{
			continue;
		}

		// avoid checking collisions already checked
		for (uint k = i + 1; k < MAX_COLLIDERS; ++k)
		{
			// skip empty colliders
			if (colliders[k] == nullptr)
				continue;

			c2 = colliders[k];
			if (c2->Enabled == false)
			{
				continue;
			}

			if (c1->CheckCollision(c2->rect) == true)
			{
				if (matrix[c1->type][c2->type] && c1->callback)
					c1->callback->OnCollision(c1, c2);

				if (matrix[c2->type][c1->type] && c2->callback)
					c2->callback->OnCollision(c2, c1);
			}
		}
	}

	return true;
}

// Called before render is available
bool j1Collider::Update(float dt)
{
	BROFILER_CATEGORY("Update Collider", Profiler::Color::LimeGreen);

	DebugDraw();
	DT = dt;

	return true;
}

void j1Collider::DebugDraw()
{
	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		debug = !debug;

	if (debug == false)
		return;

	Uint8 alpha = 80;
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
			continue;

		switch (colliders[i]->type)
		{
		case COLLIDER_NONE: // Set Collider Color white
			App->render->DrawQuad(colliders[i]->rect, 255, 255, 255, alpha);
			break;
		case COLLIDER_FLOOR: // Set Collider Color blue
			App->render->DrawQuad(colliders[i]->rect, 0, 0, 255, alpha);
			break;
		case COLLIDER_DEAD: // Set Collider Color red
			App->render->DrawQuad(colliders[i]->rect, 255, 0, 0, alpha);
			break;
		case COLLIDER_PLATFORM: // Set Collider Color blue white
			App->render->DrawQuad(colliders[i]->rect, 0, 255, 255, alpha);
			break;
		case COLLIDER_PLAYER: // Set Collider Color green
			App->render->DrawQuad(colliders[i]->rect, 0, 255, 0, alpha);
			break;

		case COLLIDER_NEXT: // Set Collider Color violet
			App->render->DrawQuad(colliders[i]->rect, 130, 5, 255, alpha);
			break;

		case COLLIDER_ENEMY: // Set Collider Color violet
			App->render->DrawQuad(colliders[i]->rect, 255, 200, 20, alpha);
			break;
		case COLLIDER_CHECKPOINT: // Set Collider Color violet
			App->render->DrawQuad(colliders[i]->rect, 100, 255, 100, alpha);
			break;
		case COLLIDER_HEART: // Set Collider Color violet
			App->render->DrawQuad(colliders[i]->rect, 255, 0, 255, alpha);
			break;
		case COLLIDER_COIN: // Set Collider Color violet
			App->render->DrawQuad(colliders[i]->rect, 100, 100, 100, alpha);
			break;
		case COLLIDER_TIMER:
			App->render->DrawQuad(colliders[i]->rect, 255, 200, 0, alpha);
			break;
		
		default:
			break;
		}
	}
}

// Called before quitting
bool j1Collider::CleanUp()
{
	LOG("Freeing all colliders");

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	return true;
}

Collider* j1Collider::AddCollider(SDL_Rect* rect, ColliderType type, j1Module* callback)
{
	Collider* ret = nullptr;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
		{
			ret = colliders[i] = new Collider(*rect, type, callback);
			break;
		}
	}

	return ret;
}

bool Collider::CheckCollision(const SDL_Rect& r) const
{
	return (rect.x < r.x + r.w &&
			rect.x + rect.w > r.x &&
			rect.y < r.y + r.h &&
			rect.h + rect.y > r.y);
}

