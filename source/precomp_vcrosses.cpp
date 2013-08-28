#include "precomp_thread.hpp"

#include "blockmodels.hpp"
#include "lighting.hpp"

namespace cppcraft
{
	int PrecompThreadData::emitCross(block_t id, int bx, int by, int bz)
	{
		// copy cross-mesh object 0 (cross)
		int vertices = blockmodels.crosses.copyTo(0, indic);
		
		// huge boring list of cross-lighting
		indic[0].c = ldata.tableLight(*sector, bx  , by  , bz  ); //, 2, 2
		indic[1].c = ldata.tableLight(*sector, bx+1, by  , bz+1); //, 7, 2
		indic[2].c = ldata.tableLight(*sector, bx+1, by+1, bz+1); //, 7, 2
		indic[3].c = ldata.tableLight(*sector, bx  , by+1, bz  ); //, 2, 2
		indic[4].c = ldata.tableLight(*sector, bx+1, by  , bz  ); //, 5, 2
		indic[5].c = ldata.tableLight(*sector, bx+1, by+1, bz  ); //, 5, 2
		indic[6].c = ldata.tableLight(*sector, bx  , by+1, bz+1); //, 4, 2
		indic[7].c = ldata.tableLight(*sector, bx  , by  , bz+1); //, 4, 2
		
		indic[0].biome = fbiome[0]; // (0, 0)
		indic[3].biome = fbiome[0];
		
		indic[4].biome = fbiome[1]; // (1, 0)
		indic[5].biome = fbiome[1];
		
		indic[6].biome = fbiome[2]; // (0, 1)
		indic[7].biome = fbiome[2];
		
		indic[1].biome = fbiome[3]; // (1, 1)
		indic[2].biome = fbiome[3];
		
		// set first vertex tile id
		indic->w = Block::cubeFaceById(id, 0, 0);
		// set reset
		for (int i = 1; i < vertices; i++)
			indic[i].w = indic->w;
		
		return vertices;
	}
}
