#ifndef COMPILERS_HPP
#define COMPILERS_HPP

/**
 * 
 * 
 * 
**/

#include <vector>

namespace cppcraft
{
	static const double MAX_RENDERTHREAD_WAIT = 0.005;
	
	class Sector;
	class Precomp;
	
	class Compilers
	{
	public:
		void initCompilers();
		void compile();
		
		inline int colqCount() const
		{
			return colq.size();
		}
		inline void add(const Sector* sector)
		{
			// add to queue
			colq.push_back( sector );
		}
		inline void clear()
		{
			colq.clear();
		}
		
	private:
		inline void remove(int i)
		{
			colq.erase(colq.begin() + i);
		}
		
		// simple column queue
		std::vector<const Sector*> colq;
	};
	extern Compilers compilers;
}

#endif
