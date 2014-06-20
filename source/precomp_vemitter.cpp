#include "precomp_thread_data.hpp"

#include "blockmodels.hpp"

namespace cppcraft
{
	
	int PrecompThreadData::emitCube(const Block& currentBlock, int bx, int by, int bz, int model, block_t facing)
	{
		///////////////////////////////////////////////
		////  loop through of all sides on a cube  ////
		////   ----   ----   ----   ----   ----    ////
		////  emit vertices & calculate lighting   ////
		///////////////////////////////////////////////
		
		int vertices = 0;
		block_t id = currentBlock.getID();
		block_t face = currentBlock.getFacing();
		
		for (int i = 1; i < 64; i <<= 1)
		{
			// add face only if predetermined visible
			if (facing & i)
			{
				// emit vertex & apply lighting
				switch (i)
				{
				case 1:   // +z
					// emit vertex PZ
					emitCubeVertexPZ(model, id, bx, by, face);
					// calculate lighting
					applyFaceLighting_PZ(bx, by, bz + 1);
					break;
				case 2:   // -z
					// emit vertex NZ
					emitCubeVertexNZ(model, id, bx, by, face);
					// calculate lighting
					applyFaceLighting_NZ(bx, by, bz - 1);
					break;
				case 4:   // +y
					// emit vertex PY
					emitCubeVertexPY(model, id, bx, by, bz);
					// calculate lighting
					applyFaceLighting_PY(bx, by + 1, bz);
					break;
				case 8:   // -y
					// emit vertex_NY
					emitCubeVertexNY(model, id, bx, by, bz);
					// calculate lighting
					applyFaceLighting_NY(bx, by - 1, bz);
					break;
				case 16:  // +x
					// emit vertex PX
					emitCubeVertexPX(model, id, by, bz, face);
					// calculate lighting
					applyFaceLighting_PX(bx + 1, by, bz);
					break;
				case 32:  // -x
					// emit vertex NX
					emitCubeVertexNX(model, id, by, bz, face);
					// calculate lighting
					applyFaceLighting_NX(bx - 1, by, bz);
					break;
				} // switch (i)
				
				indic += 4;
				vertices += 4;
				
			} // if facing & i
			
		} // for each side of cube
		
		// go back to start vertex and return
		indic -= vertices;
		return vertices;
	}
	
	void PrecompThreadData::emitCubeVertexPZ(int model, block_t id, int bx, int by, unsigned short block_facing)
	{
		// copy cube-mesh object 0 (+z side)
		blockmodels.cubes[model].copyTo(0, indic);
		
	#ifdef USE_SLOPING
		// if has bottom-face:
		if slopebot then
			// if has +x face
			if facing and 16 then indic[1].z -= slope_size(4) : indic[1].x -= slope_size(4)
			// if has -x face
			if facing and 32 then indic[0].z -= slope_size(5) : indic[0].x += slope_size(5)
		
		// if has top-face:
		endif
		if slopetop then
			// if has +x face
			if facing and 16 then indic[2].z -= slope_size(0) : indic[2].x -= slope_size(0)
			// if has -x face
			if facing and 32 then indic[3].z -= slope_size(1) : indic[3].x += slope_size(1)
		endif
		
	#endif
		
		indic[0].biome = fbiome[2];
		indic[1].biome = fbiome[3];
		indic[2].biome = fbiome[3];
		indic[3].biome = fbiome[2];
		
		if (shaderLine == RenderConst::TX_REPEAT)
		{
			if (repeat_y)
			{
				indic[0].u = bx * repeat_factor;       // 0, 0
				indic[0].v = by * repeat_factor;
				indic[1].u = (1 + bx) * repeat_factor; // 1, 0
				indic[1].v = by * repeat_factor;
				indic[2].u = (1 + bx) * repeat_factor; // 1, 1
				indic[2].v = (1 + by) * repeat_factor;
				indic[3].u = bx * repeat_factor;       // 0, 1
				indic[3].v = (1 + by) * repeat_factor;
			}
			else
			{
				indic[0].u = bx * repeat_factor;       // 0, 0
				indic[0].v = 2 * repeat_factor;
				indic[1].u = (1 + bx) * repeat_factor; // 1, 0
				indic[1].v = 2 * repeat_factor;
				indic[2].u = (1 + bx) * repeat_factor; // 1, 1
				indic[2].v = 3 * repeat_factor;
				indic[3].u = bx * repeat_factor;       // 0, 1
				indic[3].v = 3 * repeat_factor;
			}
			
			indic->w   = Block::bigFaceById(id, 0, block_facing);
			indic[1].w = indic->w;
			indic[2].w = indic->w;
			indic[3].w = indic->w;
		}
		else
		{
			indic->w   = Block::cubeFaceById(id, 0, block_facing);
			indic[1].w = indic->w;
			indic[2].w = indic->w;
			indic[3].w = indic->w;
		}
		
	} // emitCubeVertexPZ()
	
