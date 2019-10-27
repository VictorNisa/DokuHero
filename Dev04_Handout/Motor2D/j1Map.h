#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"

// TODO 1: Create a struct for the map layer
// ----------------------------------------------------
struct map_layer
{
	p2SString name;
	uint width;
	uint height;
	uint* tiled_gid;
	// TODO 6: Short function to get the value of x,y
	inline uint Get(int x, int y) const;
	float parallaxspeed;
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
	SDL_Rect TileRect(uint tiled_gid);
};

struct object
{
	p2SString name;
	uint id;
	SDL_Rect rect;
};

struct objectgroup
{
	p2SString name;
	uint num_objects;
	object* objects;
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
	int						width;
	int						height;
	int						tile_width;
	int						tile_height;
	float					gravity;
	SDL_Color				background_color;
	MapTypes				type;
	p2List<TileSet*>		tilesets;
	// TODO 2: Add a list/array of layers to the map!
	p2List<map_layer*>		layer;
	p2List<objectgroup*>	objectgroup;
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
	int trans_x(int x);
	int trans_y(int y);

private:

	bool LoadMap();
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set, j1Module*module);
	// TODO 3: Create a method that loads a single layer
	bool LoadLayer(pugi::xml_node& tileset_node, map_layer* layer);
	bool add_map_colliders();
	TileSet* GetTilesetFromID(int id) const;


public:

	MapData data;

private:

	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;
	float gravity;
	friend class j1Chara;
};

#endif // __j1MAP_H__