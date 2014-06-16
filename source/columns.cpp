#include "columns.hpp"

#include <library/log.hpp>
#include <library/opengl/opengl.hpp>
#include "camera.hpp"
#include "renderconst.hpp"
#include "threading.hpp"
#include "vertex_block.hpp"
#include <cstring>

using namespace library;

namespace cppcraft
{
	const float Column::COLUMN_DEPRESSION = 8.0f;
	
	// all the columns you'll ever need
	Columns columns;
	
	// list of metadata that add up to a complete column VBO
	vbodata_t* vboList;
	// column compiler accumulation buffer
	vertex_t*  column_dump;
	
	void Columns::init()
	{
		logger << Log::INFO << "* Initializing columns" << Log::ENDL;
		
		//////////////////////
		// allocate columns //
		//////////////////////
		int num_columns = Sectors.getXZ() * Sectors.getXZ() * this->height;
		this->columns = 
			new Column[num_columns]();
		
		//////////////////////////////////////////////////////////////
		// determine if above water and allocate metadata container //
		//////////////////////////////////////////////////////////////
		for (int i = 0; i < num_columns; i++)
		{
			int y = i % this->height;
			// the column is above water if the first sector is >= water level
			columns[i].aboveWater = 
				(getSectorLevel(y) * Sector::BLOCKS_Y >= RenderConst::WATER_LEVEL);
			// determine column size (in sectors)
			columns[i].vbodata = new vbodata_t[getSizeInSectors(y)]();
		}
	}
	Columns::Columns()
	{
		this->height = 3;
		
		sectorLevels = new int[this->height];
		sectorLevels[0] = 0;
		sectorLevels[1] = 7;
		sectorLevels[2] = 8;
		
		sectorSizes = new int[this->height];
		sectorSizes[0] = 7;
		sectorSizes[1] = 1;
		sectorSizes[2] = Sectors.getY() - 8;
		
		// NOTE: ASSUMPTION -- TOP COLUMN IS TALLEST
		int tallest = sectorSizes[height - 1];
		
		////////////////////////////////////////////////////////
		// allocate temporary datadumps for compiling columns //
		////////////////////////////////////////////////////////
		
		vboList = new vbodata_t[tallest];
		
		column_dump = 
			new vertex_t[tallest * RenderConst::MAX_FACES_PER_SECTOR * 4];
		
	}
	Columns::~Columns()
	{
		int num_columns = Sectors.getXZ() * Sectors.getXZ() * this->height;
		for (int i = 0; i < num_columns; i++)
		{
			int y = i % this->height;
			columns[i].reset(y);
		}
		delete[] columns;
		delete[] sectorLevels;
		delete[] sectorSizes;
		// compile stuff
		delete[] vboList;
		delete[] column_dump;
	}
	
	Column::Column()
	{
		// initialize VAO to 0, signifying a column without valid GL resources
		this->vao = 0;
		// set initial flags
		this->updated    = false;
		this->renderable = false;
		this->hasdata = false;
	}
	Column::~Column()
	{
		delete[] this->vbodata;
	}
	
	inline void Column::deleteData(int y)
	{
		for (int sy = columns.getSizeInSectors(y)-1; sy >= 0; sy--)
		{
			delete[] vbodata[sy].pcdata;
			vbodata[sy].pcdata = nullptr;
		}
	}
	void Column::reset(int y)
	{
		deleteData(y);
		renderable = false;
		updated = false;
	}
	
