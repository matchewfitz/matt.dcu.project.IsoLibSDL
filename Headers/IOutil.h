#ifndef __IOUTIL_H__
#define __IOUTIL_H__

#include "xml.h"
#include "Tile.h"

class IOutil{

public:
	IOutil(char* file);											//constructor, taking a file path as argument

	bool getElemRoot(char** dest, char* name);					//get char element from root element based on name
	bool getElemRoot(int* dest, char* name);					//get int element from root element based on name
	bool getElem(char** dest, char* name, XMLElement* elem);	//get char element from a given element based on name
	bool getElem(int* dest, char* name, XMLElement* elem);		//get int element from a given element based on name
	void getTile(Tile* dest, int index);						//get a tile object at given index
	void getRect(int *dest, int index);							//get a rect value at given index
	bool getMap();												//get a complete map
	
	int offset;				//element offset
	XML* xmlFile;			//the main XML file object
	XMLElement* root;		//the root element
	XMLElement** elems;		//our root elements list
	XMLContent** vars;		//the variables contained within a given element

private:
	bool loadElem(char* dest,bool noDecode,XMLElement* elem);	//load a given element into a destination variable
	bool loadElem(int* dest, bool noDecode,XMLElement* elem);	
};
#endif