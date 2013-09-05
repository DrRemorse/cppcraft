#include "precompiler.hpp"

#include "library/config.hpp"
#include "library/log.hpp"
#include "vertex_block.hpp"
#include "blockmodels.hpp"
#include "precomp_thread.hpp"
#include "torchlight.hpp"

#include <string>

using namespace library;

namespace cppcraft
{
	// precompiler manager
	Precompiler precompiler;
	// precompilation threads
	std::vector<PrecompThread> pcthreads;
	
	// the precompiler uses a set amount of threads to function
	// on initialization each data-segment belonging to each thread
	// must be allocated
	
	void Precompiler::init()
	{
		logger << Log::INFO << "* Initializing block meshes" << Log::ENDL;
		// construct all mesh objects
		blockmodels.init();
		
		logger << Log::INFO << "* Initializing torchlight" << Log::ENDL;
		torchlight.init();
		
		logger << Log::INFO << "* Initializing precompiler" << Log::ENDL;
		int NUM_PCTHREADS = config.get("threading", 2);
		
		const int pipelineSize[RenderConst::MAX_UNIQUE_SHADERS] =
		{
			6000, // TX_REPEAT
			7000, // TX_SOLID
			3000, // TX_TRANS
			8000, // TX_2SIDED
			4000, // TX_CROSS
			4000  // TX_WATER
		};
		
		for (int t = 0; t < NUM_PCTHREADS; t++)
		{
			// create precomiler thread objects
			pcthreads.emplace_back(PrecompThread());
			// initialize each shaderline
			for (int i = 0; i < RenderConst::MAX_UNIQUE_SHADERS; i++)
			{
				// initialize thread object
				pcthreads[t].pcg.databuffer[i] = new vertex_t[pipelineSize[i]];
			}
		}
		
	}
	
	std::size_t Precompiler::getThreads()
	{
		return pcthreads.size();
	}
	
	PrecompThread& Precompiler::getThread(int t)
	{
		return pcthreads[t];
	}
	
	Precomp& Precompiler::operator [] (unsigned int i)
	{
		return this->queue[i];
	}
	
}