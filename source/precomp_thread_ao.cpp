#include "precomp_thread.hpp"

#include "library/log.hpp"
#include "library/storage/bitarray.hpp"

#include "blockmodels.hpp"
#include "lighting.hpp"
#include "precompiler.hpp"
#include "sector.hpp"
#include "spiders.hpp"
#include "renderconst.hpp"
#include <cstring>

using namespace library;

namespace cppcraft
{
	class AmbientOcclusion
	{
	public:
		static const int CRASH_MAX_POINTS = 2400;
		static const int POINT_ELEMENTS = CRASH_MAX_POINTS * 8 * 6;
		
		// number of corner points set (must be cleared)
		int count;
		
		// lookup table (must be cleared)
		short t_lookup[Sector::BLOCKS_XZ+1]
					  [Sector::BLOCKS_Y +1]
					  [Sector::BLOCKS_XZ+1];
		// bit array for defining which faces are connected to each point (must be cleared)
		BitArray pointData;
		// corner shadow value (does not need to be cleared)
		unsigned char t_corner[CRASH_MAX_POINTS];
		// test data (does not need to be cleared)
		visiblefaces_t testdata;
		
		AmbientOcclusion()
		{
			pointData = BitArray(POINT_ELEMENTS);
		}
		
		// get/set point bits
		void setPoint(int index, int corner, int face)
		{
			pointData.set((index * 8 + corner) * 6 + face);
		}
		bool getPoint(int index, int corner, int face) const
		{
			return pointData[(index * 8 + corner) * 6 + face];
		}
		
		void clear()
		{
			count = 0;
			// clear point bitarray
			pointData.clear();
			// clear lookup table
			memset(t_lookup, 0, sizeof(t_lookup));
		}
		
	};
	
	void PrecompThread::ambientOcclusion()
	{
		Sector& sector = *precomp->sector;
		
		// recount total vertices
		int cnt = precomp->vertices[0];
		for (int i = 1; i < RenderConst::MAX_UNIQUE_SHADERS; i++)
		{
			cnt += precomp->vertices[i];
		}
		
		if (sector.progress != Sector::PROG_AO)
		{
			logger << Log::ERR << "PrecompThread::ambientOcclusion(): invalid sector progress state" << Log::ENDL;
			logger << Log::ERR << "PrecompThread::ambientOcclusion(): state: " << (int) sector.progress << Log::ENDL;
			cancelPrecomp();
			return;
		}
		else if (precomp->datadump == nullptr)
		{
			logger << Log::ERR << "PrecompThread::ambientOcclusion(): datadump was null" << Log::ENDL;
			cancelPrecomp();
			return;
		}
		
		// ambient occlusion processing stage
		#ifdef AMBIENT_OCCLUSION_GRADIENTS
			if (this->occ == nullptr)
			{
				this->occ = new AmbientOcclusion();
			}
			ambientOcclusionGradients(this->occ, precomp->datadump, cnt);
		#endif
		
		// optimize mesh
		/*
		int waterShaderline = (int)RenderConst::MAX_UNIQUE_SHADERS-1;
		if (precomp->vertices[waterShaderline] == 1024)
		{
			vertex_t* water = precomp->datadump + precomp->bufferoffset[waterShaderline];
			
			water[0].x = 0;
			water[0].z = 0;
			
			water[1].x = 0;
			water[1].z = RenderConst::VERTEX_SCALE * 16;
			
			water[2].x = RenderConst::VERTEX_SCALE * 16;
			water[2].z = RenderConst::VERTEX_SCALE * 16;
			
			water[3].x = RenderConst::VERTEX_SCALE * 16;
			water[3].z = 0;
			
			precomp->vertices[waterShaderline] = 4;
		}*/
		
		sector.progress = Sector::PROG_NEEDCOMPILE;
		sector.culled  = false;
	}
	
