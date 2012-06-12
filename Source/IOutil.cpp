#include "IOutil.h"

//construct our IOutil object and open our input file
IOutil::IOutil(char *file){	
	xmlFile = new XML(file);
	root = xmlFile->GetRootElement();
	elems = root->GetChildren();
	offset = 0;
}

bool IOutil::getElem(char **dest, char *name, XMLElement* elem){

	//a char array in which our element will be stored
	char*temp = new char;

	//create our temp element
	XMLElement** currElems = new XMLElement*;

	//if we're not looking at the root element, currElems is elems children, else it's just elems
	if(elem != root){
		currElems = elem->GetChildren();
	}
	else{
		currElems = elems;
	}

	//find our element
	int x = elem->FindElement(name);
	//if found, extract variable
	if(x!=-1){
		if(vars = currElems[x]->GetContents()){
			vars[0]->GetValue(temp,false);
			*dest = temp;
			return true;
		}
		//error response
		else
			fprintf(stderr, "Error reading element: %s\n", name);
			return false;
	}
	//error response
	else
		fprintf(stderr, "Unable to find element: %s\n", name);
		return false;
}

// get an int element from a given XMLElement*
bool IOutil::getElem(int *dest, char *name, XMLElement* elem){
	char* temp = new char;
	if(getElem(&temp,name,elem)){
		*dest = atoi(temp);
		return true;
	}
	else
		return false;
}
bool IOutil::getElemRoot(int *dest, char *name){
	char* temp = new char;
	if(getElem(&temp,name,root)){
		*dest = atoi(temp);
		return true;
	}
	else
		return false;
}
bool IOutil::getElemRoot(char **dest, char *name){
	char* temp = new char;
	if(getElem(&temp,name,root)){
		*dest = temp;
		return true;
	}
	else
		return false;
}

void IOutil::getTile(Tile *dest, int index){
	
	//create some temporary variables
	int iTemp;
	char *temp = new char;

	//find the index of the first tile
	int tileIdx = root->FindElement("tile");
	
	//set our tile equal to that at the index value, plus the tile index
	XMLElement * tile = elems[tileIdx+index];
	//get our three tile variables, height, tiletype and blocked
	if(getElem(&iTemp,"height",tile)){
		dest->height=iTemp;
	}
	if(getElem(&iTemp,"tiletype",tile)){
		dest->tiletype=iTemp;
	}
	if(getElem(&temp,"blocked",tile)){
		if(!strcmp(temp,"true"))
			dest->blocked=true;
		else
			dest->blocked=false;
	}
}

// getRect is rather simple, it's just a 
void IOutil::getRect(int *dest, int index){
	char *temp = new char;
	int rectIdx = root->FindElement("rect");
	XMLElement * rect = elems[rectIdx+index];
	vars = rect->GetContents();
	vars[0]->GetValue(temp, false);
	*dest = atoi(temp);
}
