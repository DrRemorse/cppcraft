#ifndef PRECOMPILER_HPP
#define PRECOMPILER_HPP

#define USE_BIOMEDATA

#include "blocks.hpp"
#include "renderconst.hpp"
#include "vertex_block.hpp"
#include <vector>

namespace cppcraft
{
	class Sector;
	
	class Precomp
	{
	public:
		Precomp();
		~Precomp();
		
		enum jobresult_t
		{
			STATUS_NEW,
			STATUS_CULLED,
			STATUS_FAILED,
			STATUS_DONE
		};
		
		inline bool isAlive() const
		{
			return alive;
		}
		inline jobresult_t getResult() const
		{
			return result;
		}
		inline int getJob() const
		{
			return job;
		}
		
		bool isolator();
		//! try to complete a column and schedule it for transfer to renderer
		void complete();
		
		void cancel();
		void kill();
		
		bool        alive;
		jobresult_t result;
		// the job we are to do
		int         job;
		// testdata from isolator
		visiblefaces_t vfaces;
		// sector as mesh source
		Sector*   sector;
		// resulting mesh data
		vertex_t* datadump;
		// resulting index data
		indice_t* indidump;
		indice_t indices     [RenderConst::MAX_UNIQUE_SHADERS];
		
		unsigned short vertices    [RenderConst::MAX_UNIQUE_SHADERS];
		unsigned short bufferoffset[RenderConst::MAX_UNIQUE_SHADERS];
	};
	
	class Precompiler
	{
	public:
		static const int MAX_PRECOMPQ = 64;
		
		inline Precomp& operator[] (std::size_t i)
		{
			return this->queue[i];
		}
		
		// initializes the precompiler pipeline stage
		void init();
		
		// executes a round of precompilation
		void run();
		
	private:
		Precomp queue[MAX_PRECOMPQ];
	};
	extern Precompiler precompiler;
	
}

#endif
