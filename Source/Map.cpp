#include "Map.h"
#include "Tile.h"
#include "IOutil.h"

Map::Map(){
}

// this Map constructor opens a given file and creates a map based on that file
Map::Map(char *mapFile){
	target = mapFile;
	this->loadMap(target);
}

Map::Map(int siz){
	size=siz;
}

void Map::setSize(int siz){
	size=siz;
}

void Map::setTarget(char* file){
	target = file;
	this->loadMap(target);
}

//replace all map data with that supplied via the map file
void Map::loadMap(char* mapFile){

	//first, open the file
	IOutil* xmlIO = new IOutil(mapFile);

	//populate general map information
	xmlIO->getElemRoot(&author,"author");
	xmlIO->getElemRoot(&size,"size");
	xmlIO->getElemRoot(&bkgd,"bkgd");
	xmlIO->getElemRoot(&name,"name");
	xmlIO->getElemRoot(&gametype,"gametype");
	xmlIO->getElemRoot(&rectCount,"rectcount");
	
	//allocate the memory for our tile array.
	tiles = (Tile*)malloc(size* sizeof(Tile));
	if(tiles == NULL){
		fprintf(stderr, "Unable to allocate memory for tiles \n");
		exit(1);
	}

	//create a temporary tile object, and use it to read in our tiles
	Tile* tempTile = new Tile();
	
	for(int i=0; i<size; i++){
		//reset tempTile
		tempTile->blocked="false";
		tempTile->height=0;
		tempTile->tiletype=0;

		//load in new values
		xmlIO->getTile(tempTile,i);
		tiles[i].blocked = tempTile->blocked;
		tiles[i].height = tempTile->height;
		tiles[i].tiletype = tempTile->tiletype;
	}

	//allocate the memory for our rect array.
	rects = (int*)malloc(rectCount* sizeof(int));
	if(rects == NULL){
		fprintf(stderr, "Unable to allocate memory for rects \n");
		exit(1);
	}

	for(int i=0; i<rectCount; i++){
		xmlIO->getRect(&rects[i],i);
	}
}