#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Scene.h"
#include "j1Map.h"
#include <math.h>

j1Map::j1Map() : j1Module(), map_loaded(false)
{
	name.create("map");
}

// Destructor
j1Map::~j1Map()
{}

// Called before render is available
bool j1Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;

	folder.create(config.child("folder").child_value());
	
	return ret;
}

void j1Map::Draw()
{
	if (map_loaded == false)
		return;
	
	for (uint x = 0; x < data.tilesets.count(); x++) {		//Run the total tilesets

		for (uint y = 0; y < data.layers.count(); y++) {	//Run total of the layers

			for (uint i = 0; i < data.height; i++) {			//Run the MapData height

				for (uint j = 0; j < data.width; j++) {			//Run the MapData width

					App->render->Blit(data.tilesets[x]->texture, j*data.tile_width, i*data.tile_height, &data.tilesets[x]->tile_id(data.layers[y]->data[data.layers[y]->Get(j, i)]), SDL_FLIP_NONE, -data.layers[y]->parallax); //Parallax// -data.layers[y]->parallax); //	Blits the Map and call the parallax item to do it
				
				}																						
			
			}
		
		}
	}

}

iPoint j1Map::GetWorldPos(int x, int y) const
{

	iPoint ret;

	ret.x = x * data.tile_width;
	ret.y = y * data.tile_height;

	return ret;
}

SDL_Rect TileSet::tile_id(int id) const
{

	int id_tileset = id - firstgid;
	SDL_Rect rect;
		
		rect.h = tile_height;
		rect.w = tile_width;
		rect.x = margin + ((rect.w + spacing)*(id_tileset  % num_tiles_width));
		rect.y = margin + ((rect.h + spacing)*(id_tileset / num_tiles_width));


	return rect;
}

// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");

	// Remove all tilesets
	p2List_item<TileSet*>* item;
	item = data.tilesets.start;

	while(item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	data.tilesets.clear();

	// TODO 2: clean up all layer data
	// Remove all layers

	p2List_item<MapLayer*>* item2;
	item2 = data.layers.start;

	while (item2 != NULL)
	{
		RELEASE(item2->data);
		item2 = item2->next;
	}
	data.layers.clear();

	//Remove all object layers
	p2List_item<OBJG*>* item3;
	item3 = data.objects.start;

	while (item3 != NULL)
	{
		RELEASE(item3->data);
		item3 = item3->next;
	}
	data.objects.clear();

	// Clean up the pugui tree
	map_file.reset();

	return true;
}

