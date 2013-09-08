#ifndef PRECOMPILER_HPP
#define PRECOMPILER_HPP

#define USE_BIOMEDATA

#include "blocks.hpp"
#include "renderconst.hpp"
#include <vector>

namespace cppcraft
{
	class Sector;
	class PrecompThread;
	struct vertex_t;
	
	class Precomp
	{
	public:
		bool alive;
		Sector* sector;
		vertex_t* datadump;
		
		#ifdef USE_INDEXING
			indice_t*       indidump;
			unsigned short* indices;
		#endif
		
		unsigned short vertices    [RenderConst::MAX_UNIQUE_SHADERS];
		unsigned short bufferoffset[RenderConst::MAX_UNIQUE_SHADERS];
	};
	
	class Precompiler
	{
	public:
		static const int MAX_PRECOMPQ = 64;
		
		Precomp& operator[] (unsigned int);
		
		// initializes the precompiler pipeline stage
		void init();
		
		// number of precompiler threads
		std::size_t getThreads();
		// returns a precompilation thread object
		PrecompThread& getThread(int);
		// executes a round of precompilation
		void run();
		
	private:
		Precomp queue[MAX_PRECOMPQ];
		
	};
	extern Precompiler precompiler;
	
}

#endif
