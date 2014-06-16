#include "biome.hpp"

#include <library/bitmap/colortools.hpp>
#include "blocks.hpp"

using namespace library;

namespace cppcraft
{
	static int biomeColors16[16][3] =
	{
		{ 20,  12,   28 },
		{ 68,  36,   52 },
		{ 48,  52,  109 },
		{ 78,  74,   78 },
		{ 133, 76,   48 },
		{ 52,  101,  36 },
		{ 208, 70,   72 },
		{ 117, 113,  97 },
		{ 89,  125, 206 },
		{ 210, 125,  44 },
		{ 133, 149, 161 },
		{ 109, 170,  44 },
		{ 210, 170, 153 },
		{ 109, 194, 202 },
		{ 218, 212,  94 },
		{ 222, 238, 214 }
	};
	
	int Biomes::idToColorIndex(unsigned short id)
	{
		if (isStone(id))
		{
			return Biomes::CL_STONE;
		}
		else if (isDirt(id) || id == _LOWSNOW)
		{
			return Biomes::CL_GRASS;
		}
		else if (isLeaf(id) || id == _VINES)
		{
			return Biomes::CL_TREES;
		}
		else if (isCross(id))
		{
			return Biomes::CL_GRASS;
		}
		else if (id == _DESERTSAND)
		{
			return Biomes::CL_SAND;
		}
		else if (id >= _GRAVEL1 && id <= SAND_END)
		{
			return Biomes::CL_GRAVEL;
		}
		/*else if (id == _WATER)
		{
			return Biomes::CL_WATER;
		}*/
		return -1;
	}
	
	Biomes::biomecolor_t Biomes::getSpecialColorBGRA(int clid)
	{
		return BGRA8(biomeColors16[clid][0], biomeColors16[clid][1], biomeColors16[clid][2], 255);
	}
	
	Biomes::biomecolor_t Biomes::getSpecialColorRGBA(int clid)
	{
		return RGBA8(biomeColors16[clid][0], biomeColors16[clid][1], biomeColors16[clid][2], 255);
	}
	
}