// Load new map
bool j1Map::Load(const char* file_name)
{
	bool ret = true;
	p2SString tmp("%s%s", folder.GetString(), file_name);
	
	pugi::xml_parse_result result = map_file.load_file(tmp.GetString());

	if(result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	// Load general info ----------------------------------------------
	if(ret == true)
	{
		ret = LoadMap();
	}

	// Load all tilesets info ----------------------------------------------
	pugi::xml_node tileset;
	for(tileset = map_file.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if(ret == true)
		{
			
			ret = LoadTilesetDetails(tileset, set);
		}

		if(ret == true)
		{
			ret = LoadTilesetImage(tileset, set);
		}

		data.tilesets.add(set);
	}

	
	// TODO 4: Iterate all layers and load each of them
	// Load layer info ----------------------------------------------

	pugi::xml_node layernode;
	for (layernode = map_file.child("map").child("layer"); layernode && ret; layernode = layernode.next_sibling("layer"))
	{
		MapLayer* set = new MapLayer();

		if (ret == true)
		{
			ret = LoadLayer(layernode, set);
		}

		data.layers.add(set);
	}

	pugi::xml_node groupnode;
	for (groupnode = map_file.child("map").child("objectgroup"); groupnode && ret; groupnode = groupnode.next_sibling("objectgroup"))
	{
		OBJG* set = new OBJG();
		
				
		if (ret == true)
		{
			set->name = groupnode.attribute("name").as_string();
			for (pugi::xml_node& obj = groupnode.child("object"); obj && ret; obj = obj.next_sibling("object"))
			{
				MapObject* data = new MapObject;

				data->height = obj.attribute("height").as_uint();
				data->width = obj.attribute("width").as_uint();
				data->x = obj.attribute("x").as_uint();
				data->y = obj.attribute("y").as_uint();
				data->name = obj.attribute("name").as_string();

				set->objects.add(data);
			}
		}
		data.objects.add(set);
	}

	if(ret == true)
	{
		LOG("Successfully parsed map XML file: %s", file_name);
		LOG("width: %d height: %d", data.width, data.height);
		LOG("tile_width: %d tile_height: %d", data.tile_width, data.tile_height);

		p2List_item<TileSet*>* item = data.tilesets.start;
		while(item != NULL)
		{
			TileSet* s = item->data;
			LOG("Tileset ----");
			LOG("name: %s firstgid: %d", s->name.GetString(), s->firstgid);
			LOG("tile width: %d tile height: %d", s->tile_width, s->tile_height);
			LOG("spacing: %d margin: %d", s->spacing, s->margin);
			item = item->next;
		}

		p2List_item<MapLayer*>* item_layers = data.layers.start;
		while(item_layers != NULL)
		{
			MapLayer* l = item_layers->data;
			LOG("Layer ----");
			LOG("name: %s", l->name.GetString());
			LOG("tile width: %d tile height: %d", l->width, l->height);
			item_layers = item_layers->next;
		}

		p2List_item<OBJG*>* obj_layer = data.objects.start;
		while (item_layers != NULL)
		{
			OBJG* obj = obj_layer->data;
			LOG("OBJ ----");
			LOG("name: %s", obj->name.GetString());
			obj_layer = obj_layer->next;
		}
	}

	map_loaded = ret;

	return ret;
}

// Load map general properties
bool j1Map::LoadMap()
{
	bool ret = true;
	pugi::xml_node map = map_file.child("map");

	if(map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		data.width = map.attribute("width").as_int();
		data.height = map.attribute("height").as_int();
		data.tile_width = map.attribute("tilewidth").as_int();
		data.tile_height = map.attribute("tileheight").as_int();
		p2SString bg_color(map.attribute("backgroundcolor").as_string());

		data.background_color.r = 0;
		data.background_color.g = 0;
		data.background_color.b = 0;
		data.background_color.a = 0;

		if(bg_color.Length() > 0)
		{
			p2SString red, green, blue;
			bg_color.SubString(1, 2, red);
			bg_color.SubString(3, 4, green);
			bg_color.SubString(5, 6, blue);

			int v = 0;

			sscanf_s(red.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.r = v;

			sscanf_s(green.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.g = v;

			sscanf_s(blue.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.b = v;
		}

		p2SString orientation(map.attribute("orientation").as_string());

		if(orientation == "orthogonal")
		{
			data.type = MAPTYPE_ORTHOGONAL;
		}
		else if(orientation == "isometric")
		{
			data.type = MAPTYPE_ISOMETRIC;
		}
		else if(orientation == "staggered")
		{
			data.type = MAPTYPE_STAGGERED;
		}
		else
		{
			data.type = MAPTYPE_UNKNOWN;
		}
	}

	return ret;
}

bool j1Map::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	set->name.create(tileset_node.attribute("name").as_string());
	set->firstgid = tileset_node.attribute("firstgid").as_int();
	set->tile_width = tileset_node.attribute("tilewidth").as_int();
	set->tile_height = tileset_node.attribute("tileheight").as_int();
	set->margin = tileset_node.attribute("margin").as_int();
	set->spacing = tileset_node.attribute("spacing").as_int();
	pugi::xml_node offset = tileset_node.child("tileoffset");

	if(offset != NULL)
	{
		set->offset_x = offset.attribute("x").as_int();
		set->offset_y = offset.attribute("y").as_int();
	}
	else
	{
		set->offset_x = 0;
		set->offset_y = 0;
	}

	return ret;
}

bool j1Map::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tileset_node.child("image");

	if(image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		set->texture = App->tex->Load(PATH(folder.GetString(), image.attribute("source").as_string()));
		int w, h;
		SDL_QueryTexture(set->texture, NULL, NULL, &w, &h);
		set->tex_width = image.attribute("width").as_int();

		if(set->tex_width <= 0)
		{
			set->tex_width = w;
		}

		set->tex_height = image.attribute("height").as_int();

		if(set->tex_height <= 0)
		{
			set->tex_height = h;
		}

		set->num_tiles_width = set->tex_width / set->tile_width;
		set->num_tiles_height = set->tex_height / set->tile_height;
	}

	return ret;
}

bool j1Map::LoadLayer(pugi::xml_node& layernode, MapLayer* layer)
{

	bool ret = true;
	int i=0;
	layer->name.create(layernode.attribute("name").as_string());
	layer->width = layernode.attribute("width").as_uint();
	layer->height = layernode.attribute("height").as_uint();
	layer->tilegid =layer->width * layer->height;
	layer->data = new uint[layer->tilegid];
	layer->parallax = layernode.child("properties").child("property").attribute("value").as_float(0.0f);
	memset(layer->data, 0, sizeof(uint)*layer->tilegid);

	pugi::xml_node layernode1;

	for (layernode1 = layernode.child("data").child("tile"); layernode1; layernode1 = layernode1.next_sibling("tile")) {

		layer->data[i++] = layernode1.attribute("gid").as_uint(0);
		
	}

	/*for (int i = 0; i <= layer->width * layer->height; i++) {				//Comprovate that all layers id are loaded

		LOG("TileGid= %d", layer->data[i]);
	}*/
	
	return ret;
	   	 
}

bool j1Map::LoadObject(pugi::xml_node& objectnode, OBJG* object)
{

	bool ret = true;

	object->name = objectnode.attribute("name").as_string();

	for (pugi::xml_node& obj = objectnode.child("object"); obj && ret; obj = obj.next_sibling("object"))
	{
		MapObject* set = new MapObject;

		set->height = obj.attribute("height").as_uint();
		set->width = obj.attribute("width").as_uint();
		set->x = obj.attribute("x").as_uint();
		set->y = obj.attribute("y").as_uint();
		set->name = obj.attribute("name").as_string();

		object->objects.add(set);
	}

	int i = 0;

	/*for (int i = 0; i <= object->width * object->height; i++) {				//Comprovate that all layers of objects are loaded

		LOG("TileGid= %d", object->set[i]);
	}*/

	return ret;

}

MapLayer::~MapLayer()
{
	delete[] data;
}

OBJG::~OBJG()
{
	objects.clear();
}





