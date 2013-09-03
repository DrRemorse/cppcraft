#include "rendergrid.hpp"

#include "library/log.hpp"
#include "library/opengl/opengl.hpp"
#include "columns.hpp"
#include "drawq.hpp"
#include "camera.hpp"
#include "sectors.hpp"

using namespace library;

namespace cppcraft
{
	static const float gs_half = Sector::BLOCKS_XZ * 0.5f;
	static const int   visibility_border = RenderGrid::visibility_border;
	
	// forward
	void gridTesting(RenderGrid::rendergrid_t& rg, int x, int z, int axis);
	
	void RenderGrid::uniformGrid(RenderGrid::rendergrid_t& rg, int x0, int x1, int z0, int z1, int quant)
	{
		const int  stepsize = camera.getGridsize() >> quant;
		const bool gridtest = (stepsize <= 2);
		
		const int bigstpx = rg.xstp * stepsize;
		const int bigstpz = rg.zstp * stepsize;
		
		// signed step
		const int sizex = bigstpx << 3;
		const int sizez = bigstpz << 3;
		// half size of this chunk (FIXME)
		const float size = (stepsize+1) << 3;
		
		static const int MAX_Y = Sectors.SECTORS_Y * Sector::BLOCKS_Y;
		
		int countX = (x1 - x0) / bigstpx + 1;
		int countZ = (z1 - z0) / bigstpz + 1;
		
		if (rg.majority)
		{
			for (int x = x0; countX-- != 0; x += bigstpx)
			{
				int x2 = x * Sector::BLOCKS_XZ + sizex;
				int cz = countZ;
				
				for (int z = z0; cz-- != 0; z += bigstpz)
				{
					if (camera.getFrustum().column(x2, z * Sector::BLOCKS_XZ + sizez, 0, MAX_Y, size))
					{
						if (gridtest)
							gridTesting(rg, x, z, stepsize);
						else
							uniformGrid(rg, x, x + bigstpx - rg.xstp, z, z + bigstpz - rg.zstp, quant + 1);
					}
				}
			}
		}
		else
		{
			for (int z = z0; countZ-- != 0; z += bigstpz)
			{
				int z2 = z * Sector::BLOCKS_XZ + sizez;
				int cx = countX;
				
				for (int x = x0; cx-- != 0; x += bigstpx)
				{
					if (camera.getFrustum().column(x * Sector::BLOCKS_XZ + sizex, z2, 0, MAX_Y, size))
					{
						if (gridtest)
							gridTesting(rg, x, z, stepsize);
						else
							uniformGrid(rg, x, x + bigstpx - rg.xstp, z, z + bigstpz - rg.zstp, quant + 1);
					}
				}
			}
		}
		
	}
	
	void gridTesting(RenderGrid::rendergrid_t& rg, int x, int z, int axis)
	{
		int x0 = x, x1;
		int z0 = z, z1;
		int y0, y1;
		
		if (rg.xstp == 1)
		{
			// x0 == x
			if (x0 < visibility_border) x0 = visibility_border;
			
			x1 = x + (axis - 1);
			if (x1 >= Sectors.getXZ() - visibility_border)
				x1 = Sectors.getXZ()-1 - visibility_border;
			
			if (x0 >= Sectors.getXZ() - visibility_border || x0 > x1) return;
		}
		else
		{
			// x0 == x
			if (x0 >= Sectors.getXZ() - visibility_border)
				x0 = Sectors.getXZ()-1 - visibility_border;
			
			x1 = x - (axis - 1);
			if (x1 < visibility_border) x1 = visibility_border;
			
			if (x1 >= Sectors.getXZ() - visibility_border || x1 > x0) return;
		}
		
		if (rg.ystp == 1)
		{
			y0 = 0;
			y1 = Columns.COLUMNS_Y - 1;
		}
		else
		{
			y0 = Columns.COLUMNS_Y - 1;
			y1 = 0;
		}
		
		if (rg.zstp == 1)
		{
			// z0 = z
			if (z0 < visibility_border) z0 = visibility_border;
			
			z1 = z + (axis - 1);
			if (z1 >= Sectors.getXZ() - visibility_border)
				z1 = Sectors.getXZ()-1 - visibility_border;
			
			if (z0 >= Sectors.getXZ() - visibility_border || z0 > z1) return;
		}
		else
		{
			// z0 = z
			if (z0 >= Sectors.getXZ() - visibility_border)
				z0 = Sectors.getXZ()-1 - visibility_border;
			
			z1 = z - (axis - 1);
			if (z1 < visibility_border) z1 = visibility_border;
			
			if (z1 >= Sectors.getXZ() - visibility_border || z1 > z0) return;
		}
		
		int y = y0;
		x = x0; z = z0;
		int center_grid = (Sectors.getXZ() / 2) + 0.5;
		int max_gridrad = (center_grid-1);
		max_gridrad *= max_gridrad;
		
		float fx = (x + 0.5) - center_grid;
		float fz = (z + 0.5) - center_grid;
		
		while (true)
		{	
			Column& cv = Columns(x, y, z);
			
			if (cv.renderable)
			{
				if (fx*fx + fz*fz < max_gridrad)
				{
					if (camera.getFrustum().column(
							x * Sector::BLOCKS_XZ + gs_half,
							z * Sector::BLOCKS_XZ + gs_half,
							y * Columns.COLUMNS_SIZE * Sector::BLOCKS_Y,
							Columns.COLUMNS_SIZE * Sector::BLOCKS_Y, 
							gs_half // 12.65
						))
					{
						cv.pos.x = x * Sector::BLOCKS_XZ;
						cv.pos.z = z * Sector::BLOCKS_XZ;
						
						for (int i = 0; i < RenderConst::MAX_UNIQUE_SHADERS; i++)
						{
							if (cv.vertices[i])
							{
								// as long as not currently testing:
								if (cv.occluded[i] != 1)
								{
									// as long as not contested or was not rendered last frame
									//cv.occluded[i] = 3;
								}
								
								// add to draw queue
								drawq[i].add(&cv);
							}	
							
						} // while i < shaders
						
					} // frustum test
					
				} // radial test
			}
			else if (cv.vao) // not renderable, but has VAO
			{
				// DONT DISABLE THIS, GPU WILL RUN OUT OF MEMORY IN A HEARTBEAT!!!!!!!!
				glDeleteVertexArrays(1, &cv.vao);
				glDeleteBuffers(1, &cv.vbo);
				glDeleteQueries(RenderConst::MAX_UNIQUE_SHADERS, cv.occlusion);
				cv.vao = 0;
				
			} // render test
			
			if (rg.majority)
			{
				// Y -> Z -> X
				if (y == y1)
				{
					if (z == z1)
					{
						if (x == x1) return;
						
						x += rg.xstp;
						z = z0;
					}
					else z += rg.zstp;
					y = y0;
					// set new (fx, fy) centroidal position
					fx = (x + 0.5) - center_grid;
					fz = (z + 0.5) - center_grid;
				}
				else y += rg.ystp;
			}
			else
			{
				// Y -> X -> Z
				if (y == y1)
				{
					if (x == x1)
					{
						if (z == z1) return;
						
						z += rg.zstp;
						x = x0;
					}
					else x += rg.xstp;
					y = y0;
				}
				else y += rg.ystp;
				
			} // majority
			
		} // do loop
		
	} // gridTesting()
	
}