	void Column::compile(int x, int y, int z)
	{
		/////////////////////////////////////////////////////////////
		// assemble entire column from vbodata section of a column //
		/////////////////////////////////////////////////////////////
		
		int vboCount = 0;
		
		for (int sy = columns.getSizeInSectors(y)-1; sy >= 0; sy--)
		{
			if (vbodata[sy].pcdata != nullptr)
			{
				// COPY the VBO data section
				vboList[vboCount] = vbodata[sy];
				// renderable and consistent, add to queue
				vboCount += 1;
			}
		}
		
		// exit if this column isn't renderable at all
		if (vboCount == 0)
		{
			logger << Log::WARN << "Column::compile(): column was not ready" << Log::ENDL;
			this->updated    = false;
			this->renderable = false;
			return;
		}
		
		///////////////////////////////////////////////////////
		// find offsets for each shader type, and total size //
		///////////////////////////////////////////////////////
		
		int totalverts[RenderConst::MAX_UNIQUE_SHADERS] = {0};
		
		// go through entire column and find entry points and total bytes
		for (int vy = 0; vy < vboCount; vy++)
		{
			// count vertices
			for (int i = 0; i < RenderConst::MAX_UNIQUE_SHADERS; i++)
			{
				// increase by vertices from each path
				totalverts[i] += vboList[vy].vertices[i];
			}
		}
		
		vertex_t* vertoffset[RenderConst::MAX_UNIQUE_SHADERS];
		int vertcount = 0; // dont remove this, needed below
		
		// set the proper offsets into offset(i)
		for (int i = 0; i < RenderConst::MAX_UNIQUE_SHADERS; i++)
		{
			vertoffset[i] = (vertex_t*) column_dump + vertcount;
			
			// set column data:
			this->bufferoffset[i] = vertcount;
			this->vertices[i]     = totalverts[i];
			
			vertcount += totalverts[i];
		}
		
		int totalbytes = vertcount * sizeof(vertex_t);
		
		////////////////////////////////////////////////////////
		// loop through each sector in column and memcpy data //
		////////////////////////////////////////////////////////
		
		for (int vindex = 0; vindex < vboCount; vindex++)
		{
			vbodata_t& v = vboList[vindex];
			
			// loop through all vertices in shader path
			for (int i = 0; i < RenderConst::MAX_UNIQUE_SHADERS; i++)
			{
				// find count for this shader type
				if (v.vertices[i])
				{
					// macro for (vertex*)[source] + number_of_vertices
					#define m_sourcedata ((vertex_t*) v.pcdata) + v.bufferoffset[i]
					
					// to: column_dump + offset(i) from: pcdata + bufferoffset(i) in vertices
					memcpy( vertoffset[i], m_sourcedata, v.vertices[i] * sizeof(vertex_t) );
					
					vertoffset[i] += v.vertices[i];
				}
			} // shaders
			
		} // next vbo
		
		// relieves some RAM usage
		this->deleteData(y);
		
		///////////////////////////////////
		// generate resources for column //
		///////////////////////////////////
		
		bool updateAttribs = false;
		
		if (this->vao == 0)
		{
			// occlusion culling
			glGenQueries(RenderConst::MAX_UNIQUE_SHADERS, this->occlusion);
			// vertex array object
			glGenVertexArrays(1, &this->vao);
			// vertex and index buffer object
			glGenBuffers(1, &this->vbo);
			
			updateAttribs = true;
		}
		
		// bind vao
		glBindVertexArray(this->vao);
		// bind vbo and upload data
		glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
		glBufferData(GL_ARRAY_BUFFER, totalbytes, column_dump, GL_STATIC_DRAW);
		
		if (updateAttribs)
		{
		// attribute pointers
		const vertex_t* vrt = nullptr;
		glVertexAttribPointer(0, 3, GL_SHORT,		  GL_FALSE, sizeof(vertex_t), &vrt->x); // vertex
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_BYTE,		  GL_TRUE,  sizeof(vertex_t), &vrt->nx); // normal
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 4, GL_SHORT,		  GL_FALSE, sizeof(vertex_t), &vrt->u); // texture
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(3, 4, GL_UNSIGNED_BYTE, GL_TRUE,  sizeof(vertex_t), &vrt->biome); // biome color
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(4, 4, GL_UNSIGNED_BYTE, GL_TRUE,  sizeof(vertex_t), &vrt->c);  // shadow and brightness
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(5, 4, GL_UNSIGNED_BYTE, GL_TRUE,  sizeof(vertex_t), (char*) (&vrt->c) + 4); // torchlight color
		glEnableVertexAttribArray(5);
		}
		
		#ifdef DEBUG
		if (OpenGL::checkError())
		{
			throw std::string("Column::compile(): OpenGL error after ending compiler");
		}
		#endif
		
		if (camera.getFrustum().column(x * Sector::BLOCKS_XZ + Sector::BLOCKS_XZ / 2,
								z * Sector::BLOCKS_XZ + Sector::BLOCKS_XZ / 2,
								columns.getSectorLevel(y) * Sector::BLOCKS_Y,
								columns.getSizeInSectors(y) * Sector::BLOCKS_Y, 
								Sector::BLOCKS_XZ / 2))
		{
			// update render list
			camera.needsupd = true;
			
			if (this->renderable == false)
			{
				// make sure it is added to renderq
				camera.recalc = true;
				this->pos.y = -COLUMN_DEPRESSION;
			}
		}
		
		// set as renderable, 
		this->renderable = true;
		// and no longer needing update
		this->updated = false;
		// the vbo has data stored in gpu
		this->hasdata = true;
		
		// reset occluded state
		for (size_t i = 0; i < RenderConst::MAX_UNIQUE_SHADERS; i++)
			this->occluded[i] = 0;
	}
	
}
