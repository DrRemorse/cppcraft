#include "precompiler.hpp"

#include <library/log.hpp>
#include <library/config.hpp>
#include "blockmodels.hpp"
#include "columns.hpp"
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
	}
	
	Precomp::Precomp()
	{
		this->alive    = false;
		this->result   = STATUS_NEW;
		this->sector   = nullptr;
		this->datadump = nullptr;
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
		int columnY = columns.fromSectorY(sector.getY());
		int internalY = columns.internalSectorY(sector.getY());
		Column& cv = columns(sector.getX(), columnY, sector.getZ());
		vbodata_t& v = cv.vbodata[internalY];
		
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
		
		// set progress to finished state
		sector.progress = Sector::PROG_COMPILED;
		// set renderable flag to sector
		sector.render = true;
		// sector was definitely not culled
		sector.culled = false;
		// disable precomp
		this->alive = false;
		
		PrecompScheduler::add(Sectors(sector.getX(), columnY, sector.getZ()));
	}
}
