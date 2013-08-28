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
		int txW, txH;
		int tilesX, tilesY;
		int skinSize;
		
		// size of a big tile is exactly N regular tiles
		static const int tilesPerBigtile = 4;
		// dimensions of big tiles
		int bigW, bigH;
		// number of big tiles on each axis
		int bigTilesX, bigTilesY;
		
		Tiles();
		void init();
	};
	extern Tiles tiles;
}

#endif
