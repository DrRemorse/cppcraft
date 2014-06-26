#include "precomp_thread.hpp"

#include <library/log.hpp>
#include "precompiler.hpp"
#include "vertex_block.hpp"
#include <cstring>

using namespace library;

namespace cppcraft
{
	void PrecompThread::createIndices(Precomp& precomp, int vertexCount)
	{
		// create some index bullshit
		delete[] precomp.indidump;
		precomp.indidump = new indice_t[vertexCount];
		
		static const unsigned short MAX_INDEX_VALUE = 65535;
		indice_t* indices = precomp.indidump;
		indice_t currentIndex = 0;
		indice_t baseIndex = 0;
		
		for (int i = 0; i < RenderConst::MAX_UNIQUE_SHADERS; i++)
		{
			if (precomp.vertices[i] != 0)
			{
				vertex_t* source = precomp.datadump + precomp.bufferoffset[i];
				
				if (i != RenderConst::TX_CROSS)
				{
					// set the indices for the 4 first vertices
					for (int vert = 0; vert < 4; vert++)
					{
						indices[currentIndex] = vert;
						source[vert].face = vert;
						currentIndex++;
					}
					
					for (int vert = 4; vert < precomp.vertices[i]; vert++)
					{
						vertex_t* current = source + vert;
						
						// set face to MAX_INDEX_VALUE, signalling no index has been set
						current[0].face = MAX_INDEX_VALUE;
						
						// find any previous vertices that matches current
						for (vertex_t* prev = source; prev < current; prev++)
						{
							// current has same texture as its next
							if (current->w == prev->w)
							// same normal
							if (current->nx == prev->nx && current->ny == prev->ny && current->nz == prev->nz)
							{
								// find matching vertices
								if (current[0].x == prev[0].x && 
									current[0].y == prev[0].y && 
									current[0].z == prev[0].z)
								{
									// use existing vertex
									current[0].face = prev[0].face;
									indices[currentIndex] = prev[0].face;
									currentIndex++;
									break;
								}
							} // same normal
						}
						
						// emit index for unset vertex
						if (current[0].face == MAX_INDEX_VALUE)
						{
							indices[currentIndex] = vert;
							current[0].face = vert;
							currentIndex++;
						}
					}
				} // supported shader path
				else
				{
					// direct mapping
					for (int vert = 0; vert < precomp.vertices[i]; vert++)
					{
						indices[currentIndex] = vert;
						currentIndex++;
					}
				}
			} // vertices != 0
			
			precomp.indices[i] = precomp.vertices[i];
			precomp.indexoffset[i] = baseIndex;
			baseIndex = currentIndex;
		}
		
	} // createIndices()
	
}
