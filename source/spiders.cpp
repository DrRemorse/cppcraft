#include "spiders.hpp"

#include "generator.hpp"
#include "lighting.hpp"
#include "player.hpp"
#include "player_physics.hpp"
#include "sectors.hpp"
#include "torchlight.hpp"

namespace cppcraft
{
	Block air_block(_AIR);
	
	// wrap absolute position
	Sector* Spiders::spiderwrap(int& bx, int& by, int& bz)
	{
		int fx = bx >> 4;
		int fy = by >> 3;
		int fz = bz >> 4;
		
		if (fx < 0 || fx >= Sectors.getXZ() || 
			fz < 0 || fz >= Sectors.getXZ() || 
			fy < 0 || fy >= Sectors.SECTORS_Y)
				return nullptr;
		
		bx &= (Sector::BLOCKS_XZ-1);
		by &= (Sector::BLOCKS_Y-1);
		bz &= (Sector::BLOCKS_XZ-1);
		return Sectors.getSectorPtr(fx, fy, fz);
	}
	
	// wrap position relative to sector
	Sector* Spiders::spiderwrap(Sector& s, int& bx, int& by, int& bz)
	{
		int fx = s.x + (bx >> 4); // Sector::BLOCKS_XZ;
		int fy = s.y + (by >> 3); // Sector::BLOCKS_Y;
		int fz = s.z + (bz >> 4); // Sector::BLOCKS_XZ;
		
		if (fx < 0 || fx >= Sectors.getXZ() || 
			fz < 0 || fz >= Sectors.getXZ() || 
			fy < 0 || fy >= Sectors.SECTORS_Y)
				return nullptr;
		
		bx &= (Sector::BLOCKS_XZ-1);
		by &= (Sector::BLOCKS_Y-1);
		bz &= (Sector::BLOCKS_XZ-1);
		return Sectors.getSectorPtr(fx, fy, fz);
	}
	
	Block& Spiders::getBlock(int x, int y, int z)
	{
		Sector* ptr = spiderwrap(x, y, z);
		if (ptr == nullptr) return air_block;
		
		if (ptr->progress == Sector::PROG_NEEDGEN)
		{
			if (ptr->contents == Sector::CONT_NULLSECTOR)
			{
				throw std::string("ERROR: Sector was nullsector (CONT_NULLSECTOR), yet needed generation (PROG_NEEDGEN).");
			}
			// generate sector
			Generator::generate(*ptr, nullptr, 0);
		}
		if (ptr->contents == Sector::CONT_NULLSECTOR) return air_block;
		
		return ptr[0](x, y, z);
	}
	
	Block& Spiders::getBlock(Sector& s, int x, int y, int z)
	{
		Sector* ptr = spiderwrap(s, x, y, z);
		if (ptr == nullptr) return air_block;
		
		if (ptr->progress == Sector::PROG_NEEDGEN)
		{
			if (ptr->contents == Sector::CONT_NULLSECTOR)
			{
				throw std::string("ERROR: Sector was nullsector (CONT_NULLSECTOR), yet needed generation (PROG_NEEDGEN).");
			}
			// generate sector
			Generator::generate(*ptr, nullptr, 0);
		}
		if (ptr->contents == Sector::CONT_NULLSECTOR) return air_block;
		
		return ptr[0](x, y, z);
	}
	Block& Spiders::getBlockNoGen(Sector& s, int x, int y, int z)
	{
		Sector* ptr = spiderwrap(s, x, y, z);
		if (ptr == nullptr) return air_block;
		if (ptr->contents < Sector::CONT_SAVEDATA) return air_block;
		
		return ptr[0](x, y, z);
	}
	
	Block& Spiders::getBlock(double x, double y, double z, double size)
	{
		// make damn sure!
		if (y < 0.0) return air_block;
		if (y >= Sectors.getY() * Sector::BLOCKS_Y) return air_block;
		
		int    by = int(y);  // integral
		double fy = y - by;  // fractional
		
		double dx, dz;
		for (dz = z-size; dz <= z+size; dz += size)
		for (dx = x-size; dx <= x+size; dx += size)
		{
			Block& b = getBlock(int(dx), by, int(dz));
			if (b.getID() != _AIR)
			{
				double fx = dx - int(dx);
				double fz = dz - int(dz);
				if (Block::blockHitbox3D(b, fx, fy, fz)) return b;
			}
		}
		return air_block;
	}
	
	block_t Spiders::testArea(double x, double y, double z)
	{
		return getBlock(x, y, z, PlayerPhysics::PLAYER_SIZE).getID();
	}
	
	block_t Spiders::testAreaEx(double x, double y, double z)
	{
		// make damn sure!
		if (y < 0.0) return _AIR;
		if (y >= Sectors.getY() * Sector::BLOCKS_Y) return _AIR;
		
		int    by = int(y);  // integral
		double fy = y - by;  // fractional
		const double PLAYER_SIZE = PlayerPhysics::PLAYER_SIZE;
		
		double dx, dz;
		for (dz = z-PLAYER_SIZE; dz <= z+PLAYER_SIZE; dz += PLAYER_SIZE)
		for (dx = x-PLAYER_SIZE; dx <= x+PLAYER_SIZE; dx += PLAYER_SIZE)
		{
			const Block& b = getBlock(int(dx), by, int(dz));
			if (b.getID() != _AIR)
			{
				double fx = dx - int(dx);
				double fz = dz - int(dz);
				if (Block::physicalHitbox3D(b, fx, fy, fz))
				{
					if (Block::movementTest(b.getID())) return b.getID();
				}
			} // not air
		} // 3x3 test grid
		return _AIR;
	}
	
	Sector* lastGatheredSector = nullptr;
	LightList lightNowList;
	
	vertex_color_t Spiders::getLightNow(double x, double y, double z)
	{
		if (y <= 0) return 0; // mega-dark
		if (y >= Sectors.getY() * Sector::BLOCKS_Y) return 255 << 8;
		
		int ix = x, iy = y, iz = z;
		Sector* sector = Spiders::spiderwrap(ix, iy, iz);
		// not really cause to exit, since torchlights can still affect someone barely outside of world,
		// but whatever, we exit when out of bounds
		if (sector == nullptr) return 0;
		
		if (lastGatheredSector != sector)
		{
			lastGatheredSector = sector;
			// clear lights
			lightNowList.lights.clear();
			// gather torchlights for sector
			torchlight.lightGatherer(*sector, lightNowList);
		}
		// return calculated shadows & lighting
		return Lighting.lightCheck(lightNowList, *sector, ix, iy, iz, 1);
	}
	
}
