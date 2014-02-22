#include "precomp_thread.hpp"

#include <library/log.hpp>
#include <cstring>
#include "precompiler.hpp"
#include "renderconst.hpp"
#include "sectors.hpp"
#include "flatlands.hpp"
#include "tiles.hpp"
#include "threading.hpp"

using namespace library;

namespace cppcraft
{
	const int PrecompThreadData::repeat_factor = RenderConst::VERTEX_SCALE / tiles.tilesPerBigtile;
	
	// free vertex arrays
	PrecompThreadData::~PrecompThreadData()
	{
		for (int i = 0; i < RenderConst::MAX_UNIQUE_SHADERS; i++)
		{
			delete[] this->databuffer[i];
		}
	}
	
	void PrecompThread::precompile()
	{
		Precomp& pc = precomp[0];
		Sector&  sector  = pc.sector[0];
		
		// reset light list
		pcg.ldata.gathered = false;
		
		// vertex data variables
		pcg.indic = nullptr;
		
		// last blockid, starting with _AIR
		pcg.lastid = _AIR;
		
		// selected shaderline
		pcg.shaderLine = 0;
		
		// zero out faces present
		for (int i = 0; i < RenderConst::MAX_UNIQUE_SHADERS; i++)
		{
			pcg.vertices[i] = 0;
		}
		
		// repeating big tiles
		pcg.repeat_y = true;
		// number of big tiles
		pcg.bigTextures = tiles.bigTilesX * tiles.bigTilesY;
		
		// world coordinates
		pcg.worldY = (sector.y * Sector::BLOCKS_Y) * RenderConst::VERTEX_SCALE;
		pcg.worldY_extra = 0;
		
		
		// flatland data (biome +++)
		pcg.flatl = flatlands.manipulate(sector.x, sector.z);
		// flatlands +x
		pcg.flatl_x = nullptr;
		if (sector.x != Sectors.getXZ()-1)
			pcg.flatl_x = flatlands.manipulate(sector.x+1, sector.z);
		// flatlands +z
		pcg.flatl_z = nullptr;
		if (sector.z != Sectors.getXZ()-1)
			pcg.flatl_z = flatlands.manipulate(sector.x, sector.z+1);
		// flatlands +xz
		pcg.flatl_xz = nullptr;
		if (pcg.flatl_x != nullptr && pcg.flatl_z != nullptr)
			pcg.flatl_xz = flatlands.manipulate(sector.x+1, sector.z+1);
		// initialize to invalid CRC value
		pcg.fbicrc = 256;
		
		// iterate all
		pcg.sector = pc.sector;
		int bx = Sector::BLOCKS_XZ-1;
		int by = Sector::BLOCKS_Y -1;
		int bz = Sector::BLOCKS_XZ-1;
		// get pointer to current block
		Block* currentBlock = &sector(bx, by, bz);
		// number of non-air blocks
		int blocks = sector.blockpt->blocks;
		//bool totalsolid = (sector.hardsolid == Sector::MAX_HARDSOLID);
		
		while(blocks)
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
		
		// count the number of vertices we've collected
		int cnt = pcg.vertices[0];
		for (int i = 1; i < RenderConst::MAX_UNIQUE_SHADERS; i++)
		{
			cnt += pcg.vertices[i];
		}
		
		if (cnt == 0)
		{
			// only air, no faces present
			sector.render = false;
			sector.progress = Sector::PROG_COMPILED;
			sector.culled = true;
			precomp->alive = false; // nothing to compile...
			return;
		}
		
		// allocate exact number of vertices
		vertex_t* datadump = new vertex_t[cnt];
		
		if (datadump == nullptr)
		{
			logger << Log::ERR << "PrecompThread::precompile(): datadump was null" << Log::ENDL;
			// we cannot continue, kill the precomp
			killPrecomp();
			return;
		}
		
		int bufferoffset[RenderConst::MAX_UNIQUE_SHADERS];
		cnt = 0;
		
		for (int i = 0; i < RenderConst::MAX_UNIQUE_SHADERS; i++)
		{
			// copy over to our local dump, but only if it had vertices
			bufferoffset[i] = cnt;
			if (pcg.vertices[i])
			{
				memcpy(datadump + cnt, pcg.databuffer[i], pcg.vertices[i] * sizeof(vertex_t));
				cnt += pcg.vertices[i];
			}
		}
		
		// prepare for next stage
		pc.datadump = datadump;
		for (int i = 0; i < RenderConst::MAX_UNIQUE_SHADERS; i++)
		{
			// copy over to dump, but only if it had faces
			pc.bufferoffset[i] = bufferoffset[i];
			pc.vertices[i]     = pcg.vertices[i];
		}
		
		// this stage has ended
		// go to next stage: ambient occlusion gradients
		if (sector.progress == Sector::PROG_RECOMPILING)
			sector.progress = Sector::PROG_NEEDAO;
	}
}
