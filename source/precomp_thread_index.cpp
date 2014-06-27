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
		precomp.indidump = new indice_t[totalVertices];
		
		indice_t* indices = precomp.indidump;
		
		for (int i = 0; i < RenderConst::MAX_UNIQUE_SHADERS; i++)
		{
			int indexCount = precomp.vertices[i];
			
			if (indexCount)
			{
				if (i != RenderConst::TX_CROSS && i != RenderConst::TX_2SIDED)
				{
					vertex_t* source = precomp.datadump + precomp.bufferoffset[i];
					
					// set the indices for the 4 first vertices
					for (int vert = 0; vert < 4; vert++)
					{
						indices[vert] = vert;
						source[vert].face = vert;
					}
					indices += 4;
					vertex_t* current = source + 4;
					
					for (int vert = 4; vert < precomp.vertices[i]; vert++)
					{
						// no index has been set yet
						bool skip = false;
						
						// find any previous vertices that matches current
						for (vertex_t* prev = current-1; prev >= source; prev--)
						{
							// same texture tile id
							if (current->w == prev->w)
							// same normal
							if (current->nx == prev->nx && current->ny == prev->ny && current->nz == prev->nz)
							// same UVs
							if (current->u == prev->u && current->v == prev->v)
							// same position
							if (current->x == prev->x && 
								current->y == prev->y && 
								current->z == prev->z)
							{
								// use existing vertex
								indices[0] = prev->face;
								indices++;
								// prevent vertex from being used again
								current->w = 32767;
								current++;
								// remove vertex
								/*for (int i = vert+1; i < precomp.vertices[i]; i++)
								{
									memcpy(source+i-1, source+i, sizeof(vertex_t));
								}
								precomp.vertices[i]--;
								vert--;*/
								skip = true; break;
							}
						}
						// emit index for unset vertex
						if (skip == false)
						{
							current->face = vert;
							current++;
							indices[0] = vert;
							indices++;
						}
					}
				} // supported shader path
				else
				{
					// direct vertex->index mapping
					for (int vert = 0; vert < precomp.vertices[i]; vert++)
						indices[vert] = vert;
					
					indices += indexCount;
				}
				
			} // vertices != 0
			
			// MUST be set, even if 0
			precomp.indices[i] = indexCount;
		}
		
	} // createIndices()
	
}
