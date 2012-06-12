#ifndef __MAP_H__
#define __MAP_H__

#include "xml.h"
#include "tile.h"

class Map{

public:

	int size,						// public variables for map size,
		tileHeight,					// a tile's height
		tileWidth,					// a tile's width
		rectCount;					// the number of rects in the BMP

	char* name;						// name of the map
	char* author;					// author of the map
	char* gametype;					// the type of game (name only)
	char* bkgd;						// the bitmap used for the map background
	
	Tile* tiles;					// a pointer to a tile object, which will be used to point to the tile array
	int* rects;						// a pointer to an int, which will be used to point to our rect array

	Map();							// blank constructor
	Map(int siz);					// constructing a map with included size
	Map(char* mapFile);				// constructing a map with an included map file

	void setSize(int siz);			// set size of the map
	void setTarget(char* file);		// set target of the map
	void loadMap(char* mapFile);	// load a map based on a given file

private:
	char* target;					// the target file
};
#endif