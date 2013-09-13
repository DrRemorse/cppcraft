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
		// reset drawing queue (all shaderlines)
		static void reset();
		
		// adds a column to this draw queue
		inline void add(Column* cv)
		{
			if (this->items < queue.size())
			{
				this->queue[this->items] = cv;
			}
			else queue.push_back(cv);
			
			this->items++;
		}
		
		// returns the number of items in the current queue
		inline int count() const
		{
			return this->items;
		}
		// "clears" the drawing queue
		inline void clear()
		{
			this->items = 0;
		}
		
		// returns an element from the draw queue
		inline Column* get(int index)
		{
			return this->queue[index];
		}
		
	};
	extern DrawQueue drawq[RenderConst::MAX_UNIQUE_SHADERS];
}

#endif
