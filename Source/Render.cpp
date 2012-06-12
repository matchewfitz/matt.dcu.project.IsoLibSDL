#include "Render.h"
#include "SDL.h"
#include "Map.h"

#define PITCH (gScreen->pitch / 4)

// Screen surface
SDL_Surface *gScreen;

// Surface that contains the tiles
SDL_Surface *gTiles,*gText;

// Tick value
int gLastTick;

// Global Map File
Map gMap;

// The set of Rects that define each tile
SDL_Rect tileRects[13];

// constructor, takes a char specifying path to XML file
Render::Render(char* file){
	target = file;
}

// drawRect function, taken from Sol's SDL tutorial http://sol.gfxile.net/gp/
void Render::drawRect(int x, int y, int width, int height, int c)
{
  int i, j;
  for (i = 0; i < height; i++)
  {
    // vertical clipping: (top and bottom)
    if ((y + i) >= 0 && (y + i) < HEIGHT)
    {
      int len = width;
      int xofs = x;

      // left border
      if (xofs < 0)
      {
        len += xofs;
        xofs = 0;
      }

      // right border
      if (xofs + len >= WIDTH)
      {
        len -= (xofs + len) - WIDTH;
      }
      int ofs = (i + y) * PITCH + xofs;

      // note that len may be 0 at this point, 
      // and no pixels get drawn!
      for (j = 0; j < len; j++)
        ((unsigned int*)gScreen->pixels)[ofs + j] = c;
    }
  }
}

// drawTile function, takes the x and y position, and the current tile as args
void Render::drawTile(int x, int y, int tile)
{
	// Lock surface if needed
	if (SDL_MUSTLOCK(gTiles))
		if (SDL_LockSurface(gTiles) < 0) 
			return;

	//take into account previous tiles in calculating x and y
	y += gHeightOffs*(gMap.tiles[tile].height*-1);
	SDL_Rect rcSprite;
	rcSprite.x = x;
	rcSprite.y = y;

	//draw the tile in the appropriate place
	SDL_BlitSurface(gTiles, &tileRects[gMap.tiles[tile].tiletype], gScreen, &rcSprite);

	// Unlock surface
	if (SDL_MUSTLOCK(gTiles)) 
		SDL_UnlockSurface(gTiles);
}

//reset is currently very simple, it retrieves a new tick value
void Render::reset()
{
	//get new tick value
    gLastTick = SDL_GetTicks(); 
}

// setTiles is a function that sets the x, y, width and height properties of a given tile rect
void Render::setTiles(int index, int x, int y, int w, int h){
	tileRects[index].x = x;
	tileRects[index].y = y;
	tileRects[index].w = w;
	tileRects[index].h = h;
}

// initTiles calls setTiles for each tile type
void Render::initTiles(){
	int yOffs=0;
	int rectSizes[2] = {gMap.tileHeight,((gMap.tileHeight+1)/2) + (gMap.tileHeight-2)};
	for(int i=0; i<13; i++){
		setTiles(i,0,yOffs, gMap.tileWidth,rectSizes[(gMap.rects[i]-1)]);
		yOffs += rectSizes[(gMap.rects[i]-1)];
	}
}

/*init does quite a bit, it initializes the FPS count, calls to load the map, 
**loads the bitmap and initializes the transparent colour*/
void Render::init()
{
	//init FPS count
	FPS = 0;
	pastFPS = 0;
	past =0;

	//set map variables and load the map
	gMap.setTarget(target);

	//load the tile bitmap
	SDL_Surface *tempSurface = SDL_LoadBMP(gMap.bkgd);
	gTiles = SDL_ConvertSurface(tempSurface, gScreen->format, SDL_SWSURFACE);	
	SDL_FreeSurface(tempSurface);

	//get tileWidth from our bitmap and instance all associated variables
	gMap.tileWidth = gTiles->w;
	gMap.tileHeight = (gTiles->w -2)/2;
	gMapCols = (int)sqrt((double)gMap.size);
	gHeightOffs = (gMap.tileHeight-3)/2;
	initTiles();
	//set pink as the transparent colour
	Uint32 colorkey = SDL_MapRGB( gTiles->format, 255, 0, 255);

	// now tell SDL to remeber our choice
	SDL_SetColorKey( gTiles, SDL_SRCCOLORKEY, colorkey);

	reset();
}

