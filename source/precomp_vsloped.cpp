#include "precomp_thread.hpp"

#include "blockmodels.hpp"

namespace cppcraft
{
	int PrecompThreadData::emitSloped(block_t id, int bx, int by, int bz, block_t model, block_t facing)
	{
		int vertices = 0;
		
		if (model != 0 && model != 1)
		{
			if ((facing & 1) != 0)
			{
				vertices += blockmodels.slopedLeafs[model].copyTo(0, indic);
				applyFaceLighting_PZ(bx, by, bz, 1);
				indic += 4;
			}
			if ((facing & 2) != 0)
			{
				vertices += blockmodels.slopedLeafs[model].copyTo(1, indic);
				applyFaceLighting_NZ(bx, by, bz, 1);
				indic += 4;
			}
		}
		
		vertices += blockmodels.slopedLeafs[model].copyTo(2, indic);
		applyFaceLighting_PY(bx, by, bz, 1);
		indic += 4;
		vertices += blockmodels.slopedLeafs[model].copyTo(3, indic);
		applyFaceLighting_NY(bx, by, bz, 1);
		indic += 4;
		
		if (model != 2 && model != 3)
		{
			if ((facing & 16) != 0)
			{
				vertices += blockmodels.slopedLeafs[model].copyTo(4, indic);
				applyFaceLighting_PX(bx, by, bz, 1);
				indic += 4;
			}
			if ((facing & 32) != 0)
			{
				vertices += blockmodels.slopedLeafs[model].copyTo(5, indic);
				applyFaceLighting_NX(bx, by, bz, 1);
				indic += 4;
			}
		}
		
		indic -= vertices; // go back to start
		
		short tid = Block::cubeFaceById(id, 0, 0);
		
		for (int i = 0; i < vertices; i++)
		{
			indic[i].w = tid;
			indic[i].biome = fbiome[0];
		}
		
		return vertices;
	}
}
