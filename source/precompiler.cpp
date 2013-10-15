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

#include "precompq.hpp"

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
		
		// set progress to finished state
		sector.progress = Sector::PROG_COMPILED;
		// set renderable flag to sector
		sector.render = true;
		
		// determine readiness of column before sending to compiler
		// first sector & end iterator in column
		int start_y = sector.y - (sector.y & (Columns.COLUMNS_SIZE-1));
		int end_y   = start_y + Columns.COLUMNS_SIZE;
		//bool ready = true;
		
		for (int y = start_y; y < end_y; y++)
		{
			Sector& s2 = Sectors(sector.x, y, sector.z);
			
			if (s2.progress != Sector::PROG_COMPILED)
			{
				// avoid resetting the sector compiled status
				//ready = false;
			}
			else if (s2.render)
			{
				// a renderable sector without VBO data, is not renderable!
				if (s2.vbodata == nullptr)
				{
					//logger << Log::WARN << "Precomp::complete(): rescheduling renderable" << Log::ENDL;
					s2.progress = Sector::PROG_NEEDRECOMP;
					//ready = false;
				}
				else if (s2.vbodata->pcdata == nullptr)
				{
					logger << Log::ERR << "Precomp::complete(): vertex data was null" << Log::ENDL;
					//ready = false;
				}
			}
		}
		
		// only add sector to column / compiler queue when ready
		//if (ready == false) return;
		
		/// add to compiler queue ///
		mtx.compiler.lock();
		
		int cy = sector.y / Columns.COLUMNS_SIZE;
		
		Column& cv = Columns(sector.x, cy, sector.z);
		if (cv.updated == false)
		{
			cv.updated = true;
			
			// last: add to queue
			compilers.add(sector.x, cy, sector.z);
		}
		
		mtx.compiler.unlock();
	}
}
