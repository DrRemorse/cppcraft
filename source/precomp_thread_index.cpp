#include "precomp_thread.hpp"

#include <library/log.hpp>
#include "precompiler.hpp"
#include "vertex_block.hpp"
#include <cstring>

using namespace library;

namespace cppcraft
{
	void PrecompThread::createIndices(Precomp& precomp, int totalVertices)
	{
		// create some index bullshit
		delete[] precomp.indidump;
		precomp.indidump = new indice_t[(totalVertices / 4) * 6];
		
		indice_t* indices = precomp.indidump;
		
		for (int i = 0; i < RenderConst::MAX_UNIQUE_SHADERS; i++)
		{
			for (int vert = 0; vert < precomp.vertices[i]; vert += 4)
			{
				indices[0] = vert + 0;
				indices[1] = vert + 1;
				indices[2] = vert + 2;
				
				indices[3] = vert + 2;
				indices[4] = vert + 3;
				indices[5] = vert + 0;
				indices += 6;
			}
			
			// MUST be set, even if 0
			int indexCount = (precomp.vertices[i] / 4) * 6;
			precomp.indices[i] = indexCount;
		}
		
	} // createIndices()
	
}
