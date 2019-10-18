#pragma once
#ifndef __j1ENTITY_H__
#define __j1ENTITY_H__

#include "p2Point.h"
#include "p2Animation.h"

struct SDL_Texture;
struct Collider;

class j1Entity
{
private:
	SDL_Rect last_frame = { 0,0,0,0 };
protected:
	Animation* animation = nullptr;
	Collider* collider = nullptr;
	int lives = 0;
	uint currentTime = 0u;
	uint lastTime = 0u;
	bool key_entities_speed = false;

public:
	fPoint position = fPoint(0, 0);
	int type = 0;
	bool to_destroy = false;

public:
	j1Entity(int x, int y);
	virtual ~j1Entity();

	const Collider* GetCollider() const;

	virtual void Update(float dt) {};
	virtual void Shoot(float dt) {};
	virtual void Draw(SDL_Texture* sprites);
	virtual void OnCollision(Collider* collider);
	virtual uint getLives() { return 0; };
	virtual void SetEntitiesSpeed(float dt) {};

	virtual void LoadAnimation(pugi::xml_node animation_node, Animation* animation) {};

	virtual bool Load(pugi::xml_node&)
	{
		return true;
	}

	virtual bool Save(pugi::xml_node&) const
	{
		return true;
	}
};

#endif // __j1ENTITY_H__