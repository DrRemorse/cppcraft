#ifndef DRAWQ_HPP
#define DRAWQ_HPP

#include "renderconst.hpp"
#include <vector>

namespace cppcraft
{
	class Column;
	
	class DrawQueue
	{
	private:
		// queue
		std::vector<Column*> queue;
		unsigned int items;
		
	public:
		// initialize drawing queue
		static void init();
		// reset drawing queue
		static void reset();
		
		void add(Column*);
		int  count();
		void clear();
		Column* get(int index);
	};
	extern DrawQueue drawq[RenderConst::MAX_UNIQUE_SHADERS];
}

#endif
