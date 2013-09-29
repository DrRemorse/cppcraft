#include "lighttable.hpp"

#include "library/log.hpp"
#include "lighting.hpp"
#include "sector.hpp"
#include <cstring>

using namespace library;

namespace cppcraft
{
	PrecompScratchTable::PrecompScratchTable()
	{
		this->elements = (Sector::BLOCKS_XZ + 2) * (Sector::BLOCKS_Y + 2) * (Sector::BLOCKS_XZ + 2);
		
		inuse = BitArray(elements);
		value = new vertex_color_t[elements];
	}
	PrecompScratchTable::~PrecompScratchTable()
	{
		delete[] value;
	}
	
	void PrecompScratchTable::clear()
	{
		// clear inuse array, but NOT value array
		inuse.clear();
	}
	
	inline int element(int x, int y, int z)
	{
		return ((x + 1) * (Sector::BLOCKS_Y + 2) + (y + 1)) * (Sector::BLOCKS_XZ + 2) + (z + 1);
	}
	
	void PrecompScratchTable::set(int x, int y, int z, vertex_color_t vcolor)
	{
		int index = element(x, y, z);
		// set the inuse value (bit) to true
		inuse.set(index);
		// and set vertex color
		this->value[index] = vcolor;
	}
	
	bool PrecompScratchTable::isset(int x, int y, int z) const
	{
		return inuse[element(x, y, z)];
	}
	
	vertex_color_t PrecompScratchTable::color(int x, int y, int z) const
	{
		return this->value[element(x, y, z)];
	}
	
	vertex_color_t LightList::tableLight(Sector& sector, int x, int y, int z)
	{
		// if the table entry has already been set, 
		if (table.isset(x, y, z))
		{
			// return immediately with the precalculated complex color
			return table.color(x, y, z);
		}
		// otherwise, calculate new complex color
		vertex_color_t vcolor = Lighting.lightCheck(*this, sector, x, y, z, Lighting.ray_count);
		// set (remember) value
		table.set(x, y, z, vcolor);
		// return value
		return vcolor;
	}
	
}
