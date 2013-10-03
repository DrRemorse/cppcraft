#include "compilers.hpp"

#include "library/log.hpp"
#include "columns.hpp"
#include "precompiler.hpp"
#include "threading.hpp"
#include "vertex_block.hpp"
#include <mutex>

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
		mtx.compiler.lock();
		
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
			{
				remove(i);
			}
			
		}
		
		mtx.compiler.unlock();
		
	} // handleCompilers
	
	int Compilers::colqCount()
	{
		return colq.size();
	}
	
	Compilers::columnqueue_t& Compilers::get(int i)
	{
		return colq[i];
	}
	
	void Compilers::add(int x, int y, int z)
	{
		colq.emplace_back( (columnqueue_t) { x, y, z } );
	}
	
}
