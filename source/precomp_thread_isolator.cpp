#include "precomp_thread.hpp"

#include "library/log.hpp"
#include "precompiler.hpp"
#include "sectors.hpp"

using namespace library;

namespace cppcraft
{
	// killing a precomp means theres is nothing to generate mesh for
	// the precompilation process is complete
	void PrecompThread::killPrecomp()
	{
		// kill this precompilation run
		Sector* s   = precomp->sector;
		s->progress = Sector::PROG_COMPILED;
		precomp->alive = false;
	}
	
	// when cancelling precompilation, it's because some prerequisites
	// were not fulfilled, so we need to come back later
	void PrecompThread::cancelPrecomp()
	{
		// kill this precompilation run
		Sector* s   = precomp->sector;
		s->progress = Sector::PROG_NEEDRECOMP;
		precomp->alive = false;
	}
	
	// returns true if we should continue to stage 2 - generating mesh
	// returns false if for some reason there is a problem,
	// or there is simply nothing to generate mesh for
	bool PrecompThread::isolator()
	{
		Precomp& pc = precomp[0];
		Sector&  sector  = pc.sector[0];
		
		// sector could have been in queue, and been reduced to nullsector
		if (sector.contents == Sector::CONT_NULLSECTOR)
		{
			logger << Log::ERR << "nullsector in precompiler" << Log::ENDL;
			killPrecomp();
			return false;
		}
		else if (sector.hasBlocks() == false)
		{
			logger << Log::ERR << "sector without blocks in precompiler" << Log::ENDL;
			killPrecomp();
			return false;
		}
		else if (sector.blockpt->blocks == 0)
		{
			logger << Log::WARN << "sector with 0 blocks in precompiler" << Log::ENDL;
			killPrecomp();
			return false;
		}
		else if (sector.progress != Sector::PROG_RECOMPILING)
		{
			logger << Log::ERR << "sector with progress mismatch in isolator" << Log::ENDL;
			cancelPrecomp();
			return false;
		}
		
		// prepare testdata for determining visible faces
		pcg.testdata.sector = pc.sector;
		
		if (sector.x) // -x
		{
			Sector& testsector = Sectors(sector.x-1, sector.y, sector.z);
			
			if (testsector.contents == Sector::CONT_NULLSECTOR)
			{
				pcg.testdata.test_x_m = 1; // air  (always)
			}
			else if (testsector.contents == Sector::CONT_UNKNOWN)
			{
				// unfinished sector, cancel precompilation
				cancelPrecomp();
				return false;
			}
			else // sector must have blocks (WARNING assumption)
			{
				pcg.testdata.test_x_m = (testsector.blockpt->hardsolid & 1) == 0;
				if (pcg.testdata.test_x_m)
				{
					pcg.testdata.test_x_m = 2; // read (compare)
					pcg.testdata.sb_x_m = &testsector;
				}
			}
		}
		else
		{
			pcg.testdata.test_x_m = 0;
		} // -x
		
		if (sector.x != Sectors.getXZ()-1) // test +x
		{
			Sector& testsector = Sectors(sector.x+1, sector.y, sector.z);
			
			if (testsector.contents == Sector::CONT_NULLSECTOR)
			{
				pcg.testdata.test_x_p = 1; // nullsector
			}
			else if (testsector.contents == Sector::CONT_UNKNOWN)
			{
				cancelPrecomp();
				return false;
			}
			else // sector must have blocks (WARNING assumption)
			{
				pcg.testdata.test_x_p = (testsector.blockpt->hardsolid & 2) == 0;
				
				if (pcg.testdata.test_x_p)
				{
					pcg.testdata.test_x_p = 2; // contains data
					pcg.testdata.sb_x_p   = &testsector;
				}
			}
		}
		else
		{
			pcg.testdata.test_x_p = 0;
		} // test +x
		
		if (sector.y) // test -y
		{
			Sector& testsector = Sectors(sector.x, sector.y-1, sector.z);
			
			if (testsector.contents == Sector::CONT_NULLSECTOR)
			{
				pcg.testdata.test_y_m = 1; // nullsector
			}
			else if (testsector.contents == Sector::CONT_UNKNOWN)
			{
				cancelPrecomp();
				return false;
			}
			else // sector must have blocks (WARNING assumption)
			{
				pcg.testdata.test_y_m = (testsector.blockpt->hardsolid & 4) == 0;
				
				if (pcg.testdata.test_y_m)
				{
					pcg.testdata.test_y_m = 2; // contains data
					pcg.testdata.sb_y_m   = &testsector;
				}
			}
		}
		else
		{
			pcg.testdata.test_y_m = 0;
		} // test -y
		
		if (sector.y != Sectors.getY()-1) // test +y
		{
			Sector& testsector = Sectors(sector.x, sector.y+1, sector.z);
			
			if (testsector.contents == Sector::CONT_NULLSECTOR)
			{
				pcg.testdata.test_y_p = 1; // nullsector
			}
			else if (testsector.contents == Sector::CONT_UNKNOWN)
			{
				cancelPrecomp();
				return false;
			}
			else // sector must have blocks (WARNING assumption)
			{
				pcg.testdata.test_y_p = (testsector.blockpt->hardsolid & 8) == 0;
				if (pcg.testdata.test_y_p)
				{
					pcg.testdata.test_y_p = 2; // contains data
					pcg.testdata.sb_y_p   = &testsector;
				}
			}
		}
		else
		{
			pcg.testdata.test_y_p = 0;
		} // test +y
		
		if (sector.z) // test -z
		{
			Sector& testsector = Sectors(sector.x, sector.y, sector.z-1);
			
			if (testsector.contents == Sector::CONT_NULLSECTOR)
			{
				pcg.testdata.test_z_m = 1; // nullsector
			}
			else if (testsector.contents == Sector::CONT_UNKNOWN)
			{
				cancelPrecomp();
				return false;
			}
			else // sector must have blocks (WARNING assumption)
			{
				pcg.testdata.test_z_m = (testsector.blockpt->hardsolid & 16) == 0;
				
				if (pcg.testdata.test_z_m)
				{
					pcg.testdata.test_z_m = 2; // data
					pcg.testdata.sb_z_m   = &testsector;
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
			
			if (testsector.contents == Sector::CONT_NULLSECTOR)
			{
				pcg.testdata.test_z_p = 1; // nullsector
			}
			else if (testsector.contents == Sector::CONT_UNKNOWN)
			{
				cancelPrecomp();
				return false;
			}
			else // sector must have blocks (WARNING assumption)
			{
				pcg.testdata.test_z_p = (testsector.blockpt->hardsolid & 32) == 0;
				
				if (pcg.testdata.test_z_p)
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
			pc.alive = false; // nothing to compile...
			return false;
		}
		
		// continue to stage 2 - generate mesh
		return true;
	}
}
