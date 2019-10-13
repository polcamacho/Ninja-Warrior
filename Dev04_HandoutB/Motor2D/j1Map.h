#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"

// TODO 1: Create a struct for the map layer
// ----------------------------------------------------
struct MapLayer {

	p2SString name;
	uint width;
	uint height;
	uint *tilegid1;
	uint tilegid;
	float parallax;
	uint* data = nullptr;
	inline uint Get(uint x, uint y) const {

		return (y * width) + x;
	
	}

	inline uint Get1(uint x, uint y) const {

		return (y * width) + x;

	}
	~MapLayer();

};
	// TODO 6: Short function to get the value of x,y

struct MapObject
{
	p2SString	name;
	int			x;
	int			y;
	uint		width;
	uint		height;

};

struct OBJG
{
	p2SString				name;
	p2List<MapObject*>	objects;
	~OBJG();
};

// ----------------------------------------------------
struct TileSet
{
	// TODO 7: Create a method that receives a tile id and returns it's Rect

	p2SString			name;
	int					firstgid;
	int					margin;
	int					spacing;
	int					tile_width;
	int					tile_height;
	SDL_Texture*		texture;
	int					tex_width;
	int					tex_height;
	int					num_tiles_width;
	int					num_tiles_height;
	int					offset_x;
	int					offset_y;

	SDL_Rect tile_id(int id) const;

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
	p2List<OBJG*>	objects;

	p2Point<uint> GetTilePos(uint wx, uint wy) const
	{
		p2Point<uint> ret;

		ret.x = wx / tile_width;

		ret.y = wy / tile_height;

		return ret;
	}

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

	

	// TODO 8: Create a method that translates x,y coordinates from map positions to world positions
	iPoint GetWorldPos(int x, int y) const; //Gets the position in the world

private:

	bool LoadMap(); //Load the map
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set); //Load all details of the tilset
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set); //Load the image of the tileset
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer); //Load the layer
	bool LoadObject(pugi::xml_node& objectnode, OBJG* object);
	
public:

	MapData data;
	int Id = 0;

private:

	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;
	uint idtile = 0;
	SDL_Texture* texture;
	
	
	
};

#endif // __j1MAP_H__