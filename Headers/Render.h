#ifndef __RENDER_H__
#define __RENDER_H__

#include <stdlib.h>
#include <string.h>
#include <math.h>

class Render{

// Screen width
#define WIDTH 1024
// Screen height
#define HEIGHT 768

public:

	Render(char* file);
	
	void drawRect(int x, int y, int width, int height, int c);
	void drawTile(int x, int y, int tile);
	void doFPS(int tick);
	void doRender();
	void reset();
	void init();
	void initTiles();
	void setTiles(int index, int x, int y, int w, int h);
	void deinit();
	void render();
	int callMe(bool debug);

	bool gDebug;		//whether initialized to debug or not
	char* target,		//location of XML file
		debugInfo;		
	int gMapCols,		//number of cols in a map (taken from file)
		gMapRows,		//number of rows in a map (taken from file)
		gStartPtX,		//where the first tiles are drawn
		gStartPtY,
		gTileWid,		//width of a tile (taken from file)
		gTileHt,		//height of a tile (taken from file)
		gOffsX,			//the current x offset
		gOffsY,			//the current y offset
		gHeightOffs,	//the height offset
		FPS,
		pastFPS,
		past;
};

#endif