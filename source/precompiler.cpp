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
		this->indidump = nullptr;
	}
	Precomp::~Precomp()
	{
		delete[] this->datadump;
		delete[] this->indidump;
	}
	
	// tries to complete a precompilation job
	void Precomp::complete()
	{
		Sector& sector = *this->sector;
		
		// column mesh belongs to
		int columnY = columns.fromSectorY(sector.getY());
		Column& cv = columns(sector.getX(), columnY, sector.getZ());
		
		// mesh structure (vbodata)
		int internalY = columns.internalSectorY(sector.getY());
		vbodata_t& v = cv.vbodata[internalY];
		
		// ye olde switcharoo
		delete[] v.pcdata;         // remove old data (if any) from column
		v.pcdata = this->datadump; // set to same as precomp data pointer
		this->datadump = nullptr;  // unassign precomp data pointer
		
		delete[] v.indexdata;      // remove old data (if any) from column
		v.indexdata = this->indidump; // set to same as precomp data pointer
		this->indidump = nullptr;  // unassign precomp data pointer
		
		// copy info from precomp to vbodata struct
		for (int n = 0; n < RenderConst::MAX_UNIQUE_SHADERS; n++)
		{
			// vertices
			v.vertices[n]     = this->vertices[n];
			v.bufferoffset[n] = this->bufferoffset[n];
			// and indices (must be sequential memory)
			v.indices[n]     = this->indices[n];
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
