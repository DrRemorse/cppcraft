/**
 * Recursive grid, sparse oct-tree
 * 
**/

#ifndef RENDERGRID_HPP
#define RENDERGRID_HPP

namespace cppcraft
{
	class RenderGrid
	{
	public:
		static const int visibility_border = 2;
		
		struct rendergrid_t
		{
			int xstp, ystp, zstp;
			int majority;
			int minGrid, maxGrid;
			float playerY;
		};
		
		static void uniformGrid(rendergrid_t& rg, int x0, int x1, int z0, int z1, int quant);
	};
	
}

#endif
