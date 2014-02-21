#ifndef FLATLANDS_HPP
#define FLATLANDS_HPP

#include <library/opengl/texture.hpp>
#include "sectors.hpp"
#include <deque>

namespace cppcraft
{
	class FlatlandSector
	{
	public:
		static const int FLATCOLORS = 8;
		
		struct flatland_t
		{
			typedef unsigned int flat_color_t;
			
			flat_color_t fcolor[FLATCOLORS];
			unsigned char terrain;
			unsigned char skyLevel;
			unsigned char groundLevel;
		};
		
		// returns a reference to flatland_t for the 2D location (x, z)
		flatland_t& operator() (int x, int z);
		
		inline bool mustRebuild() const { return rebuild; }
		
	private:
		flatland_t fdata[Sector::BLOCKS_XZ][Sector::BLOCKS_XZ];
		bool rebuild;
		
		// allow chunk compressor direct access for loading
		friend class FlatlandsContainer;
		friend class Compressor;
		friend class Generator;
	};
	
	class FlatlandsContainer
	{
	public:
		~FlatlandsContainer();
		
		// returns a FlatlandSector reference from location (x, z)
		inline FlatlandSector& operator() (int x, int z)
		{
			return *this->fsectors[x * Sectors.getXZ() + z];
		}
		
		// initialize flatlands axis (using sectors axis), called from World::init()
		void init();
		
		// returns skylevel (in block units) from a local-grid position
		FlatlandSector::flatland_t& getData(int x, int z);
		
		inline int getTerrain(int x, int z)
		{
			return getData(x, z).terrain;
		}
		inline int getSkyLevel(int x, int z)
		{
			return getData(x, z).skyLevel;
		}
		inline int getGroundLevel(int x, int z)
		{
			return getData(x, z).groundLevel;
		}
		
		// interact with biome texture queue
		void initTextures();
		void buildTexture(int fx, int fz);
		void bindTexture(int unit);
		void uploadTexture();
		
		inline bool mustUpload() const { return upload; }
		
	private:
		FlatlandSector** fsectors;
		library::Texture biomeTexture;
		unsigned int*    biomeArray;
		bool upload;
		
		// used by: Seamless
		// returns a FlatlandSector* reference from location (x, z)
		inline FlatlandSector* & manipulate(int x, int z)
		{
			return this->fsectors[x * Sectors.getXZ() + z];
		}
		
		friend class Compressor;
		friend class Seamless;
		friend class PrecompThread;
	};
	extern FlatlandsContainer flatlands;
}

#endif
