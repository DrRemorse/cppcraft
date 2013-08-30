#include "compilers.hpp"

#include "library/log.hpp"
#include "columns.hpp"
#include "precompiler.hpp"
#include "threading.hpp"
#include "vertex_block.hpp"

using namespace library;

namespace cppcraft
{
	// instantiation of compilers
	Compilers compilers;
	
	// initialize compiler data buffers
	void Compilers::initCompilers()
	{
		logger << Log::INFO << "* Initializing compilers" << Log::ENDL;
		Columns.init();
		colq.clear();
	}
	
	// run compilers and try to clear queue, if theres enough time
	void Compilers::compile()
	{
		// precompile queue
		for (unsigned int i = 0; i < Precompiler::MAX_PRECOMPQ; i++)
		{
			Precomp& p = precompiler[i];
			
			// regular precomps
			if (p.alive)
			{
				//mtx.precompiler.lock();
				Sector* s = p.sector;
				if (s->precomp == 3)
				{
					//mtx.precompiler.unlock();
					sectorCompiler(*s, p);
					//if (colq_count == max_colq) break;
				}
				//else mtx.precompiler.unlock();
			}
		}
		
		// no reason to continue if theres nothing in the queue
		if (colq.size() == 0) return;
		
		// column assemblage
		// also, prune array removing dead columns
		for (int i = colq.size()-1; i >= 0; i--)
		{
			Column& cv = Columns(colq[i].x, colq[i].y, colq[i].z);
			
			if (cv.updated == true)
			{
				cv.compile(colq[i].x, colq[i].y, colq[i].z);
			}
			if (cv.updated == false)
			{	// remove columns from queue that no longer needs to be updated
				colq.erase(colq.begin() + i);
			}
		}
			
	} // handleCompilers
	
	int Compilers::colqCount()
	{
		return colq.size();
	}
	
	Compilers::columnqueue_t& Compilers::get(int i)
	{
		return colq[i];
	}
	
	void Compilers::remove(int i)
	{
		colq.erase(colq.begin() + i);
	}
	
	// allocates VBO renderable structure
	// copies pointers from precomp to vbo
	// invalidates precomp, adds vbodata to column assembly queue
	
	void Compilers::sectorCompiler(Sector& s, Precomp& pc)
	{
		// allocate sector vertex data
		if (s.vbodata == nullptr)
		{
			s.vbodata = new vbodata_t;
			s.vbodata->pcdata = nullptr;
		}
		
		// renderable VBO structure
		vbodata_t* v = s.vbodata;
		
		// copy info from precomp to vbodata struct
		for (int n = 0; n < RenderConst::MAX_UNIQUE_SHADERS; n++)
		{
			v->vertices[n]     = pc.vertices[n];
			v->bufferoffset[n] = pc.bufferoffset[n];
		}
		
		// ye olde switcharoo
		delete v->pcdata;         // remove old data (if any)
		v->pcdata = pc.datadump;  // set vbodata to become precomp data
		pc.datadump = nullptr;    // unassign precomp data pointer
		
		// set renderable flag to sector
		s.render = true;
		// set progress to finished state
		if (s.progress == Sector::PROG_NEEDCOMPILE)
		{
			s.progress = Sector::PROG_COMPILED;
		}
		
		// free precomp slot
		pc.alive  = false;
		// reset sector state to unqueued
		s.precomp = 0;
		
		// check if this column may be complete (ready to be compiled)
		int cy = s.y / Columns.COLUMNS_SIZE;
		
		// check all sectors that belong to column
		/*
		int start_y = cy * Columns.COLUMNS_SIZE;
		int end_y = start_y + Columns.COLUMNS_SIZE;
		bool ready = true;
		
		for (int y = end_y - 1; y >= start_y; y--)
		{
			Sector& sector = Sectors(s.x, y, s.z);
			
			if (sector.render)
			{
				// a renderable without VBO data, is not a renderable!
				if (sector.vbodata == nullptr)
				{
					sector.progress = Sector::PROG_NEEDRECOMP;
					ready = false;
				}
			}
			
		}
		if (ready == false) return;
		*/
		Column& cv = Columns(s.x, cy, s.z);
		// if the column wasn't previously updated
		// and we (think we) are ready to compile
		if (cv.updated == false)
		{
			// add to queue
			colq.emplace_back( columnqueue_t {s.x, cy, s.z} );
			// set as recently "updated"
			cv.updated = true;
		}
		
	} // sectorCompiler
	
}