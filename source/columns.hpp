#ifndef COLUMNS_HPP
#define COLUMNS_HPP

#include <library/math/vector.hpp>
#include "renderconst.hpp"
#include "sectors.hpp"
#include "world.hpp"

namespace cppcraft
{
	class Column
	{
	public:
		// position below Y-position when column first appearing in world
		static const float COLUMN_DEPRESSION;
		
		Column();
		
		// used by: Seamless
		inline void reset()
		{
			renderable = false;
			updated = false;
		}
		// used by: Compiler pipeline
		void compile(int x, int y, int z);
		
		// flags
		bool renderable; // is renderable
		bool updated;    // needs update
		bool hasdata;    // has uploaded gpu data
		bool aboveWater; // is above the waterline (reflectable)
		
	//private:
		unsigned int vao;
		unsigned int vbo;  // opengl renderable objects
		library::vec3 pos; // rendering position
		int	       bufferoffset[RenderConst::MAX_UNIQUE_SHADERS];
		int           vertices [RenderConst::MAX_UNIQUE_SHADERS];
		unsigned int  occlusion[RenderConst::MAX_UNIQUE_SHADERS];
		char          occluded [RenderConst::MAX_UNIQUE_SHADERS];
	};
	
	class Columns
	{
	public:
		// number of columns
		static const int COLUMNS_Y = 4;
		// size of each column in sectors
		static const int COLUMNS_SIZE = Sectors.SECTORS_Y / COLUMNS_Y;
		
		Columns();
		~Columns();
		void init();
		
		// column index operator
		inline Column& operator() (int x, int y, int z)
		{
			x = (x + world.getDeltaX()) % Sectors.getXZ();
			z = (z + world.getDeltaZ()) % Sectors.getXZ();
			
			return columns[((x * Sectors.getXZ() + z) * COLUMNS_Y) + y];
		}
		
	private:
		Column* columns;
	};
	extern Columns columns;
}

#endif
