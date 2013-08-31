/**
 * Information about tile sizes
 * 
**/

#ifndef __TILES_HPP
#define __TILES_HPP

namespace cppcraft
{
	class Tiles
	{
	public:
		// size of a big tile is exactly N regular tiles
		static const int tilesPerBigtile = 4;
		
		// regular tiles
		int tileSize;
		int tilesX, tilesY;
		
		// big tiles
		int bigSize;
		// number of big tiles on each axis
		int bigTilesX, bigTilesY;
		
		// player model texture size
		int skinSize;
		
		Tiles();
		void init();
	};
	extern Tiles tiles;
}

#endif
