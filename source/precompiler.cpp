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
#include <initializer_list>
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
		
		auto init = std::initializer_list<int>
		({
			5000, // TX_REPEAT
			5000, // TX_SOLID
			8000, // TX_TRANS
			1500, // TX_2SIDED
			1500, // TX_CROSS
			2000  // TX_WATER
		});
		
		for (int t = 0; t < NUM_PCTHREADS; t++)
		{
			// create precomiler thread objects
			pcthreads.emplace_back();
			// set initial shaderline sizes
			std::copy(init.begin(), init.end(), pcthreads[t].pcg.pipelineSize);
			// initialize each shaderline
			for (int i = 0; i < RenderConst::MAX_UNIQUE_SHADERS; i++)
			{
				// initialize thread object
				pcthreads[t].pcg.databuffer[i] = new vertex_t[pcthreads[t].pcg.pipelineSize[i]];
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
	
	Precomp::~Precomp()
	{
		delete[] this->datadump;
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
