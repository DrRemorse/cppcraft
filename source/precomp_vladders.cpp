#include "precomp_thread.hpp"

#include "blockmodels.hpp"

namespace cppcraft
{
	int PrecompThreadData::emitLadder(block_t id, int bx, int by, int bz, block_t block_facing)
	{
		blockmodels.ladders.copyTo(block_facing, indic);
		
		switch (block_facing)
		{
		case 0:
			applyFaceLighting_PZ(bx, by, bz);
			indic[0].biome = fbiome[0]; // (0, 0)
			indic[1].biome = fbiome[1]; // (1, 0)
			indic[2].biome = fbiome[1]; // (1, 0)
			indic[3].biome = fbiome[0]; // (0, 0)
			break;
		case 1:
			applyFaceLighting_NZ(bx, by, bz);
			indic[0].biome = fbiome[2]; // (0, 1)
			indic[1].biome = fbiome[2]; //
			indic[2].biome = fbiome[3]; // (1, 1)
			indic[3].biome = fbiome[3]; //
			break;
		case 2:
			applyFaceLighting_PX(bx, by, bz);
			indic[0].biome = fbiome[0]; // (0, 0)
			indic[1].biome = fbiome[0]; //
			indic[2].biome = fbiome[2]; // (0, 1)
			indic[3].biome = fbiome[2]; //
			break;
		case 3:
			applyFaceLighting_NX(bx, by, bz);
			indic[0].biome = fbiome[1]; // (1, 0)
			indic[1].biome = fbiome[3]; // (1, 1)
			indic[2].biome = fbiome[3]; // (1, 1)
			indic[3].biome = fbiome[1]; // (1, 0)
			break;
		}
		
		short tid = Block::cubeFaceById(id, 0, 0);
		indic[0].w = tid;
		indic[1].w = tid;
		indic[2].w = tid;
		indic[3].w = tid;
		
		return 4; //blockmodels.ladders.count(block_facing);
	}
}
