#include "j1App.h"
#include "j1Entity.h"
#include "j1Collider.h"
#include "j1Entities.h"
#include "j1Render.h"
#include "j1Scene.h"

j1Entity::j1Entity(int x, int y) : position(x, y)
{
}

j1Entity::~j1Entity()
{
	if (collider != nullptr) {
		App->collision->EraseCollider(collider);
		collider = nullptr;
	}
}

const Collider* j1Entity::GetCollider() const
{
	return collider;
}

void j1Entity::Draw(SDL_Texture* sprites)
{
	SDL_Rect r = animation->GetCurrentFrame();

	if (collider != nullptr)
		(this->type == PLAYER) ? collider->SetPos(position.x, position.y - r.h) :
		collider->SetPos(position.x, position.y);

}

void j1Entity::OnCollision(Collider* collider)
{

}