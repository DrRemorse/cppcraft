#include "precomp_thread.hpp"
#include "precompiler.hpp"
#include "vertex_block.hpp"

namespace cppcraft
{
	void PrecompThread::createIndices(Precomp& precomp, int verts)
	{
		// create some index bullshit
		delete[] precomp.indidump;
		precomp.indidump = new indice_t[verts];
		
		indice_t* indices = precomp.indidump;
		indice_t baseIndex = 0;
		
		for (int i = 0; i < RenderConst::MAX_UNIQUE_SHADERS; i++)
		{
			for (int vert = 0; vert < precomp.vertices[i]; vert++)
			{
				indices[vert] = vert;
			}
			indices += precomp.vertices[i];
			
			precomp.indexoffset[i] = baseIndex;
			precomp.indices[i] = precomp.vertices[i];
			baseIndex += precomp.indices[i];
		}
	}
	
}
