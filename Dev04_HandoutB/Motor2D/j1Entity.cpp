#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Audio.h"
#include "j1Textures.h"
#include "j1Entity.h"
#include "j1Window.h"
#include "j1Map.h"
#include "p2Animation.h"

j1Entity::j1Entity(entity_type type){
}

j1Entity::~j1Entity()
{
	App->tex->UnLoad(data_entity.texture);
	App->tex->UnLoad(data_entity.path_texture);
}

bool j1Entity::DrawEntity(int x, int y, entity_type type) {
	
	bool ret = false;
	if (data_entity.current_animation != nullptr)
	{
		App->render->Blit(data_entity.texture, data_entity.position.x, data_entity.position.y, &(data_entity.current_animation->GetCurrentFrame()), SDL_FLIP_HORIZONTAL);
		ret = true;
	}
	return ret;
}

void j1Entity::OnCollision(Collider* c1, Collider* c2) {}