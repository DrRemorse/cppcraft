#include "precompiler.hpp"

#include "library/config.hpp"
#include "library/log.hpp"
#include "vertex_block.hpp"
#include "blockmodels.hpp"
#include "columns.hpp"
#include "compilers.hpp"
#include "precomp_thread.hpp"
#include "sectors.hpp"
#include "threading.hpp"
#include "torchlight.hpp"
#include <string>

using namespace library;

namespace cppcraft
{
	// precompiler manager
	Precompiler precompiler;
	
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
		
		// create precomiler thread objects
		this->pcthread_count = config.get("threading", 2);
		this->pcthreads = new PrecompThread[pcthread_count]();
		
		// random default values for vertex array sizes
		// the arrays get automatically resized as needed
		int pipelineSize[RenderConst::MAX_UNIQUE_SHADERS] =
		{
			5000, // TX_REPEAT
			5000, // TX_SOLID
			8000, // TX_TRANS
			1500, // TX_2SIDED
			1500, // TX_CROSS
			2000  // TX_WATER
		};
		
		for (int t = 0; t < pcthread_count; t++)
		{
			for (int i = 0; i < RenderConst::MAX_UNIQUE_SHADERS; i++)
			{
				// set initial shaderline sizes
				pcthreads[t].pcg.pipelineSize[i] = pipelineSize[i];
				// initialize each shaderline
				pcthreads[t].pcg.databuffer[i] = new vertex_t[pcthreads[t].pcg.pipelineSize[i]];
			}
		}
		
	}
	
	std::size_t Precompiler::getThreads()
	{
		return pcthread_count;
	}
	
	PrecompThread& Precompiler::getThread(int t)
	{
		return pcthreads[t];
	}
	
	Precomp::~Precomp()
	{
		delete[] this->datadump;
	}
	
	Precompiler::~Precompiler()
	{
		delete[] pcthreads;
	}
	
	Precomp& Precompiler::operator [] (unsigned int i)
	{
		return this->queue[i];
	}
	
	// tries to complete a precompilation job
	void Precomp::complete()
	{
		Sector& sector = *this->sector;
		
		mtx.compiler.lock();
		
		// allocate sector vertex data
		if (sector.vbodata == nullptr)
		{
			sector.vbodata = new vbodata_t;
			sector.vbodata->pcdata = nullptr;
		}
		
		// renderable VBO structure
		vbodata_t& v = *sector.vbodata;
		
		// ye olde switcharoo
		delete[] v.pcdata;         // remove old data (if any)
		v.pcdata = this->datadump; // set vbodata to become precomp data
		this->datadump = nullptr;  // unassign precomp data pointer
		
		// copy info from precomp to vbodata struct
		for (int n = 0; n < RenderConst::MAX_UNIQUE_SHADERS; n++)
		{
			v.vertices[n]     = this->vertices[n];
			v.bufferoffset[n] = this->bufferoffset[n];
		}
		
		// free precomp slot (as early as possible)
		this->alive = false;
		// reset sector state to unqueued
		sector.precomp = 0;
		
		// set renderable flag to sector
		sector.render = true;
		// set progress to finished state (if possible)
		if (sector.progress == Sector::PROG_NEEDCOMPILE)
		{
			sector.progress = Sector::PROG_COMPILED;
		}
		
		mtx.compiler.unlock();
		
		/// add to compiler queue ///
		int cy = sector.y / Columns.COLUMNS_SIZE;
		
		Column& cv = Columns(sector.x, cy, sector.z);
		cv.updated = true;
		
		// last: add to queue
		compilers.add(sector.x, cy, sector.z);
		
	}
}
