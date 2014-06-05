#include "lighttable.hpp"

#include <library/log.hpp>
#include "lighting.hpp"
#include "sector.hpp"
#include <cstring>

using namespace library;

namespace cppcraft
{
	PrecompScratchTable::PrecompScratchTable()
	{
		this->elements = ST_AXIS_XZ * ST_AXIS_Y * ST_AXIS_XZ;
		
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
	
	void PrecompScratchTable::set(int x, int y, int z, vertex_color_t vcolor)
	{
		int index = element(x, y, z);
		// set the inuse value (bit) to true
		inuse.set(index);
		// and set vertex color
		this->value[index] = vcolor;
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
