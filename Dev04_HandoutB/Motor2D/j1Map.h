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
	uint* tilegid;
	inline uint Get(uint x, uint y) const {

		return y * width + x;
	
	}

};
	// TODO 6: Short function to get the value of x,y



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

	SDL_Rect tile_id(int) const;

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

	p2Point<uint> GetTilePos(uint wx, uint wy) const
	{
		p2Point<uint> ret;

		ret.x = wx / tile_width;

		ret.y = wy / tile_height;

		return ret;
	}

	

	
	// TODO 2: Add a list/array of layers to the map!
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
	// TODO 3: Create a method that loads a single laye
	
	
	
	
	
public:

	MapData data;


private:

	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;
	uint idtile = 0;

	
	
	
};

#endif // __j1MAP_H__