#include "columns.hpp"

#include "library/log.hpp"
#include "library/opengl/opengl.hpp"
#include "camera.hpp"
#include "renderconst.hpp"
#include "vertex_block.hpp"

using namespace library;

namespace cppcraft
{
	const float Column::COLUMN_DEPRESSION = 8.0f;
	
	// all the columns you'll ever need
	ColumnsContainer Columns;
	
	// column compiler accumulation buffer
	vertex_t* column_dump;
	
	void ColumnsContainer::init()
	{
		//////////////////////
		// allocate columns //
		//////////////////////
		logger << Log::INFO << "* Initializing columns" << Log::ENDL;
		
		this->columns = new Column*[Sectors.getXZ() * Sectors.getXZ() * COLUMNS_Y];
		
		int i = 0;
		
		// allocate just as many actual columns
		for (int x = 0; x < Sectors.getXZ(); x++)
		for (int z = 0; z < Sectors.getXZ(); z++)
		for (int y = 0; y < COLUMNS_Y; y++)
		{
			this->columns[i++] = new Column(x, z);
		}
		
		////////////////////////////////////////////////////////
		// allocate temporary datadumps for compiling columns //
		////////////////////////////////////////////////////////
		
		// colv_size is number of sectors in a column
		column_dump = (vertex_t*) calloc(Columns.COLUMNS_SIZE * RenderConst::MAX_FACES_PER_SECTOR, 4 * sizeof(vertex_t));
		if (column_dump == nullptr)
		{
			throw std::string("ColumnQueue::initCompilers(): ERROR: failed to allocate dump");
		}
		
	}
	
	#define column_memory_layout  (x * Sectors.getXZ() * this->COLUMNS_Y + z * this->COLUMNS_Y + y)
	
	Column::Column(int x, int z)
	{
		this->wx = x;
		this->wz = z;
		// initialize VAO to 0, signifying a column without valid GL resources
		this->vao = 0;
		// set initial flags
		this->updated    = false;
		this->renderable = false;
	}
	
	Column* & ColumnsContainer::manipulate(int x, int y, int z)
	{
		return *(this->columns + column_memory_layout);
	}
	
	Column& ColumnsContainer::operator() (int x, int y, int z)
	{
		return *manipulate(x, y, z);
	}
	
	void Column::compile(int x, int y, int z)
	{
		/////////////////////////////////////////////////////////////////////
		// assemble entire column from vbodata section of a column sectors //
		/////////////////////////////////////////////////////////////////////
		
		// first sector in column
		int start_y = y * Columns.COLUMNS_SIZE;
		int end_y = start_y + Columns.COLUMNS_SIZE;
		
		int vboCount = 0;
		bool ready = true;
		vbodata_t* vboList[Columns.COLUMNS_SIZE] = {nullptr};
		
		for (int sy = end_y - 1; sy >= start_y; sy--)
		{
			// first check if compiled!!
			// to avoid compiling a column that is to be updated AGAIN soon!
			// nullsectors and culled sectors are considered COMPILED
			
			if (Sectors(x, sy, z).progress != Sector::PROG_COMPILED)
			{
				ready = false;
			}
			if (Sectors(x, sy, z).render)
			{
				// get VBO data section
				vboList[vboCount] = Sectors(x, sy, z).vbodata;
				
				// a renderable without VBO data, is not a renderable!
				if (vboList[vboCount] == nullptr)
				{
					Sectors(x, sy, z).progress = Sector::PROG_NEEDRECOMP;
					ready = false;
				}
				// renderable and consistent, add to queue
				vboCount += 1;
			}
			
		}
		if (ready == false) return;
		
		// exit if this column isn't renderable
		if (vboCount == 0)
		{
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
			vbodata_t* v = vboList[vy];
			
			// count vertices
			for (int i = 0; i < RenderConst::MAX_UNIQUE_SHADERS; i++)
			{
				// increase by vertices from each path
				totalverts[i] += v->vertices[i];
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
			vbodata_t* v = vboList[vindex];
			
			// loop through all vertices in shader path
			for (int i = 0; i < RenderConst::MAX_UNIQUE_SHADERS; i++)
			{
				// find count for this shader type
				if (v->vertices[i])
				{
					// macro for (vertex*)[source] + number_of_vertices
					#define m_sourcedata ((vertex_t*) v->pcdata) + v->bufferoffset[i]
					
					// to: column_dump + offset(i) from: pcdata + bufferoffset(i) in vertices
					memcpy( vertoffset[i], m_sourcedata, v->vertices[i] * sizeof(vertex_t) );
					
					vertoffset[i] += v->vertices[i];
				}
			} // shaders
			
		} // next vbo
		
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
		
		// bind vbo and set to total bytes
		glBindBuffer(GL_ARRAY_BUFFER_ARB, this->vbo);
		// upload data
		glBufferData(GL_ARRAY_BUFFER_ARB, totalbytes, column_dump, GL_STATIC_DRAW_ARB);
		
		if (updateAttribs)
		{
		// attribute pointers
		glVertexAttribPointer(0, 3, GL_SHORT,			GL_FALSE, sizeof(vertex_t), (void*) offsetof(vertex_t, x )); // vertex
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_BYTE,			GL_TRUE,  sizeof(vertex_t), (void*) offsetof(vertex_t, nx)); // normal
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 3, GL_BYTE,			GL_TRUE,  sizeof(vertex_t), (void*) offsetof(vertex_t, tx)); // tangent
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(3, 3, GL_SHORT,			GL_FALSE, sizeof(vertex_t), (void*) offsetof(vertex_t, u )); // texture
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(4, 4, GL_UNSIGNED_BYTE,	GL_TRUE,  sizeof(vertex_t), (void*) offsetof(vertex_t, c));  // shadow and brightness
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(5, 4, GL_UNSIGNED_BYTE,	GL_TRUE,  sizeof(vertex_t), (void*) (offsetof(vertex_t, c) + 4)); // torchlight color
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(6, 4, GL_UNSIGNED_BYTE,	GL_TRUE,  sizeof(vertex_t), (void*) offsetof(vertex_t, biome)); // biome color
		glEnableVertexAttribArray(6);
		}
		
		// disable vao & vbo
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER_ARB, 0);
		
		// disable vertex attributes
		if (updateAttribs)
		{
			for (int i = 0; i < 7; i++)
				glDisableVertexAttribArray(i);
		}
		
		if (ogl.checkError())
		{
			throw std::string("Column::compile(): OpenGL error after ending compiler");
		}
		
		if (camera.getFrustum().column(x * Sector::BLOCKS_XZ + Sector::BLOCKS_XZ / 2,
								z * Sector::BLOCKS_XZ + Sector::BLOCKS_XZ / 2,
								y * Columns.COLUMNS_SIZE * Sector::BLOCKS_Y,
								Columns.COLUMNS_SIZE * Sector::BLOCKS_Y, 
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
		this->updated    = false;
		
		// remove extraneous data from sectors
		for (int sy = start_y + Columns.COLUMNS_SIZE - 1; sy >= start_y; sy--)
		{
			if (Sectors(x, sy, z).vbodata)
			{
				vbodata_t* v = Sectors(x, sy, z).vbodata;
				
				// remove vertice data
				if (v->pcdata)
				{
					free(v->pcdata);
					v->pcdata = nullptr;
				}
				// remove container
				delete v;
				// null deleted vbodata pointer
				Sectors(x, sy, z).vbodata = nullptr;
			}	
		}
		
		// reset occluded state
		for (size_t i = 0; i < RenderConst::MAX_UNIQUE_SHADERS; i++)
			this->occluded[i] = 0;
		
	}
	
}
