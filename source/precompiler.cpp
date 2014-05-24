#include "precompiler.hpp"

#include <library/log.hpp>
#include <library/config.hpp>
#include "blockmodels.hpp"
#include "columns.hpp"
#include "precomp_thread.hpp"
#include "precompq_schedule.hpp"
#include "sectors.hpp"
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
		this->pcthread_count = config.get("jobs", 4);
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
			1500, // TX_LAVA
			2000, // TX_WATER
			 500  // TX_RUNNING_WATER
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
	
	Precompiler::~Precompiler()
	{
		delete[] pcthreads;
	}
	
	PrecompThread& Precompiler::getJob(int job)
	{
		return pcthreads[job];
	}
	
	Precomp::~Precomp()
	{
		delete[] this->datadump;
	}
	
	// tries to complete a precompilation job
	void Precomp::complete()
	{
		Sector& sector = *this->sector;
		
		// renderable VBO structure
		int cy = sector.y & (Columns::COLUMNS_SIZE-1);
		Column& cv = columns(sector.x, sector.y / Columns::COLUMNS_SIZE, sector.z);
		vbodata_t& v = cv.vbodata[cy];
		
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
		// sector was definitely not culled
		sector.culled = false;
		
		PrecompScheduler::add(sector);
	}
}
