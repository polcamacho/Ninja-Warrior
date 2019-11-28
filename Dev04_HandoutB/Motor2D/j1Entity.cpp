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

j1Entity::j1Entity(entity_type type) : type(type){
}

j1Entity::~j1Entity()
{
	
}

bool j1Entity::CleanUp() {

	entity_colliders->to_delete = true;
	return true;

}

bool j1Entity::Load(pugi::xml_node& data) {
	return true;
}

bool j1Entity::Save(pugi::xml_node& data) const {
	return true;
}

/*bool j1Entity::DrawEntity(int x, int y, entity_type type) {
	
	bool ret = false;
	if (data_entity.current_animation != nullptr)
	{
		App->render->Blit(data_entity.texture, data_entity.position.x, data_entity.position.y, &(data_entity.current_animation->GetCurrentFrame()), SDL_FLIP_HORIZONTAL);
		ret = true;
	}
	return ret;
}*/

void j1Entity::DrawCollider() {

	entity_colliders->SetPos(position.x, position.y);

}
