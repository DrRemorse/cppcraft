#include "tiles.hpp"

#include "library/log.hpp"
#include "library/config.hpp"

using namespace library;

namespace cppcraft
{
	Tiles tiles;
	
	Tiles::Tiles()
	{
		this->bigTilesX = 0;
		this->bigTilesY = 0;
		this->tilesX = 0;
		this->tilesY = 0;
	}
	
	void Tiles::init()
	{
		logger << Log::INFO << "* Initializing tiles" << Log::ENDL;
		
		this->txW = config.get("textures.tilesize", 32);
		this->txH = this->txW;
		
		this->bigW = this->txW * this->tilesPerBigtile;
		this->bigH = this->txH * this->tilesPerBigtile;
		
		this->skinSize = config.get("textures.skinsize", 32);
	}
	
}