	void PrecompThreadData::emitCubeVertexNZ(int model, block_t id, int bx, int by, unsigned short block_facing)
	{
		// copy cube-mesh object 1 (-z side)
		blockmodels.cubes[model].copyTo(1, indic);
		
	#ifdef USE_SLOPING
		// if has top-face:
		if slopebot then
			// if has +x face
			if facing and 16 then indic[3].z += slope_size(6) : indic[3].x -= slope_size(6)
			// if has -x face
			if facing and 32 then indic[0].z += slope_size(7) : indic[0].x += slope_size(7)
		
		endif
		if slopetop then
			// if has +x face
			if facing and 16 then indic[2].z += slope_size(2) : indic[2].x -= slope_size(2)
			// if has -x face
			if facing and 32 then indic[1].z += slope_size(3) : indic[1].x += slope_size(3)
		EndIf
	#endif
		
		if (shaderLine == RenderConst::TX_REPEAT)
		{
			if (repeat_y)
			{
				indic[0].u = bx * repeat_factor;       // 0, 0
				indic[0].v = by * repeat_factor;
				indic[1].u = bx * repeat_factor;       // 0, 1
				indic[1].v = (1 + by) * repeat_factor;
				indic[2].u = (1 + bx) * repeat_factor; // 1, 1
				indic[2].v = (1 + by) * repeat_factor;
				indic[3].u = (1 + bx) * repeat_factor; // 1, 0
				indic[3].v = by * repeat_factor;
			}
			else
			{
				indic[0].u = bx * repeat_factor;       // 0, 0
				indic[0].v = 2 * repeat_factor;
				indic[1].u = bx * repeat_factor;       // 0, 1
				indic[1].v = 3 * repeat_factor;
				indic[2].u = (1 + bx) * repeat_factor; // 1, 1
				indic[2].v = 3 * repeat_factor;
				indic[3].u = (1 + bx) * repeat_factor; // 1, 0
				indic[3].v = 2 * repeat_factor;
			}
			
			indic->w   = Block::bigFaceById(id, 1, block_facing);
			indic[1].w = indic->w;
			indic[2].w = indic->w;
			indic[3].w = indic->w;
		}
		else
		{
			indic->w   = Block::cubeFaceById(id, 1, block_facing);
			indic[1].w = indic->w;
			indic[2].w = indic->w;
			indic[3].w = indic->w;
		}
		
		indic[0].biome = fbiome[0];
		indic[1].biome = fbiome[0];
		indic[2].biome = fbiome[1];
		indic[3].biome = fbiome[1];
		
	} // emitCubeVertexNZ()
	
	void PrecompThreadData::emitCubeVertexPY(int model, block_t id, int bx, int by, int bz)
	{
		// 0.0, 0.0,  0.0, 1.0,  1.0, 1.0,  1.0, 0.0
		
		// copy cube-mesh object 2 (+y side)
		blockmodels.cubes[model].copyTo(2, indic);
		
	#ifdef USE_SLOPING
		if slopetop then
			// if has +z face:
			if facing and 1 then
				// if has +x face
				if facing and 16 then indic[2].z -= slope_size(0) : indic[2].x -= slope_size(0)
				// if has -x face
				if facing and 32 then indic[1].z -= slope_size(1) : indic[1].x += slope_size(1)
			EndIf
			// if has -z face
			if facing and 2 then
				// if has +x face
				if facing and 16 then indic[3].z += slope_size(2) : indic[3].x -= slope_size(2)
				// if has -x face
				if facing and 32 then indic[0].z += slope_size(3) : indic[0].x += slope_size(3)
			endif
		endif
	#endif
		
		if (shaderLine == RenderConst::TX_REPEAT)
		{
			indic[0].u = bx * repeat_factor; // 0, 0
			indic[0].v = bz * repeat_factor;
			indic[0].w = Block::bigFaceById(id, 2, 0);
			
			indic[1].u = bx * repeat_factor; // 0, 1
			indic[1].v = (1 + bz) * repeat_factor;
			indic[1].w = indic[0].w;
			
			indic[2].u = (1 + bx) * repeat_factor; // 1, 1
			indic[2].v = (1 + bz) * repeat_factor;
			indic[2].w = indic[0].w;
			
			indic[3].u = (1 + bx) * repeat_factor; // 1, 0
			indic[3].v = bz * repeat_factor;
			indic[3].w = indic[0].w;
		}
		else
		{
			indic[0].w = Block::cubeFaceById(id, 2, 0);
			indic[1].w = indic[0].w;
			indic[2].w = indic[0].w;
			indic[3].w = indic[0].w;
		}
		
		indic[0].biome = fbiome[0];
		indic[1].biome = fbiome[2];
		indic[2].biome = fbiome[3];
		indic[3].biome = fbiome[1];
		
	} // emitBlockvertexPY()
	
