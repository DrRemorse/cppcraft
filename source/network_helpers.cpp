#include "network.hpp"

#include "sectors.hpp"
#include "spiders.hpp"
#include "world.hpp"

using namespace library;

namespace cppcraft
{
	PackCoord::PackCoord(double x, double y, double z)
	{
		// get integral values
		int bx = x, by = y, bz = z;
		// get sector, and truncate integrals into sector-local coordinates
		Sector* s = Spiders::spiderwrap(bx, by, bz);
		if (s == nullptr) { valid = false; return; }
		
		// set world coordinates
		wc.x = world.getWX() + s->x;
		wc.y = world.getWY() + s->y;
		wc.z = world.getWZ() + s->z;
		
		// set block coordinates (with 8-bit fractions)
		vec3 f = vec3(x, y, z).frac();
		bc.x = (bx << 8) + (f.x * 256);
		bc.y = (by << 8) + (f.y * 256);
		bc.z = (bz << 8) + (f.z * 256);
		valid = true;
	}
	PackCoord::PackCoord(int bx, int by, int bz)
	{
		// get sector, and truncate integrals into sector-local coordinates
		Sector* s = Spiders::spiderwrap(bx, by, bz);
		if (s == nullptr) { valid = false; return; }
		
		// set world coordinates
		wc.x = world.getWX() + s->x;
		wc.y = world.getWY() + s->y;
		wc.z = world.getWZ() + s->z;
		
		// set block coordinates
		bc.x = bx;
		bc.y = by;
		bc.z = bz;
		valid = true;
	}
	PackCoord::PackCoord(Sector* sector, int bx, int by, int bz)
	{
		// NOTE: may need to verify sector is not null
		// set world coordinates
		wc.x = world.getWX() + sector->x;
		wc.y = world.getWY() + sector->y;
		wc.z = world.getWZ() + sector->z;
		
		// set block coordinates
		bc.x = bx;
		bc.y = by;
		bc.z = bz;
		valid = true;
	}
	UnpackCoord::UnpackCoord(w_coord& w, b_coord& b)
	{
		// set final world coordinates
		int sx = w.x - world.getWX();
		int sy = w.y - world.getWY();
		int sz = w.z - world.getWZ();
		// validate position is inside our grid
		if (sx < 0 || sx >= Sectors.getXZ() ||
			sy < 0 || sy >= Sectors.getY() ||
			sz < 0 || sz >= Sectors.getXZ())
		{
			valid = false; return;
		}
		// set values
		this->bx = (sx << Sector::BLOCKS_XZ_SH) + b.x;
		this->by = (sy << Sector::BLOCKS_Y_SH)  + b.y;
		this->bz = (sz << Sector::BLOCKS_XZ_SH) + b.z;
		valid = true;
	}
	UnpackCoordF::UnpackCoordF(w_coord& w, b_coord& b)
	{
		wc = w;
		bc = vec3(b.x, b.y, b.z) / 256.0;
		//logger << Log::INFO << "Unpacked: " << w.x << ", " << w.y << "; " << w.z << " b " << bc.x << ", " << bc.y << ", " << bc.z << Log::ENDL;
	}
	
	NetworkBlock::NetworkBlock(int bx, int by, int bz, const Block& block, NetworkBlock::btype_t type)
	{
		PackCoord pc(bx, by, bz);
		// make a clean copy
		this->block = block;
		// set coordinates
		this->wc = pc.wc;
		this->bc = pc.bc;
		this->type = type;
	}
}
