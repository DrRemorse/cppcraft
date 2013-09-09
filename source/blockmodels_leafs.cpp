#include "blockmodels.hpp"

#include "renderconst.hpp"

namespace cppcraft
{
	void BlockModels::initSlopedLeafs()
	{
		float cube_vertices[6][12] =
		{
			{0.0, 0.0, 1.0,  1.0, 0.0, 1.0,  1.0, 1.0, 1.0,  0.0, 1.0, 1.0},  // +z front 
			{0.0, 0.0, 0.0,  0.0, 1.0, 0.0,  1.0, 1.0, 0.0,  1.0, 0.0, 0.0},  // -z back
			{0.0, 1.0, 0.0,  0.0, 1.0, 1.0,  1.0, 1.0, 1.0,  1.0, 1.0, 0.0},  // +y top
			{0.0, 0.0, 0.0,  1.0, 0.0, 0.0,  1.0, 0.0, 1.0,  0.0, 0.0, 1.0},  // -y bottom
			{1.0, 0.0, 0.0,  1.0, 1.0, 0.0,  1.0, 1.0, 1.0,  1.0, 0.0, 1.0},  // +x right
			{0.0, 0.0, 0.0,  0.0, 0.0, 1.0,  0.0, 1.0, 1.0,  0.0, 1.0, 0.0}   // -x left
		};
		
		float cube_texcoords[6][8] =
		{
			{0.0, 0.0,  1.0, 0.0,  1.0, 1.0,  0.0, 1.0},
			{1.0, 0.0,  1.0, 1.0,  0.0, 1.0,  0.0, 0.0},
			{0.0, 1.0,  0.0, 0.0,  1.0, 0.0,  1.0, 1.0},
			{1.0, 1.0,  0.0, 1.0,  0.0, 0.0,  1.0, 0.0},
			{1.0, 0.0,  1.0, 1.0,  0.0, 1.0,  0.0, 0.0},
			{0.0, 0.0,  1.0, 0.0,  1.0, 1.0,  0.0, 1.0}
		};
		
		signed char cube_normals[6][3] =
		{
			{0, 0, 127}, { 0,  0, -128}, // front back
			{0, 127, 0}, { 0, -128,  0}, // top   bottom
			{127, 0, 0}, {-128,  0,  0}  // right left
		};
		
		const short VERTEX_SCALE = RenderConst::VERTEX_SCALE;
		
		for (int model = 0; model < 4; model++)
		{
			// create mesh object with 5 * 4 vertices
			BlockMesh bm(5 * 4);
			int index = 0;
			
			for (int face = 0; face < 5; face++)
			{
				int side = face;
				
				if (model < 2)
				{
					if (face >= model) side = face + 1;
				}
				else
				{
					if (face >= model + 2) side = face + 1;
				}
				
				// iterate 4 vertices on quad
				for (int vert = 0; vert < 4; vert++)
				{
					float x = cube_vertices[side][vert * 3 + 0];
					float y = cube_vertices[side][vert * 3 + 1];
					float z = cube_vertices[side][vert * 3 + 2];
					
					switch (model)
					{
					case 0: // -z
						if (z == 0 && y == 1) y = 0.125;
						break;
					case 1: // +z
						if (z == 1 && y == 1) y = 0.125;
						break;
					case 2: // -x
						if (x == 0 && y == 1) y = 0.125;
						break;
					case 3: // +x
						if (x == 1 && y == 1) y = 0.125;
						break;
					}
					
					bm[index].x = x * VERTEX_SCALE; // x
					bm[index].y = y * VERTEX_SCALE; // y
					bm[index].z = z * VERTEX_SCALE; // z
					bm[index].face = 0;
					
					bm[index].u = cube_texcoords[side][vert * 2 + 0] * VERTEX_SCALE;
					bm[index].v = cube_texcoords[side][vert * 2 + 1] * VERTEX_SCALE;
					bm[index].w = 0;
					
					bm[index].nx = cube_normals[side][0]; // nx
					bm[index].ny = cube_normals[side][1]; // ny
					bm[index].nz = cube_normals[side][2]; // nz
					
					index++;
				}
			}
			// add mesh object to sloped leaf model list
			slopedLeafs.add(bm);
		}
		
	} // initCubes()
}