	void PrecompThreadData::emitCubeVertexNY(int model, block_t id, int bx, int by, int bz)
	{
		// copy cube-mesh object 3 (-y side)
		blockmodels.cubes[model].copyTo(3, indic);
		
	#ifdef USE_SLOPING
		if slopebot then
			// if has +z face:
			if facing and 1 then
				// if has +x face
				if facing and 16 then indic[2].z -= slope_size(4) : indic[2].x -= slope_size(4)
				// if has -x face
				if facing and 32 then indic[3].z -= slope_size(5) : indic[3].x += slope_size(5)
			EndIf
			// if has -z face
			if facing and 2 then
				// if has +x face
				if facing and 16 then indic[1].z += slope_size(6) : indic[1].x -= slope_size(6)
				// if has -x face
				if facing and 32 then indic[0].z += slope_size(7) : indic[0].x += slope_size(7)
			endif
		endif
	#endif
		
		if (shaderLine == RenderConst::TX_REPEAT)
		{
			// {1,1,  0,1,  0,0,  1,0}
			indic[0].u = bx * repeat_factor; // 0, 0
			indic[0].v = bz * repeat_factor;
			indic[0].w = Block::bigFaceById(id, 3, 0);
			
			indic[1].u = (1 + bx) * repeat_factor;       // 1, 0
			indic[1].v = bz * repeat_factor;
			indic[1].w = indic[0].w;
			
			indic[2].u = (1 + bx) * repeat_factor;       // 1, 1
			indic[2].v = (1 + bz) * repeat_factor;
			indic[2].w = indic[0].w;
			
			indic[3].u = bx * repeat_factor; // 0, 1
			indic[3].v = (1 + bz) * repeat_factor;
			indic[3].w = indic[0].w;
		}
		else
		{
			indic->w   = Block::cubeFaceById(id, 3, 0);
			indic[1].w = indic->w;
			indic[2].w = indic->w;
			indic[3].w = indic->w;
		}
		
		indic[0].biome = fbiome[0];
		indic[1].biome = fbiome[1];
		indic[2].biome = fbiome[3];
		indic[3].biome = fbiome[2];
		
	} // emitCubeVertexPY()
	
