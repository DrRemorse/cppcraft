#ifndef FLATLANDS_HPP
#define FLATLANDS_HPP

#include "sectors.hpp"

namespace cppcraft
{
	class Compressor;
	
	class FlatlandSector
	{
	public:
		struct flatland_t
		{
			typedef unsigned int flat_color_t;
			
			flat_color_t fcolor[8];
			unsigned char terrain;
			unsigned char skyLevel;
			unsigned char groundLevel;
		};
		
		// returns a reference to flatland_t for the 2D location (x, z)
		flatland_t& operator() (int x, int z);
		
	private:
		flatland_t fdata[Sector::BLOCKS_XZ][Sector::BLOCKS_XZ];
		
		// allow chunk compressor direct access for loading
		friend Compressor;
	};
	
	class FlatlandsContainer
	{
	private:
		FlatlandSector** fsectors;
		
		// used by: Seamless & PrecompThread
		// returns a FlatlandSector* reference from location (x, z)
		inline FlatlandSector* & manipulate(int x, int z)
		{
			return this->fsectors[x * Sectors.getXZ() + z];
		}
		
		friend class Seamless;
		friend class PrecompThread;
		
	public:
		FlatlandSector& operator() (int x, int z);
		
		// initialize container
		void init();
		
		// returns skylevel (in block units) from a local-grid position
		FlatlandSector::flatland_t& getData(int x, int z);
		
		int getTerrain(int x, int z);
		int getSkyLevel(int x, int z);
		int getGroundLevel(int x, int z);
	};
	extern FlatlandsContainer Flatlands;
}

#endif