#include "precomp_thread.hpp"

#include "blockmodels.hpp"

namespace cppcraft
{
	int PrecompThreadData::emitSloped(block_t id, int bx, int by, int bz, block_t model)
	{
		int vertices = blockmodels.slopedLeafs.copyTo(model, indic);
		
		if (model != 0)
		{
			applyFaceLighting_PZ(bx, by, bz, 1);
			indic += 4;
		}
		if (model != 1)
		{
			applyFaceLighting_NZ(bx, by, bz, 1);
			indic += 4;
		}
		
		applyFaceLighting_PY(bx, by, bz, 1);
		indic += 4;
		applyFaceLighting_NY(bx, by, bz, 1);
		indic += 4;
		
		if (model != 2)
		{
			applyFaceLighting_PX(bx, by, bz, 1);
			indic += 4;
		}
		if (model != 3)
		{
			applyFaceLighting_NX(bx, by, bz, 1);
			indic += 4;
		}
		
		// there is always exactly 20 vertices in sloped leafs
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
