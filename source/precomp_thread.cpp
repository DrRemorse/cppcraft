#include "precomp_thread.hpp"

#include <library/log.hpp>
#include "precompiler.hpp"
#include "precomp_thread_data.hpp"
#include "renderconst.hpp"
#include "sectors.hpp"
#include "flatlands.hpp"
#include "tiles.hpp"
#include "threading.hpp"
#include <cstring>

using namespace library;

namespace cppcraft
{
	const int PrecompThreadData::repeat_factor = RenderConst::VERTEX_SCALE / tiles.tilesPerBigtile;
	
	PrecompThread::PrecompThread()
	{
		this->ptd = new PrecompThreadData();
		
		// random default values for vertex array sizes
		// the arrays get automatically resized as needed
		static const int pipelineSize[RenderConst::MAX_UNIQUE_SHADERS] =
		{
			5000, // TX_REPEAT
			5000, // TX_SOLID
			8000, // TX_TRANS
			1500, // TX_2SIDED
			1500, // TX_CROSS
			1500, // TX_LAVA
			2000, // TX_WATER
			 500  // TX_RUNNING_WATER
		};
		
		for (int i = 0; i < RenderConst::MAX_UNIQUE_SHADERS; i++)
		{
			// set initial shaderline sizes
			ptd->pipelineSize[i] = pipelineSize[i];
			// initialize each shaderline
			ptd->databuffer[i] = new vertex_t[pipelineSize[i]];
		}
	}
	PrecompThread::~PrecompThread()
	{
		delete this->ptd;
	}
	// free vertex arrays
	PrecompThreadData::~PrecompThreadData()
	{
		for (int i = 0; i < RenderConst::MAX_UNIQUE_SHADERS; i++)
			delete[] this->databuffer[i];
	}
	
	void PrecompThread::precompile(Precomp& pc)
	{
		Sector& sector = pc.sector[0];
		PrecompThreadData& pcg = *this->ptd;
		
		// reset light list
		pcg.ldata.gathered = false;
		
		// last blockid, starting with _AIR
		pcg.lastid = _AIR;
		
		// zero out faces present
		for (int i = 0; i < RenderConst::MAX_UNIQUE_SHADERS; i++)
		{
			pcg.vertices[i] = 0;
		}
		
		// number of big tiles
		pcg.bigTextures = tiles.bigTilesX * tiles.bigTilesY;
		
		// Y-world coordinate
		pcg.worldY = (sector.getY() * Sector::BLOCKS_Y) * RenderConst::VERTEX_SCALE;
		
		// flatland data (biome +++)
		pcg.flatl = &flatlands(sector.getX(), sector.getZ());
		// flatlands +x
		pcg.flatl_x = &flatlands(sector.getX()+1, sector.getZ());
		// flatlands +z
		pcg.flatl_z = &flatlands(sector.getX(), sector.getZ()+1);
		// flatlands +xz
		pcg.flatl_xz = &flatlands(sector.getX()+1, sector.getZ()+1);
		// initialize to invalid CRC value
		pcg.fbicrc = 256;
		
		// set sector & testdata (visible neighbors)
		pcg.sector   = pc.sector;
		pcg.testdata = &pc.vfaces;
		
		if (sector.solidFlags() != Sector::MAX_HARDSOLID)
		{
			// iterate all
			int bx = Sector::BLOCKS_XZ-1;
			int by = Sector::BLOCKS_Y -1;
			int bz = Sector::BLOCKS_XZ-1;
			// get pointer to current block
			Block* currentBlock = &sector(bx, by, bz);
			// number of non-air blocks
			int blocks = sector.blockCount();
			
			while (blocks)
			{
				// ignore AIR and invalid blocks
				if (currentBlock->getID() > AIR_END && currentBlock->getID() <= MAX_UNIQUE_IDS)
				{
					// process one block id, and potentially add it to mesh
					// the generated mesh is added to a shaderline determined by its block id
					pcg.process_block(*currentBlock, bx, by, bz);
					
					// count down blocks
					blocks -= 1;
				} // if (valid id)
				
				by -= 1;
				if (by == -1)
				{
					bz -= 1;
					if (bz == -1)
					{
						bx -= 1;
						if (bx == -1) break;
						bz = Sector::BLOCKS_XZ - 1;
					}
					by = Sector::BLOCKS_Y - 1;
				}
				// current block pointer
				currentBlock -= 1;
			}
		}
		else
		{
			for (int bx = Sector::BLOCKS_XZ-1; bx >= 0; bx--)
			for (int bz = Sector::BLOCKS_XZ-1; bz >= 0; bz--)
			for (int by = Sector::BLOCKS_Y-1; by >= 0; by--)
			{
				// iterate only edges
				if ((bx == 0 && pcg.testdata->test_x_m) || (bx == Sector::BLOCKS_XZ-1 && pcg.testdata->test_x_p) || 
					(by == 0 && pcg.testdata->test_y_m) || (by == Sector::BLOCKS_Y-1  && pcg.testdata->test_y_p) ||
					(bz == 0 && pcg.testdata->test_z_m) || (bz == Sector::BLOCKS_XZ-1 && pcg.testdata->test_z_p))
				{
					Block& currentBlock = sector(bx, by, bz);
					// ignore AIR and invalid blocks
					if (currentBlock.getID() > AIR_END && currentBlock.getID() <= MAX_UNIQUE_IDS)
					{
						// process one block id, and potentially add it to mesh
						// the generated mesh is added to a shaderline determined by its block id
						pcg.process_block(currentBlock, bx, by, bz);
						
					} // if (valid id)
				}
			}
		}
		
		// count the number of vertices we've collected
		int cnt = pcg.vertices[0];
		for (int i = 1; i < RenderConst::MAX_UNIQUE_SHADERS; i++)
		{
			cnt += pcg.vertices[i];
		}
		
		if (cnt == 0)
		{
			// no vertices, we can exit early, but make sure to
			// mark the sector as culled
			pc.result = Precomp::STATUS_CULLED;
			return;
		}
		
		// allocate exact number of vertices
		delete[] pc.datadump;
		pc.datadump = new vertex_t[cnt];
		
		// prepare for next stage
		cnt = 0;
		for (int i = 0; i < RenderConst::MAX_UNIQUE_SHADERS; i++)
		{
			pc.vertices[i] = pcg.vertices[i];
			pc.bufferoffset[i] = cnt;
			
			// copy over to our local dump, but only if it had vertices
			if (pcg.vertices[i])
			{
				memcpy(pc.datadump + cnt, pcg.databuffer[i], pcg.vertices[i] * sizeof(vertex_t));
				cnt += pcg.vertices[i];
			}
		}
		
		// set job as done
		pc.result = Precomp::STATUS_DONE;
	}
	
}
