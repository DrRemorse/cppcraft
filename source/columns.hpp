#ifndef COLUMNS_HPP
#define COLUMNS_HPP

#include "library/math/vector.hpp"
#include "renderconst.hpp"
#include "sectors.hpp"

namespace cppcraft
{
	class Column
	{
	public:
		// position below Y-position when column first appearing in world
		static const float COLUMN_DEPRESSION;
		
		Column(int x, int z); // initialize column with position (x, z)
		
		bool renderable;    // true if ready to render
		bool updated;       // true if up to date
		
		// used by: Compiler pipeline
		void compile(int x, int y, int z);
		
	//private:
		unsigned int vao;
		unsigned int vbo;  // opengl renderable objects
		int wx, wz;        // world position
		library::vec3 pos; // rendering position
		//int indices  		   [RenderConst::MAX_UNIQUE_SHADERS];
		int	       bufferoffset[RenderConst::MAX_UNIQUE_SHADERS];
		int           vertices [RenderConst::MAX_UNIQUE_SHADERS];
		unsigned int  occlusion[RenderConst::MAX_UNIQUE_SHADERS];
		char          occluded [RenderConst::MAX_UNIQUE_SHADERS];
	};
	
	class ColumnsContainer
	{
	private:
		Column** columns;
		
	public:
		// number of columns
		static const int COLUMNS_Y = 4;
		// size of each column in sectors
		static const int COLUMNS_SIZE = Sectors.SECTORS_Y / COLUMNS_Y;
		
		void init();
		// column index operator
		Column& operator() (int x, int y, int z);
		
		// used by: Seamless::seamlessness()
		Column* & manipulate(int x, int y, int z);
	};
	extern ColumnsContainer Columns;
}

#endif