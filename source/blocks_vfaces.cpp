#include "blocks.hpp"

#include "sectors.hpp"
#include "spiders.hpp"

namespace cppcraft
{
	// returns visible sides of this cube, relying heavily on the provided testdata
	// which contains the 'current' sector which this block belongs to,
	// and all the neighboring sectors
	unsigned short Block::visibleFaces(visiblefaces_t& pcg, int bx, int by, int bz) const
	{
		unsigned short lbx = 0;
		
		// left side -x
		if (bx == 0) // facing = 5
		{
			if (pcg.test_x_m == 1) // nullsector
			{
				lbx |= 32;  // true
			}
			else if (pcg.test_x_m == 2) // contains data
			{
				if (visibilityComp(pcg.sb_x_m[0](Sector::BLOCKS_XZ-1, by, bz), 3))
					lbx |= 32;  // true
			}
		}
		else
		{
			if (visibilityComp(pcg.sector[0](bx-1, by, bz), 3))
				lbx |= 32; // true
		}
		
		// right side +x
		if (bx == Sector::BLOCKS_XZ-1) // facing = 4
		{
			if (pcg.test_x_p == 1) // nullsector
			{
				lbx |= 16; // true
			}
			else if (pcg.test_x_p == 2) // contains data
			{
				if (visibilityComp(pcg.sb_x_p[0](0, by, bz), 2))
					lbx |= 16; // true
			}
		}
		else
		{
			if (visibilityComp(pcg.sector[0](bx+1, by, bz), 2))
				lbx |= 16; // true
		}
		
		// bottom -y
		if (by == 0)
		{
			if (pcg.test_y_m == 1) // nullsector
			{
				lbx |= 8;
			}
			else if (pcg.test_y_m == 2) // contains data
			{
				if (visibilityCompBottom(pcg.sb_y_m[0](bx, Sector::BLOCKS_Y-1, bz)))
					lbx |= 8;
			}
		}
		else
		{
			if (visibilityCompBottom(pcg.sector[0](bx, by-1, bz))) 
				lbx |= 8;
		}
		
		// top +y
		if (by == Sector::BLOCKS_Y-1)
		{
			if (pcg.test_y_p == 1) // nullsector
			{
				lbx |= 4;
			}
			else if (pcg.test_y_p == 2) // contains data
			{
				if (visibilityCompTop(pcg.sb_y_p[0](bx, 0, bz)))
					lbx |= 4;
			}
		}
		else
		{
			if (visibilityCompTop(pcg.sector[0](bx, by+1, bz)))
				lbx |= 4;
		}
		
		// front +z
		if (bz == 0)
		{
			if (pcg.test_z_m == 1) // nullsector
			{
				lbx |= 2;
			}
			else if (pcg.test_z_m == 2) // contains data
			{
				if (visibilityComp(pcg.sb_z_m[0](bx, by, Sector::BLOCKS_XZ-1), 1))
					lbx |= 2;
			}
		}
		else
		{
			if (visibilityComp(pcg.sector[0](bx, by, bz-1), 1))
				lbx |= 2;
		}
		
		if (bz == Sector::BLOCKS_XZ-1)
		{
			if (pcg.test_z_p == 1) // nullsector
			{
				lbx |= 1;
			}
			else if (pcg.test_z_p == 2) // contains data
			{
				if (visibilityComp(pcg.sb_z_p[0](bx, by, 0), 0))
					lbx |= 1;
			}
		}
		else
		{
			if (visibilityComp(pcg.sector[0](bx, by, bz+1), 0))
				lbx |= 1;
		}
		return lbx;
	}
	
	unsigned short Block::visibleFaces(Sector& sector, int bx, int by, int bz) const
	{
		unsigned short lbx = 0;
		
		// left side -x
		if (bx == 0) // facing = 5
		{
			if (sector.getX() > 0)
			{
				Sector& s = Sectors(sector.getX()-1, sector.getY(), sector.getZ());
				if (s.contents > Sector::CONT_UNKNOWN)
				{
					if (visibilityComp(s(Sector::BLOCKS_XZ-1, by, bz), 3))
						lbx |= 32;
				}
				else lbx |= 32;
			}
		}
		else
		{
			if (visibilityComp(sector(bx-1, by, bz), 3))
				lbx |= 32;
		}
		
		// right side +x
		if (bx == Sector::BLOCKS_XZ-1) // facing = 4
		{
			if (sector.getX() < Sectors.getXZ()-1)
			{
				Sector& s = Sectors(sector.getX()+1, sector.getY(), sector.getZ());
				if (s.contents > Sector::CONT_UNKNOWN)
				{
					if (visibilityComp(s(0, by, bz), 2))
						lbx |= 16;
				}
				else lbx |= 16;
			}
		}
		else
		{
			if (visibilityComp(sector(bx+1, by, bz), 2))
				lbx |= 16;
		}
		
		// bottom -y
		if (by == 0)
		{
			if (sector.getY() > 0)
			{
				Sector& s = Sectors(sector.getX(), sector.getY()-1, sector.getZ());
				if (s.contents > Sector::CONT_UNKNOWN)
				{
					if (visibilityCompBottom(s(bx, Sector::BLOCKS_Y-1, bz)))
						lbx |= 8;
				}
				else lbx |= 8;
			}
		}
		else
		{
			if (visibilityCompBottom(sector(bx, by-1, bz)))
				lbx |= 8;
		}
		
		// top +y
		if (by == Sector::BLOCKS_Y-1)
		{
			if (sector.getY() < Sectors.getY()-1)
			{
				Sector& s = Sectors(sector.getX(), sector.getY()+1, sector.getZ());
				if (s.contents > Sector::CONT_UNKNOWN)
				{
					if (visibilityCompTop(s(bx, 0, bz)))
						lbx |= 4;
				}
				else lbx |= 4;
			}
			//else lbx |= 4; // top of the world will still add mesh
		}
		else
		{
			if (visibilityCompTop(sector(bx, by+1, bz)))
				lbx |= 4;
		}
		
		// back -z
		if (bz == 0)
		{
			if (sector.getZ() > 0)
			{
				Sector& s = Sectors(sector.getX(), sector.getY(), sector.getZ()-1);
				if (s.contents > Sector::CONT_UNKNOWN)
				{
					if (visibilityComp(s(bx, by, Sector::BLOCKS_XZ-1), 1))
						lbx |= 2;
				}
				else lbx |= 2;
			}
		}
		else
		{
			if (visibilityComp(sector(bx, by, bz-1), 1))
				lbx |= 2;
		}
		
		if (bz == Sector::BLOCKS_XZ-1)
		{
			if (sector.getZ() < Sectors.getXZ()-1)
			{
				Sector& s = Sectors(sector.getX(), sector.getY(), sector.getZ()+1);
				if (s.contents > Sector::CONT_UNKNOWN)
				{
					if (visibilityComp(s(bx, by, 0), 0))
						lbx |= 1;
				}
				else lbx |= 1;
			}
		}
		else
		{
			if (visibilityComp(sector(bx, by, bz+1), 0))
				lbx |= 1;
		}
		
		return lbx;
	}
	
}