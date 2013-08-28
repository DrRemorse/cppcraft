#include "precomp_thread.hpp"

#include "library/log.hpp"

#include "blockmodels.hpp"
#include "lighting.hpp"
#include "precompiler.hpp"
#include "sector.hpp"
#include "renderconst.hpp"

using namespace library;

namespace cppcraft
{
	struct ao_struct
	{
		static const int CRASH_MAX_POINTS = 3200;
		
		int count = 0;
		
		// counters for how many vertices are added to each face
		bool t_count[CRASH_MAX_POINTS][8][6] = {{{false}}};
		// counter for corners per vertex
		unsigned char t_corner[CRASH_MAX_POINTS] = {0};
		// final lookup table
		short t_lookup[Sector::BLOCKS_XZ+1]
					  [Sector::BLOCKS_Y +1]
					  [Sector::BLOCKS_XZ+1] = {{{0}}};
		// test data
		visiblefaces_t testdata;
	};
	
	void addCornerShadowVertex(ao_struct& ao, vertex_t* vt, short x, short y, short z)
	{
		if ((x | y | z) >= 0 && x <= Sector::BLOCKS_XZ && y <= Sector::BLOCKS_Y && z <= Sector::BLOCKS_XZ)
		{
			short corner;
			corner  = (vt->face &  8) >> 3; // 0 if left,  1 if right
			corner += (vt->face & 16) >> 3; // 0 if lower, 2 if upper
			corner += (vt->face & 32) >> 3; // 0 if back,  4 if front
			
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
			ao.t_count[index][corner][face] = true;
		}
	}
	
	void runCornerShadowTest(ao_struct& ao, int facing, short baseX, short baseY, short baseZ)
	{
		if (facing == 0) return;
		
		// this block was visible
		for (int i = 0; i < 6; i++) // iterate faces
		{
			// check which faces are visible
			if (facing & (1 << i))
			{
				// this face was visible
				for (int f = 0; f < 4; f++) // iterate vertices
				{
					vertex_t* vt = &blockmodels.cubes[0].get(i, f);
					
					short x = baseX + vt->x / RenderConst::VERTEX_SCALE;
					short y = baseY + vt->y / RenderConst::VERTEX_SCALE;
					short z = baseZ + vt->z / RenderConst::VERTEX_SCALE;
					
					addCornerShadowVertex(ao, vt, x, y, z);
				}
				
			} // facing & side
			
		} // next face
	}
	