	void PrecompThreadData::emitCubeVertexPX(int model, block_t id, int by, int bz, block_t block_facing)
	{
		// copy cube-mesh object 4 (+x side)
		blockmodels.cubes[model].copyTo(4, indic);
		
	#ifdef USE_SLOPING
		// if has -y face:
		if slopebot then
			// if has +z face
			if facing and 1 then indic[3].z -= slope_size(4) : indic[3].x -= slope_size(4)
			// if has -z face
			if facing and 2 then indic[0].z += slope_size(6) : indic[0].x -= slope_size(6)
			
		// if has +y face:
		endif
		if slopetop then
			// if has +z face
			if facing and 1 then indic[2].z -= slope_size(0) : indic[2].x -= slope_size(0)
			// if has -z face
			if facing and 2 then indic[1].z += slope_size(2) : indic[1].x -= slope_size(2)
		endif
	#endif
		
		if (shaderLine == RenderConst::TX_REPEAT)
		{
			if (repeat_y)
			{
				// V: 0.0, 0.0,  1.0, 0.0,  1.0, 1.0,  0.0, 1.0
				// T: 1.0, 0.0,  1.0, 1.0,  0.0, 1.0,  0.0, 0.0
				
				indic[0].u = (bz - 1) * repeat_factor; // 1, 0
				indic[0].v = (by - 1) * repeat_factor;
				indic[1].u = (bz - 1) * repeat_factor; // 1, 1
				indic[1].v =  by      * repeat_factor;
				indic[2].u = (bz + 0) * repeat_factor; // 0, 1
				indic[2].v =  by      * repeat_factor;
				indic[3].u = (bz + 0) * repeat_factor; // 0, 0
				indic[3].v = (by - 1) * repeat_factor;
			}
			else
			{
				indic[0].u = (bz - 1) * repeat_factor; // 1, 0
				indic[0].v = 2  * repeat_factor;
				indic[1].u = (bz - 1) * repeat_factor; // 1, 1
				indic[1].v = 3  * repeat_factor;
				indic[2].u = bz * repeat_factor;       // 0, 1
				indic[2].v = 3  * repeat_factor;
				indic[3].u = bz * repeat_factor;       // 0, 0
				indic[3].v = 2  * repeat_factor;
			}
			
			indic->w   = Block::bigFaceById(id, 4, block_facing);
			indic[1].w = indic->w;
			indic[2].w = indic->w;
			indic[3].w = indic->w;
		}
		else
		{
			indic->w   = Block::cubeFaceById(id, 4, block_facing);
			indic[1].w = indic->w;
			indic[2].w = indic->w;
			indic[3].w = indic->w;
		}
		
		indic[0].biome = fbiome[1];
		indic[1].biome = fbiome[1];
		indic[2].biome = fbiome[3];
		indic[3].biome = fbiome[3];
		
	} // exitBlockVertexPX()

	void PrecompThreadData::emitCubeVertexNX(int model, block_t id, int by, int bz, block_t block_facing)
	{
		// copy cube-mesh object 5 (+x side)
		blockmodels.cubes[model].copyTo(5, indic);
		
	#ifdef USE_SLOPING
		// if has -y face:
		if slopebot then
			// if has +z face
			if facing and 1 then indic[1].z -= slope_size(5) : indic[1].x += slope_size(5)
			// if has -z face
			if facing and 2 then indic[0].z += slope_size(7) : indic[0].x += slope_size(7)
			
		// if has +y face:
		endif
		if slopetop then
			// if has +z face
			if facing and 1 then indic[2].z -= slope_size(1) : indic[2].x += slope_size(1)
			// if has -z face
			if facing and 2 then indic[3].z += slope_size(3) : indic[3].x += slope_size(3)
		EndIf
	#endif
		
		if (shaderLine == RenderConst::TX_REPEAT)
		{
			if (repeat_y)
			{
				indic[0].u = bz * repeat_factor;       // 0, 0
				indic[0].v = by * repeat_factor;
				indic[1].u = (1 + bz) * repeat_factor; // 1, 0
				indic[1].v = by * repeat_factor;
				indic[2].u = (1 + bz) * repeat_factor; // 1, 1
				indic[2].v = (1 + by) * repeat_factor;
				indic[3].u = bz * repeat_factor;       // 0, 1
				indic[3].v = (1 + by) * repeat_factor;
			}
			else
			{
				indic[0].u = bz * repeat_factor;       // 0, 0
				indic[0].v = 2 * repeat_factor;
				indic[1].u = (1 + bz) * repeat_factor; // 1, 0
				indic[1].v = 2 * repeat_factor;
				indic[2].u = (1 + bz) * repeat_factor; // 1, 1
				indic[2].v = 3 * repeat_factor;
				indic[3].u = bz * repeat_factor;       // 0, 1
				indic[3].v = 3 * repeat_factor;
			}
			
			indic->w   = Block::bigFaceById(id, 5, block_facing);
			indic[1].w = indic->w;
			indic[2].w = indic->w;
			indic[3].w = indic->w;
		}
		else
		{
			indic->w   = Block::cubeFaceById(id, 5, block_facing);
			indic[1].w = indic->w;
			indic[2].w = indic->w;
			indic[3].w = indic->w;
		}
		
		indic[0].biome = fbiome[0];
		indic[1].biome = fbiome[2];
		indic[2].biome = fbiome[2];
		indic[3].biome = fbiome[0];
		
	} // emitCubeVertexNX()
	