// deinit is quite simple, it performs any garbage collection that isn't done automatically
void Render::deinit()
{
	// free Tiles from memory
	free(gMap.tiles);
	gMap.tiles = NULL;
	// free Rects from memory
	free(gMap.rects);
	gMap.rects = NULL;
}

// doFPS places an FPS count on the title bar
void Render::doFPS(int tick){
	if (tick <= gLastTick) 
	{
		SDL_Delay(1);
		return;
	}
	if ( tick - past >= 16 ) 
	{ 
		past = SDL_GetTicks(); 

		SDL_Flip( gScreen ); 
		FPS++; 
	} 
	if ( tick - pastFPS >= 1000 ) 
	{ 
		static char buffer[20] = {0}; 
		sprintf( buffer, "%d FPS", FPS ); 
		SDL_WM_SetCaption( buffer,0 ); 

		FPS = 0; 
		pastFPS = tick; 
	}
}
// doRender is a function that draws 
void Render::doRender()
{   
	// Ask SDL for the time in milliseconds
	int tick = SDL_GetTicks();	
	if(gDebug){
		doFPS(tick);
	}

	// Lock surface if needed
	if (SDL_MUSTLOCK(gScreen))
		if (SDL_LockSurface(gScreen) < 0) 
			return;
	
	// Initialize our start points and offsets for x and y
	gStartPtX = ((gMapCols-1)*gMap.tileWidth)/2;
	gStartPtY = 0;
	gOffsX = 0;
	gOffsY = 0;
	int tileOffs = 0;

	// draw tiles based on offsets
	int i, j;
	for (i = 0; i < gMapCols; i++){
		for (j = 0; j < gMapCols; j++){
			//draw tile at x and y taking into account the offset
			drawTile(gStartPtX + gOffsX,(gStartPtY + gOffsY), tileOffs+j);
			
			//increment offsets for x and y
			gOffsX += gMap.tileWidth/2;
			gOffsY +=(gMap.tileHeight+1)/2;
		}
		//reset offsets in x and y to zero
		gOffsX = 0;
		gOffsY = 0;

		//increment tile offset
		tileOffs +=gMapCols;
		
		//recalculate our start point
		gStartPtX -=gMap.tileWidth/2;
		gStartPtY +=(gMap.tileHeight+1)/2;
	}

	// Unlock screen
	if (SDL_MUSTLOCK(gScreen)) 
		SDL_UnlockSurface(gScreen);

	// Tell SDL to update gScreen
	SDL_UpdateRect(gScreen, 0, 0, WIDTH, HEIGHT);   
}

int Render::callMe(bool debug){
	
	//set global Debug variable
	gDebug = debug;

	// Initialize SDL's subsystems
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) 
	{
		fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
		exit(1);
	}

	/* Register SDL_Quit to be called at exit; makes sure things are
	cleaned up when we quit.*/
	atexit(SDL_Quit);

	// Attempt to create a WIDTHxHEIGHT window with 32bit pixels.
	SDL_Rect clip = {0,0,WIDTH,HEIGHT};
	gScreen = SDL_SetVideoMode(WIDTH, HEIGHT, 32, SDL_SWSURFACE);

	//initialize the clipping area for the screen
	gScreen->clip_rect = clip;
	Render::init();
	
	// If gScreen isn't able to init, return error.
	if (gScreen == NULL) 
	{
		fprintf(stderr, "Unable to set up video: %s\n", SDL_GetError());
		exit(1);
	}

	// Main loop: loop until user calls quit
	while (1)
	{
		// Render the actual map
		Render::doRender();

		// Poll for keyboard events, and handle the ones we care about.
		SDL_Event event;
		while (SDL_PollEvent(&event)) 
		{
			switch (event.type) 
			{
				case SDL_KEYUP:          
				switch (event.key.keysym.sym)
				{
					case SDLK_ESCAPE:
					// If escape is pressed, return (and thus, quit)
					deinit();
					return 0;
				}
				break;
				case SDL_QUIT:
				deinit();
				return 0;
			}
		}
	}
	deinit();
	return 0;
}