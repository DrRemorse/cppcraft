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
		typedef struct
		{
			int x, y, z;
			
		} columnqueue_t;
		
		void initCompilers();
		void compile();
		
		int colqCount();
		columnqueue_t& get(int i);
		void remove(int i);
		
	private:
		void sectorCompiler(Sector& s, Precomp& pc);
		
		// simple column queue
		std::vector<columnqueue_t> colq;
	};
	extern Compilers compilers;
}

#endif