	////////////////////////////////////////////////////////////
	////    VERTEX LIGHTING RAYTRACER                       ////
	////////////////////////////////////////////////////////////
	//	0: 0,0,0
	//	1: 1,0,0
	//	2: 0,1,0
	//	3: 0,0,1
	//	4: 0,1,1
	//	5: 1,1,0
	//	6: 1,0,1
	//	7: 1,1,1
	
	void PrecompThreadData::applyFaceLighting_PZ(int bx, int by, int bz)
	{
		indic->c   = ldata.tableLight(*sector, bx  , by,   bz, 0); //, 3, 0
		indic[1].c = ldata.tableLight(*sector, bx+1, by,   bz, 0); //, 6, 0
		indic[2].c = ldata.tableLight(*sector, bx+1, by+1, bz, 0); //, 7, 0
		indic[3].c = ldata.tableLight(*sector, bx  , by+1, bz, 0); //, 4, 0
	}
	
	void PrecompThreadData::applyFaceLighting_NZ(int bx, int by, int bz)
	{
		indic->c   = ldata.tableLight(*sector, bx  , by,   bz, 1); //, 0, 1
		indic[1].c = ldata.tableLight(*sector, bx  , by+1, bz, 1); //, 2, 1
		indic[2].c = ldata.tableLight(*sector, bx+1, by+1, bz, 1); //, 5, 1
		indic[3].c = ldata.tableLight(*sector, bx+1, by,   bz, 1); //, 1, 1
	}
	
	void PrecompThreadData::applyFaceLighting_PY(int bx, int by, int bz)
	{
		indic->c   = ldata.tableLight(*sector, bx  , by, bz  , 2); //, 2, 2
		indic[1].c = ldata.tableLight(*sector, bx  , by, bz+1, 2); //, 4, 2
		indic[2].c = ldata.tableLight(*sector, bx+1, by, bz+1, 2); //, 7, 2
		indic[3].c = ldata.tableLight(*sector, bx+1, by, bz  , 2); //, 5, 2
	}
	
	void PrecompThreadData::applyFaceLighting_NY(int bx, int by, int bz)
	{
		indic->c   = ldata.tableLight(*sector, bx  , by, bz  , 3); //, 0, 3
		indic[1].c = ldata.tableLight(*sector, bx+1, by, bz  , 3); //, 1, 3
		indic[2].c = ldata.tableLight(*sector, bx+1, by, bz+1, 3); //, 6, 3
		indic[3].c = ldata.tableLight(*sector, bx  , by, bz+1, 3); //, 3, 3
	}
	
	void PrecompThreadData::applyFaceLighting_PX(int bx, int by, int bz)
	{
		indic->c   = ldata.tableLight(*sector, bx, by,   bz  , 4); //, 1, 4
		indic[1].c = ldata.tableLight(*sector, bx, by+1, bz  , 4); //, 5, 4
		indic[2].c = ldata.tableLight(*sector, bx, by+1, bz+1, 4); //, 7, 4
		indic[3].c = ldata.tableLight(*sector, bx, by,   bz+1, 4); //, 6, 4
	}
	
	void PrecompThreadData::applyFaceLighting_NX(int bx, int by, int bz)
	{
		indic->c   = ldata.tableLight(*sector, bx, by,   bz  , 5); //, 0, 5
		indic[1].c = ldata.tableLight(*sector, bx, by,   bz+1, 5); //, 3, 5
		indic[2].c = ldata.tableLight(*sector, bx, by+1, bz+1, 5); //, 4, 5
		indic[3].c = ldata.tableLight(*sector, bx, by+1, bz  , 5); //, 2, 5
	}
	
	void PrecompThreadData::applyFaceLightingAll(int bx, int by, int bz)
	{
		applyFaceLighting_PZ(bx, by, bz+1);
		indic += 4;
		applyFaceLighting_NZ(bx, by, bz-1);
		indic += 4;
		applyFaceLighting_PY(bx, by+1, bz);
		indic += 4;
		applyFaceLighting_NY(bx, by-1, bz);
		indic += 4;
		applyFaceLighting_PX(bx+1, by, bz);
		indic += 4;
		applyFaceLighting_NX(bx-1, by, bz);
		indic -= 20; // go back
	}
	
}
