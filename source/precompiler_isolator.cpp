#include "precompiler.hpp"

#include <library/log.hpp>
#include "sectors.hpp"

using namespace library;

namespace cppcraft
{
	// killing a precomp means theres is nothing to generate mesh for
	// the precompilation process is complete
	void Precomp::kill()
	{
		// kill this precompilation run
		sector->progress = Sector::PROG_COMPILED;
		result = Precomp::STATUS_FAILED;
		alive  = false;
	}
	
	// when cancelling precompilation, it's because some prerequisites
	// were not fulfilled, so we need to come back later
	void Precomp::cancel()
	{
		// kill this precompilation run
		sector->progress = Sector::PROG_NEEDRECOMP;
		result = Precomp::STATUS_FAILED;
		alive  = false;
	}
	
	// returns true if we should continue to stage 2 - generating mesh
	// returns false if for some reason there is a problem,
	// or there is simply nothing to generate mesh for
	bool Precomp::isolator()
	{
		Sector& sector = this->sector[0];
		
		// sector could have been in queue, and been reduced to nullsector
		if (sector.contents == Sector::CONT_NULLSECTOR)
		{
			logger << Log::ERR << "nullsector in precompiler" << Log::ENDL;
			this->kill();
			return false;
		}
		else if (sector.hasBlocks() == false)
		{
			logger << Log::ERR << "sector without blocks in precompiler" << Log::ENDL;
			this->kill();
			return false;
		}
		else if (sector.blockCount() == 0)
		{
			logger << Log::WARN << "sector with 0 blocks in precompiler" << Log::ENDL;
			this->kill();
			return false;
		}
		else if (sector.progress != Sector::PROG_RECOMPILE)
		{
			logger << Log::ERR << "sector with progress mismatch in isolator" << Log::ENDL;
			this->cancel();
			return false;
		}
		
		// prepare testdata for determining visible faces
		vfaces.sector = &sector;
		
		if (sector.getX()) // -x
		{
			Sector& testsector = Sectors(sector.getX()-1, sector.getY(), sector.getZ());
			
			if (testsector.contents == Sector::CONT_NULLSECTOR)
			{
				vfaces.test_x_m = 1; // air  (always)
			}
			else if (testsector.contents == Sector::CONT_UNKNOWN)
			{
				// unfinished sector, cancel precompilation
				this->cancel();
				return false;
			}
			else // sector must have blocks (WARNING assumption)
			{
				vfaces.test_x_m = (testsector.solidFlags() & 1) == 0;
				if (vfaces.test_x_m)
				{
					vfaces.test_x_m = 2; // read (compare)
					vfaces.sb_x_m = &testsector;
				}
			}
		}
		else
		{
			vfaces.test_x_m = 0;
		} // -x
		
		if (sector.getX() != Sectors.getXZ()-1) // test +x
		{
			Sector& testsector = Sectors(sector.getX()+1, sector.getY(), sector.getZ());
			
			if (testsector.contents == Sector::CONT_NULLSECTOR)
			{
				vfaces.test_x_p = 1; // nullsector
			}
			else if (testsector.contents == Sector::CONT_UNKNOWN)
			{
				this->cancel();
				return false;
			}
			else // sector must have blocks (WARNING assumption)
			{
				vfaces.test_x_p = (testsector.solidFlags() & 2) == 0;
				
				if (vfaces.test_x_p)
				{
					vfaces.test_x_p = 2; // contains data
					vfaces.sb_x_p   = &testsector;
				}
			}
		}
		else
		{
			vfaces.test_x_p = 0;
		} // test +x
		
		if (sector.getY()) // test -y
		{
			Sector& testsector = Sectors(sector.getX(), sector.getY()-1, sector.getZ());
			
			if (testsector.contents == Sector::CONT_NULLSECTOR)
			{
				vfaces.test_y_m = 1; // nullsector
			}
			else if (testsector.contents == Sector::CONT_UNKNOWN)
			{
				this->cancel();
				return false;
			}
			else // sector must have blocks (WARNING assumption)
			{
				vfaces.test_y_m = (testsector.solidFlags() & 4) == 0;
				
				if (vfaces.test_y_m)
				{
					vfaces.test_y_m = 2; // contains data
					vfaces.sb_y_m   = &testsector;
				}
			}
		}
		else
		{
			vfaces.test_y_m = 0;
		} // test -y
		
		if (sector.getY() != Sectors.getY()-1) // test +y
		{
			Sector& testsector = Sectors(sector.getX(), sector.getY()+1, sector.getZ());
			
			if (testsector.contents == Sector::CONT_NULLSECTOR)
			{
				vfaces.test_y_p = 1; // nullsector
			}
			else if (testsector.contents == Sector::CONT_UNKNOWN)
			{
				this->cancel();
				return false;
			}
			else // sector must have blocks (WARNING assumption)
			{
				vfaces.test_y_p = (testsector.solidFlags() & 8) == 0;
				if (vfaces.test_y_p)
				{
					vfaces.test_y_p = 2; // contains data
					vfaces.sb_y_p   = &testsector;
				}
			}
		}
		else
		{
			// treat top of the world as nullsector
			vfaces.test_y_p = 1;
		} // test +y
		
		if (sector.getZ()) // test -z
		{
			Sector& testsector = Sectors(sector.getX(), sector.getY(), sector.getZ()-1);
			
			if (testsector.contents == Sector::CONT_NULLSECTOR)
			{
				vfaces.test_z_m = 1; // nullsector
			}
			else if (testsector.contents == Sector::CONT_UNKNOWN)
			{
				this->cancel();
				return false;
			}
			else // sector must have blocks (WARNING assumption)
			{
				vfaces.test_z_m = (testsector.solidFlags() & 16) == 0;
				
				if (vfaces.test_z_m)
				{
					vfaces.test_z_m = 2; // data
					vfaces.sb_z_m   = &testsector;
				}
			}
		}
		else
		{
			vfaces.test_z_m = 0;
		} // test -z
		
		if (sector.getZ() != Sectors.getXZ()-1) // test +z
		{
			Sector& testsector = Sectors(sector.getX(), sector.getY(), sector.getZ()+1);
			
			if (testsector.contents == Sector::CONT_NULLSECTOR)
			{
				vfaces.test_z_p = 1; // nullsector
			}
			else if (testsector.contents == Sector::CONT_UNKNOWN)
			{
				this->cancel();
				return false;
			}
			else // sector must have blocks (WARNING assumption)
			{
				vfaces.test_z_p = (testsector.solidFlags() & 32) == 0;
				
				if (vfaces.test_z_p)
				{
					vfaces.test_z_p = 2; // contains data
					vfaces.sb_z_p   = &testsector;
				}
			}
		}
		else
		{
			vfaces.test_z_p = 0;
		} // test +z
		
		// extreme optimizations:
		// -x +x -y +y -z +z
		if (vfaces.test_x_m + vfaces.test_x_p + vfaces.test_y_m +
			vfaces.test_y_p + vfaces.test_z_m + vfaces.test_z_p == 0)
		{
			sector.culled = true; // fully hidden by other sectors
			sector.render = false;
			// a culled sector.. is completed, and completely empty
			sector.progress = Sector::PROG_COMPILED;
			this->alive = false; // job done
			return false;
		}
		
		// continue to stage 2 - generate mesh
		return true;
	}
}
