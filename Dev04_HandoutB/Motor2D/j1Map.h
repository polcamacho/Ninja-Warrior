#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"
#include "p2Animation.h"

// ----------------------------------------------------
class p2String;

enum LayerTypes
{
	BACKGROUND = 0,
	PARALLAX1,
	PARALLAX2,
	PARALLAX3,
	GROUND,
	DEAD_ZONE,
	LOGIC,
	COLLISIONS,
	
	LAYER_NOT_DEF
};

enum TileSetTypes
{
	PLATFORM = 0,
	FLAG,


	TILESET_NOT_DEF
};

struct Properties
{
	struct Property
	{
		p2SString name;
		int value = 0;
	};

	~Properties()
	{
		p2List_item<Property*>* item;
		item = list.start;

		while (item != NULL)
		{
			RELEASE(item->data);
			item = item->next;
		}

		list.clear();
	}

	int Get(const char* name, int default_value = 0) const;

	p2List<Property*>	list;
};

struct MapLayer {

	p2SString name;
	uint width;
	uint height;
	uint* data = nullptr;
	Properties	properties;

	uint size_data = 0;

	LayerTypes layer_type = LAYER_NOT_DEF;
	~MapLayer() {
		RELEASE(data);
	}

	inline uint Get(int x, int y) const;

};

// ----------------------------------------------------
struct TileSet
{
	
	SDL_Rect GetTileRect(int id) const;

	p2SString			name;
	int					firstgid;
	int					margin;
	int					spacing;
	int					tile_width;
	int					tile_height;
	SDL_Texture*		texture = nullptr;
	int					tex_width;
	int					tex_height;
	int					num_tiles_width;
	int					num_tiles_height;
	int					offset_x;
	int					offset_y;

	TileSetTypes		tileset_type = TILESET_NOT_DEF;


};

enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};
// ----------------------------------------------------
struct MapData
{
	int					width;
	int					height;
	int					tile_width;
	int					tile_height;
	SDL_Color			background_color;
	MapTypes			type;
	
	p2List<TileSet*>	tilesets;
	p2List<MapLayer*>	layers;
	
};

// ----------------------------------------------------
class j1Map : public j1Module
{
public:

	j1Map();

	// Destructor
	virtual ~j1Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* path);

	iPoint MapToWorld(int x, int y) const;
	iPoint WorldToMap(int x, int y) const;


	void setAllLogicForMap();
	void LayersSetUp();

	TileSet* GetTilesetFromTileId(int id) const;
	bool CreateWalkabilityMap(int& width, int& height, uchar** buffer) const;

private:

	bool LoadMap(); //Load the map
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set); //Load all details of the tilset
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set); //Load the image of the tileset
	
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer); //Load the layer
	bool LoadProperties(pugi::xml_node& node, Properties& properties);
		
	
public:


	MapData data;
	p2SString sceneName;
	iPoint spawn = iPoint({ -1, -1 });
	MapLayer* collisionLayer = nullptr;

private:

	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;
	p2SString			spritesheetName;
};

#endif // __j1MAP_H__