	short addCornerShadowVertex(AmbientOcclusion& ao, vertex_t* vt, short x, short y, short z)
	{
		short corner = vt->face >> 3;
		corner =(corner & 1) + // 0 if left,  1 if right
				(corner & 2) + // 0 if lower, 2 if upper
				(corner & 4);  // 0 if back,  4 if front
		
		short face = (vt->face & 7) - 1;
		short index;
		
		if (ao.t_lookup[x][y][z] == 0)
		{
			// if the lookup wasn't set before, set it now
			index = ++ao.count;
			ao.t_lookup[x][y][z] = index;
		}
		else
		{
			// otherwise, use previous index
			index = ao.t_lookup[x][y][z];
		}
		
		// add this vertex information
		ao.setPoint(index, corner, face);
		return index;
	}
	
	void runCornerShadowTest(AmbientOcclusion& ao, int facing, short baseX, short baseY, short baseZ)
	{
		// this block was visible
		for (int i = 0; i < 6; i++) // iterate faces
		{
			// check which faces are visible
			if (facing & (1 << i))
			{
				vertex_t* vt = &blockmodels.cubes[0].get(i, 0);
				
				// this face was visible
				for (int f = 0; f < 4; f++) // iterate vertices
				{
					short x = baseX + (vt->x >> RenderConst::VERTEX_SHL);
					short y = baseY + (vt->y >> RenderConst::VERTEX_SHL);
					short z = baseZ + (vt->z >> RenderConst::VERTEX_SHL);
					
					if ((x | y | z) >= 0 && x <= Sector::BLOCKS_XZ && y <= Sector::BLOCKS_Y && z <= Sector::BLOCKS_XZ)
					{
						addCornerShadowVertex(ao, vt, x, y, z);
					}
					vt += 1;
				}
				
			} // facing & side
			
		} // next face
	}
	
	inline bool cornerShadowBlocktest(const Block& block)
	{
		block_t id = block.getID();
		return (id > AIR_END && id < CROSS_START && id != _LANTERN && id != _VINES);
	}
	