	void PrecompThread::ambientOcclusionGradients(vertex_t* datadump, int vertexCount)
	{
		// ambient occlusion structure
		ao_struct ao;
		ao.count = 0;
		// sector belonging to precomp
		Sector& sector = precomp->sector[0];
		
		// world height in block units
		short worldY = sector.y * Sector::BLOCKS_Y;
		
		// calculate face counts for each integral vertex position
		vertex_t* vt = datadump; // first vertex
		
		for (int i = 0; i < vertexCount; i++)
		{
			if (vt->face) // only supported faces!
			{
				short x = vt->x / RenderConst::VERTEX_SCALE;
				short y = vt->y / RenderConst::VERTEX_SCALE - worldY;
				short z = vt->z / RenderConst::VERTEX_SCALE;
				
				addCornerShadowVertex(ao, vt, x, y, z);
			}
			vt += 1; // next vertex
		}
		
		// we need to add remaining outside-of-sector points using Solidity()
		// first create transversal data with true for all reads
		ao.testdata.sector = &sector;
		ao.testdata.sb_x_m = &sector; ao.testdata.sb_x_p = &sector;
		ao.testdata.sb_y_m = &sector; ao.testdata.sb_y_p = &sector;
		ao.testdata.sb_z_m = &sector; ao.testdata.sb_z_p = &sector;
		
		#define cornerShadowBlocktest(block) \
			(block.getID() > AIR_END && block.getID() < CROSS_START && block.getID() != _LANTERN && block.getID() != _VINES)
		
		// - Y
		if (pcg.testdata.test_y_m == 2)
		{
			// set sectorblock pointer to this test-subject
			ao.testdata.sector = pcg.testdata.sb_y_m;
			ao.testdata.test_y_p = 2; // test y-negative
			
			// run tests
			for (int bx = 0; bx < Sector::BLOCKS_XZ; bx++)
			{
				for (int bz = 0; bz < Sector::BLOCKS_XZ; bz++)
				{
					// testing y = blockszy-1, on the sector above us
					const Block& block = ao.testdata.sector[0](bx, Sector::BLOCKS_Y-1, bz);
					
					// shaky test to ignore ids that definitely can't produce corner shadows
					if (cornerShadowBlocktest(block))
					{
						// finally run transversal
						unsigned short facing = block.visibleFaces(ao.testdata, bx, Sector::BLOCKS_Y-1, bz);
						
						runCornerShadowTest(ao, facing, bx, -1, bz);
						
					} // correct id
					
				} // next bz
				
			} // next bx
			
			// disable test y-positive
			ao.testdata.test_y_p = false;
		}
		
		// + Y
		if (pcg.testdata.test_y_p == 2)
		{
			// set sectorblock pointer to this test-subject
			ao.testdata.sector = pcg.testdata.sb_y_p;
			ao.testdata.test_y_m = 2; // test y- from +y sector
			
			// run tests
			for (int bx = 0; bx < Sector::BLOCKS_XZ; bx++)
			{
				for (int bz = 0; bz < Sector::BLOCKS_XZ; bz++)
				{
					// testing y = 0, on the sector above us
					const Block& block = ao.testdata.sector[0](bx, 0, bz);
					
					// shaky test to ignore ids that definitely can't produce corner shadows
					if (cornerShadowBlocktest(block))
					{
						// finally run transversal
						unsigned short facing = block.visibleFaces(ao.testdata, bx, 0, bz);
						
						runCornerShadowTest(ao, facing, bx, Sector::BLOCKS_Y, bz);
						
					} // correct id
					
				} // next bz
			} // next bx
			
			// disable test y-
			ao.testdata.test_y_m = false;
		}
		
		// + X
		if (pcg.testdata.test_x_p == 2)
		{
			// set sectorblock pointer to this test-subject
			ao.testdata.sector = pcg.testdata.sb_x_p;
			ao.testdata.test_x_m = 2; // test -x
			
			// run tests
			for (int bz = 0; bz < Sector::BLOCKS_XZ; bz++)
			{
				for (int by = 0; by < Sector::BLOCKS_Y; by++)
				{
					// testing x = 0, on the sector to the right
					const Block& block = ao.testdata.sector[0](0, by, bz);
					
					// shaky test to ignore ids that definitely can't produce corner shadows
					if (cornerShadowBlocktest(block))
					{
						// finally run transversal
						unsigned short facing = block.visibleFaces(ao.testdata, 0, by, bz);
						
						runCornerShadowTest(ao, facing, Sector::BLOCKS_XZ, by, bz);
					}
				}
			}
			
			// disable test x-
			ao.testdata.test_x_m = false;
		}
		
		// - X
		if (pcg.testdata.test_x_m == 2)
		{
			// set sectorblock pointer to this test-subject
			ao.testdata.sector = pcg.testdata.sb_x_m;
			ao.testdata.test_x_p = 2; // test +x
			
			// run tests
			for (int bz = 0; bz < Sector::BLOCKS_XZ; bz++)
			{
				for (int by = 0; by < Sector::BLOCKS_Y; by++)
				{
					// testing x = blocksz-1, on the sector to the left
					const Block& block = ao.testdata.sector[0](Sector::BLOCKS_XZ-1, by, bz);
					
					// shaky test to ignore ids that definitely can't produce corner shadows
					if (cornerShadowBlocktest(block))
					{
						// finally run transversal
						unsigned short facing = block.visibleFaces(ao.testdata, Sector::BLOCKS_XZ-1, by, bz);
						
						runCornerShadowTest(ao, facing, -1, by, bz);
					}
				}
			}
			
			// disable test x+
			ao.testdata.test_x_p = false;
		}
		
		// + Z
		if (pcg.testdata.test_z_p == 2)
		{
			// set sectorblock pointer to this test-subject
			ao.testdata.sector = pcg.testdata.sb_z_p;
			ao.testdata.test_z_m = 2; // test -z
			
			// run tests
			for (int bx = 0; bx < Sector::BLOCKS_XZ; bx++)
			{
				for (int by = 0; by < Sector::BLOCKS_Y; by++)
				{
					// testing z = 0, on the sector in front
					const Block& block = ao.testdata.sector[0](bx, by, 0);
					
					// shaky test to ignore ids that definitely can't produce corner shadows
					if (cornerShadowBlocktest(block))
					{
						// finally run transversal
						unsigned short facing = block.visibleFaces(ao.testdata, bx, by, 0);
						
						runCornerShadowTest(ao, facing, bx, by, Sector::BLOCKS_XZ);
						
					} // correct id
					
				} // bz
			} // bx
			
			// disable test z-
			ao.testdata.test_z_m = false;
		}
		// - Z
		if (pcg.testdata.test_z_m == 2)
		{
			// set sectorblock pointer to this test-subject
			ao.testdata.sector = pcg.testdata.sb_z_m;
			ao.testdata.test_z_p = 2; // test +z
			
			// run tests
			for (int bx = 0; bx < Sector::BLOCKS_XZ; bx++)
			{
				for (int by = 0; by < Sector::BLOCKS_Y; by++)
				{
					// testing z = blocksz-1, on the sector to the back
					const Block& block = ao.testdata.sector[0](bx, by, Sector::BLOCKS_XZ-1);
					
					// shaky test to ignore ids that definitely can't produce corner shadows
					if (cornerShadowBlocktest(block))
					{
						// finally run transversal
						unsigned short facing = block.visibleFaces(ao.testdata, bx, by, Sector::BLOCKS_XZ-1);
						
						runCornerShadowTest(ao, facing, bx, by, -1);
						
					} // correct id
					
				} // bz
			} // bx
		}
		
		if (ao.count >= ao.CRASH_MAX_POINTS)
		{
			logger << Log::ERR << "Too many corner shadow points" << Log::ENDL;
		}
		
		const int CRASH_CORNER = Lighting.CORNERS + 8;
		const int CRASH_WALL   = Lighting.CORNERS;
		const int CRASH_PILLAR = Lighting.CORNERS;
		const int OUTER_CORNER = Lighting.CORNERS;
		
		for (int n = 1; n < ao.count; n++)
		{
			// calculate final look of vertex x,y,z
			#define gets(vertex, face) ao.t_count[n][vertex][face]
			
			// 1 0 0 = { 1 + 0 + 0 } = 1
			// 0 1 0 = { 0 + 2 + 0 } = 2
			// 0 0 1 = { 0 + 0 + 4 } = 4
			
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
			
		} // finally...
		
		// set corner shadow for each corner found
		// by looking up the corner id from lookup table
		vt = datadump; // first vertex
		for (int i = 0; i < vertexCount; i++)
		{
			if (vt->face)
			{
				int x = vt->x / RenderConst::VERTEX_SCALE;
				int y = vt->y / RenderConst::VERTEX_SCALE - worldY;
				int z = vt->z / RenderConst::VERTEX_SCALE;
				
				if ((x | y | z) >= 0 && x <= Sector::BLOCKS_XZ && y <= Sector::BLOCKS_Y && z <= Sector::BLOCKS_XZ)
				{
					// find index in lookup table
					short index = ao.t_lookup[x][y][z];
					if (index)
					{
						// set corner shadow channel
						((unsigned char*)&vt->c)[2] = 255 - ao.t_corner[index];
					}
				}
				
			}
			vt += 1; // next vertex
		}
		// finished... :/
	}

}
