#ifndef __TILE_H__
#define __TILE_H__

class Tile{
public:
	int height,
		tiletype;
	bool blocked;

	Tile();
	~Tile();

	void setSize(int tileSize);
};
#endif