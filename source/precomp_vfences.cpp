#include "precomp_thread.hpp"

#include "blockmodels.hpp"

namespace cppcraft
{
	int PrecompThreadData::emitFence(const Block& currentBlock, int bx, int by, int bz, block_t visibility)
	{
		int model = currentBlock.getFacing();
		model = (model < 2) ? 0 : 1;
		
		int vertices = blockmodels.fences.copyTo(model, indic);
		
		// apply lighting
		for (int i = 0; i < 4; i++)
		{
			applyFaceLightingAll(bx, by, bz);
			indic += 6 * 4;
		}
		indic -= vertices;
		
		// apply texture tile id
		short tid = Block::cubeFaceById(currentBlock.getID(), 0, 0);
		for (int i = 0; i < vertices; i++)
		{
			indic[i].w = tid;
		}
		
		return vertices;
	}
}
