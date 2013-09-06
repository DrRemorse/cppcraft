#include "precomp_thread.hpp"

#include "library/log.hpp"

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
	
	void PrecompThread::killPrecomp()
	{
		// kill this precompilation run
		Sector* s   = precomp->sector;
		s->precomp  = 0;
		s->progress = Sector::PROG_COMPILED;
		precomp->alive = false;
	}
	
	void PrecompThread::cancelPrecomp()
	{
		// kill this precompilation run
		Sector* s   = precomp->sector;
		s->precomp  = 0;
		s->progress = Sector::PROG_NEEDRECOMP;
		precomp->alive = false;
	}
	
	void PrecompThread::precompile()
	{
		Precomp& pc = precomp[0];
		Sector&  sector  = pc.sector[0];
		
		// sector could have been in queue, and been reduced to nullsector
		if (sector.contents == Sector::CONT_NULLSECTOR)
		{
			logger << Log::WARN << "nullsector in precompiler" << Log::ENDL;
			killPrecomp();
			return;
		}
		else if (sector.hasBlocks() == false)
		{
			logger << Log::WARN << "sector without blocks in precompiler" << Log::ENDL;
			killPrecomp();
			return;
		}
		else if (sector.blockpt->blocks == 0)
		{
			logger << Log::WARN << "sector with 0 blocks in precompiler" << Log::ENDL;
			killPrecomp();
			return;
		}
		
		// fix possible state error
		if (sector.progress == Sector::PROG_NEEDRECOMP)
		{
			//logger << Log::WARN << "sector state changed from needrec to recomp in precompiler" << Log::ENDL;
			sector.progress = Sector::PROG_RECOMP;
		}
		
		// prepare testdata for determining visible faces
		pcg.testdata.sector = pc.sector;
		
		if (sector.x) // -x
		{
			Sector* testsector = &Sectors(sector.x-1, sector.y, sector.z);
			pcg.testdata.test_x_m = (testsector->hardsolid & 1) == 0;
			
			if (pcg.testdata.test_x_m)
			{
				if (testsector->contents == Sector::CONT_NULLSECTOR)
				{
					pcg.testdata.test_x_m = 1; // air  (always)
				}
				else if (testsector->contents == Sector::CONT_UNKNOWN)
				{
					// unfinished sector, cancel precompilation
					cancelPrecomp();
					return;
				}
				else
				{
					pcg.testdata.test_x_m = 2; // read (compare)
					pcg.testdata.sb_x_m = testsector;
				}
				
			}
			
		}
		else
		{
			pcg.testdata.test_x_m = 0;
		} // -x
		
		if (sector.x != Sectors.getXZ()-1) // test +x
		{
			Sector* testsector = &Sectors(sector.x+1, sector.y, sector.z);
			pcg.testdata.test_x_p = (testsector->hardsolid & 2) == 0;
			
			if (pcg.testdata.test_x_p)
			{
				if (testsector->contents == Sector::CONT_NULLSECTOR)
				{
					pcg.testdata.test_x_p = 1; // nullsector
				}
				else if (testsector->contents == Sector::CONT_UNKNOWN)
				{
					cancelPrecomp();
					return;
				}
				else
				{
					pcg.testdata.test_x_p = 2; // contains data
					pcg.testdata.sb_x_p   = testsector;
				}
			}
		}
		else
		{
			pcg.testdata.test_x_p = 0;
		} // test +x
		
		if (sector.y) // test -y
		{
			Sector* testsector = &Sectors(sector.x, sector.y-1, sector.z);
			pcg.testdata.test_y_m = (testsector->hardsolid & 4) == 0;
			
			if (pcg.testdata.test_y_m)
			{
				if (testsector->contents == Sector::CONT_NULLSECTOR)
				{
					pcg.testdata.test_y_m = 1; // nullsector
				}
				else if (testsector->contents == Sector::CONT_UNKNOWN)
				{
					cancelPrecomp();
					return;
				}
				else
				{
					pcg.testdata.test_y_m = 2; // contains data
					pcg.testdata.sb_y_m   = testsector;
				}
			}
			
		}
		else
		{
			pcg.testdata.test_y_m = 0;
		} // test -y
		
		if (sector.y != Sectors.getY()-1) // test +y
		{
			Sector* testsector = &Sectors(sector.x, sector.y+1, sector.z);
			
			pcg.testdata.test_y_p = (testsector->hardsolid & 8) == 0;
			if (pcg.testdata.test_y_p)
			{
				if (testsector->contents == Sector::CONT_NULLSECTOR)
				{
					pcg.testdata.test_y_p = 1; // nullsector
				}
				else if (testsector->contents == Sector::CONT_UNKNOWN)
				{
					cancelPrecomp();
					return;
				}
				else
				{
					pcg.testdata.test_y_p = 2; // contains data
					pcg.testdata.sb_y_p   = testsector;
				}
			}
		}
		else
		{
			pcg.testdata.test_y_p = 0;
		} // test +y
		
		if (sector.z) // test -z
		{
			Sector* testsector = &Sectors(sector.x, sector.y, sector.z-1);
			pcg.testdata.test_z_m = (testsector->hardsolid & 16) == 0;
			
			if (pcg.testdata.test_z_m)
			{
				if (testsector->contents == Sector::CONT_NULLSECTOR)
				{
					pcg.testdata.test_z_m = 1; // nullsector
				}
				else if (testsector->contents == Sector::CONT_UNKNOWN)
				{
					cancelPrecomp();
					return;
				}
				else
				{
					pcg.testdata.test_z_m = 2; // data
					pcg.testdata.sb_z_m   = testsector;
				}
			}
		}
		else
		{
			pcg.testdata.test_z_m = 0;
		} // test -z
		
		if (sector.z != Sectors.getXZ()-1) // test +z
		{
			Sector& testsector = Sectors(sector.x, sector.y, sector.z+1);
			pcg.testdata.test_z_p = (testsector.hardsolid & 32) == 0;
			
			if (pcg.testdata.test_z_p)
			{
				if (testsector.contents == Sector::CONT_NULLSECTOR)
				{
					pcg.testdata.test_z_p = 1; // nullsector
				}
				else if (testsector.contents == Sector::CONT_UNKNOWN)
				{
					cancelPrecomp();
					return;
				}
				else
				{
					pcg.testdata.test_z_p = 2; // contains data
					pcg.testdata.sb_z_p   = &testsector;
				}
			}
		}
		else
		{
			pcg.testdata.test_z_p = 0;
		} // test +z
		
		// extreme optimizations:
		// -x +x -y +y -z +z
		if (pcg.testdata.test_x_m + pcg.testdata.test_x_p + pcg.testdata.test_y_m +
			pcg.testdata.test_y_p + pcg.testdata.test_z_m + pcg.testdata.test_z_p == 0)
		{
			sector.culled = true; // fully hidden by other sectors
			sector.render = false;
			// a culled sector.. is compiled!
			sector.progress = Sector::PROG_COMPILED;
			sector.precomp = 0;
			pc.alive = false; // nothing to compile...
			return;
		}
		
		// reset light list
		pcg.ldata.gathered = false;
		
		// vertex data variables
		pcg.indic     = nullptr;
		
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
		pcg.flatl = Flatlands.manipulate(sector.x, sector.z);
		// flatlands +x
		pcg.flatl_x = nullptr;
		if (sector.x != Sectors.getXZ()-1)
			pcg.flatl_x = Flatlands.manipulate(sector.x+1, sector.z);
		// flatlands +z
		pcg.flatl_z = nullptr;
		if (sector.z != Sectors.getXZ()-1)
			pcg.flatl_z = Flatlands.manipulate(sector.x, sector.z+1);
		// flatlands +xz
		pcg.flatl_xz = nullptr;
		if (pcg.flatl_x != nullptr && pcg.flatl_z != nullptr)
			pcg.flatl_xz = Flatlands.manipulate(sector.x+1, sector.z+1);
		
		pcg.fclid = 0;
		pcg.lastclid = 0;
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
		
		while(blocks)
		{
			//Block& currentBlock = s(bx, by, bz);
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
			sector.precomp = 0;
			precomp->alive = false; // nothing to compile...
			return;
		}
		
		// allocate exact number of vertices
		vertex_t* datadump = new vertex_t[cnt];
		
		if (datadump == nullptr)
		{
			logger << Log::ERR << "PrecompThread::precompile(): datadump was null" << Log::ENDL;
			// kill the precomp:
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
		
		// ambient occlusion processing stage
		#ifdef AMBIENT_OCCLUSION_GRADIENTS
			ambientOcclusionGradients(datadump, cnt);
		#endif
		
		// send to sectorcompiler
		//mtx.precompiler.lock();
		;{
			pc.datadump = datadump;
			for (int i = 0; i < RenderConst::MAX_UNIQUE_SHADERS; i++)
			{
				// copy over to dump, but only if it had faces
				pc.bufferoffset[i] = bufferoffset[i];
				pc.vertices[i] = pcg.vertices[i];
			}
			sector.progress = Sector::PROG_NEEDCOMPILE;
			sector.culled  = false;
			sector.precomp = 3; // flag as ready for compiler
		}
		//mtx.precompiler.unlock();
	}
}
