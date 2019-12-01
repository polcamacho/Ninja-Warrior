#include "j1Entity.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Audio.h"
#include "j1Textures.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Collider.h"
#include "j1EntityManager.h"
#include "j1Player.h"
#include "j1Golem1.h"
#include "j1Golem2.h"
#include "j1Bat.h"
#include "j1Scene.h"
j1Entity::j1Entity(entity_type type) : type(type){
}

j1Entity::~j1Entity()
{
	App->tex->UnLoad(App->scene->debug_tex);
}

bool j1Entity::CleanUp() {

	entity_colliders->to_delete = true;
	return true;

}

void j1Entity::DrawCollider() {

	entity_colliders->SetPos(position.x, position.y);

}
