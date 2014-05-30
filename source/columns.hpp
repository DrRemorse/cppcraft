#ifndef COLUMNS_HPP
#define COLUMNS_HPP

#include <library/math/vector.hpp>
#include "renderconst.hpp"
#include "sectors.hpp"
#include "world.hpp"

namespace cppcraft
{
	struct vertex_t;
	
	typedef struct vbodata_t
	{
		vertex_t* pcdata;
		unsigned short bufferoffset[RenderConst::MAX_UNIQUE_SHADERS];
		unsigned short vertices	[RenderConst::MAX_UNIQUE_SHADERS];
	} vbodata_t;
	
	class Column
	{
	public:
		// position below Y-position when column first appearing in world
		static const float COLUMN_DEPRESSION;
		
		Column();
		~Column();
		
		// used by: Seamless
		inline void reset()
		{
			renderable = false;
			updated = false;
		}
		// used by: Compiler pipeline
		void compile(int x, int y, int z);
		
		// opengl vbo data section
		vbodata_t* vbodata;
		
		// flags
		bool renderable; // is renderable
		bool updated;    // needs update
		bool hasdata;    // has uploaded gpu data
		bool aboveWater; // is above the waterline (reflectable)
		
	//private:
		unsigned int  vao;
		unsigned int  vbo; // opengl renderable objects
		library::vec3 pos; // rendering position
		int	       bufferoffset[RenderConst::MAX_UNIQUE_SHADERS];
		int           vertices [RenderConst::MAX_UNIQUE_SHADERS];
		unsigned int  occlusion[RenderConst::MAX_UNIQUE_SHADERS];
		bool          occluded [RenderConst::MAX_UNIQUE_SHADERS];
	};
	
	class Columns
	{
	public:
		Columns();
		~Columns();
		void init();
		
		inline int getColumnsY() const
		{
			return height;
		}
		inline int getSectorLevel(int y)
		{
			// 8 == RenderConst::WATER_SECTOR_LEVEL
			return (y == 0) ? 0 : 8;
		}
		inline int getSizeInSectors(int y) const
		{
			return (y == 0) ? 8 : Sectors.getY() - 8;
		}
		int fromSectorY(int y)
		{
			return (y < 8) ? 0 : 1;
		}
		int internalSectorY(int y)
		{
			return (y < 8) ? y : y - 8;
		}
		
		// column index operator
		inline Column& operator() (int x, int y, int z)
		{
			x = (x + world.getDeltaX()) % Sectors.getXZ();
			z = (z + world.getDeltaZ()) % Sectors.getXZ();
			
			return columns[((x * Sectors.getXZ() + z) * height) + y];
		}
		
	private:
		Column* columns; // array of all the columns
		// number of columns on Y-axis
		static const int height = 2;
		// size of a single column in sectors
		//static const int sizeSectors = Sectors.SECTORS_Y / height;
	};
	extern Columns columns;
	
}

#endif
