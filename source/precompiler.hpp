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
		Precomp() {}
		~Precomp();
		
		bool alive;
		Sector* sector;
		
		// mesh data
		vertex_t* datadump;
		// fixed size light data
		//lightdata_t* lightdump;
		
		#ifdef USE_INDEXING
			indice_t*       indidump;
			unsigned short* indices;
		#endif
		
		unsigned short vertices    [RenderConst::MAX_UNIQUE_SHADERS];
		unsigned short bufferoffset[RenderConst::MAX_UNIQUE_SHADERS];
		
		void complete();
	};
	
	class Precompiler
	{
	public:
		static const int MAX_PRECOMPQ = 256;
		
		~Precompiler();
		
		inline Precomp& operator[] (std::size_t i)
		{
			return this->queue[i];
		}
		
		// initializes the precompiler pipeline stage
		void init();
		
		// number of precompiler threads
		inline std::size_t getThreads()
		{
			return pcthread_count;
		}
		// returns a precompilation thread object
		PrecompThread& getThread(int t);
		// executes a round of precompilation
		void run();
		
	private:
		Precomp queue[MAX_PRECOMPQ];
		
		// precompilation threads
		PrecompThread* pcthreads;
		int pcthread_count;
		
	};
	extern Precompiler precompiler;
	
}

#endif