	void PrecompThread::ambientOcclusionGradients(AmbientOcclusion* ambocc, vertex_t* datadump, int vertexCount)
	{
		// ambient occlusion structure
		AmbientOcclusion& ao = *ambocc;
		// clear/reset any previous data
		ao.clear();
		
		// sector belonging to precomp
		Sector& sector = precomp->sector[0];
		
		// world height in block units
		short worldY = sector.y * Sector::BLOCKS_Y;
		
		// calculate face counts for each integral vertex position
		vertex_t* vt = datadump; // first vertex
		
		int firstCorner = 50000;
		int lastCorner  = 0;
		
		for (int i = 0; i < vertexCount; i++)
		{
			if (vt->face) // only supported faces!
			{
				short x = (vt->x >> RenderConst::VERTEX_SHL);
				short y = (vt->y >> RenderConst::VERTEX_SHL) - worldY;
				short z = (vt->z >> RenderConst::VERTEX_SHL);
				
				// set new face value to ao index
				vt->face = addCornerShadowVertex(ao, vt, x, y, z);
				
				if (i < firstCorner) firstCorner = i;
				if (i > lastCorner) lastCorner = i;
			}
			vt += 1; // next vertex
		}
		// we can exit here, since theres no need to collect more vertices
		// if there are no original vertices to test on
		if (ao.count == 0) return;
		
		// we need to add remaining outside-of-sector points using Solidity()
		// first create transversal data with true for all reads
		ao.testdata.sector = &sector;
		ao.testdata.sb_x_m = &sector; ao.testdata.sb_x_p = &sector;
		ao.testdata.sb_y_m = &sector; ao.testdata.sb_y_p = &sector;
		ao.testdata.sb_z_m = &sector; ao.testdata.sb_z_p = &sector;
		
		for (int x = -1; x <= Sector::BLOCKS_XZ; x++)
		for (int z = -1; z <= Sector::BLOCKS_XZ; z++)
		for (int y = -1; y <= Sector::BLOCKS_Y; y++)
		{
			if (x == -1 || z == -1 || y == -1 || x == Sector::BLOCKS_XZ || z == Sector::BLOCKS_XZ || y == Sector::BLOCKS_Y)
			{
				Block& block = Spiders::getBlockNoGen(sector, x, y, z);
				// shaky test to ignore ids that definitely can't produce corner shadows
				if (cornerShadowBlocktest(block))
				{
					// finally run transversal
					int bx = x, by = y, bz = z;
					Sector* testsector = Spiders::spiderwrap(sector, bx, by, bz);
					unsigned short facing = block.visibleFaces(*testsector, bx, by, bz);
					
					// add corners to THIS sector
					if (facing) runCornerShadowTest(ao, facing, x, y, z);
					
				} // correct id
			}
		}
		
		if (ao.count >= ao.CRASH_MAX_POINTS)
		{
			logger << Log::ERR << "Too many corner shadow points: " << ao.count << Log::ENDL;
		}
		else if (ao.count > 2200)
		{
			logger << Log::WARN << "Many corner shadow points: " << ao.count << Log::ENDL;
		}
		
		static const int CRASH_CORNER = 255 - (Lighting.CORNERS + 8);
		static const int CRASH_WALL   = 255 - Lighting.CORNERS;
		static const int CRASH_PILLAR = 255 - Lighting.CORNERS;
		static const int OUTER_CORNER = 255 - Lighting.CORNERS;
		
		bool weSetSomething = false;
		
		for (int n = 1; n <= ao.count; n++)
		{
			// calculate final look of vertex x,y,z
			#define gets(vertex, face) ao.getPoint(n, vertex, face)
			
			// 1 0 0 = { 1 + 0 + 0 } = 1
			// 0 1 0 = { 0 + 2 + 0 } = 2
			// 0 0 1 = { 0 + 0 + 4 } = 4
			ao.t_corner[n] = 255;
			
				// corner 0 1 0, bottom top-face
			if (gets(0 + 2 + 0, 2))
			{
				// +z face = 0 with vertex at (0, 0, 1)
				if (gets(0 + 0 + 4, 0))
				{
					// +x face = 4 with vertex at (1, 0, 0)
					if (gets(1 + 0 + 0, 4)) // CORNER
					{
						ao.t_corner[n] = CRASH_CORNER;
					}
					// another +z face = 0 with vertex at (1, 0, 1)
					if (gets(1 + 0 + 4, 0))
					{
						ao.t_corner[n] = CRASH_WALL;
					}
				}
				// outer corner +z 1 0 1
				else if (gets(1 + 0 + 4, 0))
				{
					// outer corner +x 1 0 1
					if (gets(1 + 0 + 4, 4))
					{
						ao.t_corner[n] = OUTER_CORNER;
					}
				}
			}
				// corner 0 0 0, top bottom-face
			else if (gets(0 + 0 + 0, 3))
			{
				// +z face = 0 with vertex at (0, 1, 1)
				if (gets(0 + 2 + 4, 0))
				{
					// +x face = 4 with vertex at (1, 1, 0)
					if (gets(1 + 2 + 0, 4)) // CORNER
					{
						ao.t_corner[n] = CRASH_CORNER;
					}
					// another +z face = 0 with vertex at (1, 1, 1)
					if (gets(1 + 2 + 4, 0))
					{
						ao.t_corner[n] = CRASH_WALL;
					}
				}
			}
				// pillar corner of (0 0 0)
				// corner 1 0 0 +x face
			else if (gets(1 + 0 + 0, 4))
			{
				// forming corner 0 0 1 +z face
				if (gets(0 + 0 + 4, 0))
				{
					ao.t_corner[n] = CRASH_PILLAR;
				}
			}
				// cross-junction corner of (0 1 0)
				// corner 1 1 0 +x face
			else if (gets(1 + 2 + 0, 4))
			{
				// forming corner 0 1 1 +z face, with 1 0 1 +z or 1 0 1 +x
				if (gets(0 + 2 + 4, 0) && (gets(5,0) || gets(5,4)))
				{
					ao.t_corner[n] = CRASH_PILLAR;
				}
				
			}
			
				// corner 1 1 0 bottom top-face
			if (gets(1 + 2 + 0, 2))
			{
				// -x face = 5 with vertex at (0, 0, 0)
				if (gets(0 + 0 + 0, 5))
				{
					// +z face = 0 with vertex at (1, 0, 1)
					if (gets(1 + 0 + 4, 0)) // CORNER
					{
						ao.t_corner[n] = CRASH_CORNER;
					}
					
					// another -x face = 5 with vertex at (0, 0, 1)
					if (gets(0 + 0 + 4, 5))
					{
						ao.t_corner[n] = CRASH_WALL;
					}
				}
				// outer corner +z 0 0 1
				else if (gets(0 + 0 + 4, 0))
				{
					// outer corner -x 0 0 1
					if (gets(0 + 0 + 4, 5))
					{
						ao.t_corner[n] = OUTER_CORNER;
					}
				}
			}
				// corner 1 0 0 top bottom-face
			else if (gets(1 + 0 + 0, 3))
			{
				// -x face = 5 with vertex at (0, 1, 0)
				if (gets(0 + 2 + 0, 5))
				{
					// +z face = 0 with vertex at (1, 1, 1)
					if (gets(1 + 2 + 4, 0)) // CORNER
					{
						ao.t_corner[n] = CRASH_CORNER;
					}
					
					// another -x face = 5 with vertex at (0, 1, 1)
					if (gets(0 + 2 + 4, 5))
					{
						ao.t_corner[n] = CRASH_WALL;
					}
				}
			}
				// pillar corner of (1 0 0)
				// corner 1 0 1 +z face
			else if (gets(1 + 0 + 4, 0))
			{
				// forming corner 0 0 0 -x face
				if (gets(0 + 0 + 0, 5))
				{
					ao.t_corner[n] = CRASH_PILLAR;
				}
			}
				// cross-junction corner of (1 1 0)
				// top corner 0 1 0 -x face
			else if (gets(0 + 2 + 0, 5))
			{
				// forming corner 1 1 1 +z face, with 0 0 1 +z or 0 0 1 -x
				if (gets(1 + 2 + 4, 0) && (gets(4,0) || gets(4,5)))
				{
					ao.t_corner[n] = CRASH_PILLAR;
				}
			}
			
				// corner 0 1 1 bottom top-face
			if (gets(0 + 2 + 4, 2))
			{
				// +x face = 4 with vertex at (1, 0, 1)
				if (gets(1 + 0 + 4, 4))
				{
					// -z face = 1 with vertex at (0, 0, 0)
					if (gets(0 + 0 + 0, 1)) // CORNER
					{
						ao.t_corner[n] = CRASH_CORNER;
					}
					
					// another +x face = 4 with vertex at (1, 0, 0)
					if (gets(1 + 0 + 0, 4))
					{
						ao.t_corner[n] = CRASH_WALL;
					}
				}
				// outer corner +x 1 0 0
				else if (gets(1 + 0 + 0, 4))
				{
					// outer corner -z 1 0 0
					if (gets(1 + 0 + 0, 1))
					{
						ao.t_corner[n] = OUTER_CORNER;
					}
				}
			}
				// corner 0 0 1 top bottom-face
			else if (gets(0 + 0 + 4, 3))
			{
				// +x face = 4 with vertex at (1, 1, 1)
				if (gets(1 + 2 + 4, 4))
				{
					// -z face = 1 with vertex at (0, 1, 0)
					if (gets(0 + 2 + 0, 1)) // CORNER
					{
						ao.t_corner[n] = CRASH_CORNER;
					}
					
					// another +x face = 4 with vertex at (1, 1, 0)
					if (gets(1 + 2 + 0, 4))
					{
						ao.t_corner[n] = CRASH_WALL;
					}
				}
			}
				// pillar corner of (1 0 1)
				// corner 0 0 1 -x face
			else if (gets(0 + 0 + 4, 5))
			{
				// forming corner 1 0 0 -z face
				if (gets(1 + 0 + 0, 1))
				{
					ao.t_corner[n] = CRASH_PILLAR;
				}
			}
				// cross-junction corner of (1 1 0)
				// top corner 1 1 1 +x face
			else if (gets(1 + 2 + 4, 4))
			{
				// forming corner 0 1 0 -z face, with 0 0 1 -z or 0 0 1 +x
				if (gets(0 + 2 + 0, 1) && (gets(1,1) || gets(1,4)))
				{
					ao.t_corner[n] = CRASH_PILLAR;
				}
			}
			
				// corner 1 1 1 bottom top-face
			if (gets(1 + 2 + 4, 2))
			{
				// -z face = 1 with vertex at (1, 0, 0)
				if (gets(1 + 0 + 0, 1))
				{
					// -x face = 5 with vertex at (0, 0, 1)
					if (gets(0 + 0 + 4, 5)) // CORNER
					{
						ao.t_corner[n] = CRASH_CORNER;
					}
					// another -z face = 1 with vertex at (0, 0, 0)
					if (gets(0 + 0 + 0, 1))
					{
						ao.t_corner[n] = CRASH_WALL;
					}
				}
				// outer corner -z 0 0 0
				else if (gets(0 + 0 + 0, 1))
				{
					// outer corner -x 0 0 0
					if (gets(0 + 0 + 0, 5))
					{
						ao.t_corner[n] = OUTER_CORNER;
					}
				}
			}
				// corner 1 0 1 top bottom-face
			else if (gets(1 + 0 + 4, 3))
			{
				// -z face = 1 with vertex at (1, 1, 0)
				if (gets(1 + 2 + 0, 1))
				{
					// -x face = 5 with vertex at (0, 1, 1)
					if (gets(0 + 2 + 4, 5)) // CORNER
					{
						ao.t_corner[n] = CRASH_CORNER;
					}
					
					// another -z face = 1 with vertex at (0, 1, 0)
					if (gets(0 + 2 + 0, 1))
					{
						ao.t_corner[n] = CRASH_WALL;
					}
				}
			}
				// pillar corner of (0 0 1)
				// forming corner 1 0 1 +x face
			else if (gets(1 + 0 + 4, 4))
			{
				// corner 0 0 0 -z face
				if (gets(0 + 0 + 0, 1))
				{
					ao.t_corner[n] = CRASH_PILLAR;
				}
			}
				// cross-junction corner of (1 1 1)
				// top corner 1 1 0 -z face
			else if (gets(1 + 2 + 0, 1))
			{
				// forming corner 0 1 1 -x face, with 0 0 0 -z or 0 0 0 -x
				if (gets(0 + 2 + 4, 5) && (gets(0,1) || gets(0,5)))
				{
					ao.t_corner[n] = CRASH_PILLAR;
				}
			}
			
			if (ao.t_corner[n] != 255) weSetSomething = true;
			
		} // finally...
		
		// if no shadows were set, return immediately
		if (weSetSomething == false) return;
		
		// calculate total elements
		lastCorner += 1 - firstCorner;
		// find first vertex
		vt = datadump + firstCorner;
		
		// set corner shadow for each corner found
		// by looking up the corner id from lookup table
		for (int i = 0; i < lastCorner; i++)
		{
			if (vt->face)
			{
				// set corner shadow channel
				((unsigned char*)&vt->c)[2] = ao.t_corner[vt->face];
			}
			vt += 1; // next vertex
		}
		// finished... :/
	}

}
