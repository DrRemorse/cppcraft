#include <library/log.hpp>
#include "blockmodels.hpp"
#include "gameconf.hpp"
#include "lighting.hpp"
#include "precomp_thread.hpp"
#include "precompiler.hpp"
#include "renderconst.hpp"
#include "sector.hpp"
#include "spiders.hpp"
#include "tiles.hpp"

using namespace library;

namespace cppcraft
{
	void PrecompThread::ambientOcclusion(Precomp& precomp)
	{
		// recount total vertices
		int cnt = precomp.vertices[0];
		for (int i = 1; i < RenderConst::MAX_UNIQUE_SHADERS; i++)
		{
			cnt += precomp.vertices[i];
		}
		
		if (precomp.datadump == nullptr)
		{
			logger << Log::ERR << "PrecompThread::ambientOcclusion(): datadump was null" << Log::ENDL;
			precomp.result = Precomp::STATUS_FAILED;
			return;
		}
		
		// ambient occlusion processing stage
		#ifdef AMBIENT_OCCLUSION_GRADIENTS
			if (gameconf.ssao == false)
			{
				ambientOcclusionGradients(*precomp.sector, precomp.datadump, cnt);
			}
		#endif
		
		// optimize repeating textures mesh
		optimizeMesh(precomp, RenderConst::TX_REPEAT, RenderConst::VERTEX_SCALE / tiles.tilesPerBigtile);
		// optimize normal solids
		optimizeMesh(precomp, RenderConst::TX_SOLID, RenderConst::VERTEX_SCALE);
		// optimize transparent textures
		optimizeMesh(precomp, RenderConst::TX_TRANS, RenderConst::VERTEX_SCALE);
		// optimize water & lava meshes
		optimizeShadedMesh(precomp, RenderConst::TX_WATER);
		optimizeShadedMesh(precomp, RenderConst::TX_LAVA);
		
		// set result
		precomp.result = Precomp::STATUS_DONE;
	}
	
	inline int cubeAO(char side1, char side2, char corner)
	{
		if (side1 & side2) return 3;
		return side1 + side2 + corner;
	}
	inline unsigned char blockIsOccluder(int x, int y, int z)
	{
		block_t id = Spiders::getBlock(x, y, z).getID();
		return (id > AIR_END && id < CROSS_START && id != _LANTERN && id != _VINES) & 1;
	}
	
	void PrecompThread::ambientOcclusionGradients(Sector& sector, vertex_t* datadump, int vertexCount)
	{
		static unsigned char shadowRamp[] = 
			{ 255 - 0, 255 - 120, 255 - 128, 255 - 140 };
		
		// world height in block units
		int sbx = sector.getX() << Sector::BLOCKS_XZ_SH;
		int sbz = sector.getZ() << Sector::BLOCKS_XZ_SH;
		
		// calculate face counts for each integral vertex position
		vertex_t* vt = datadump; // first vertex
		vertex_t* vt_max = vt + vertexCount;
		
		unsigned char vside1;
		unsigned char vside2;
		unsigned char vcorner;
		
		for (; vt < vt_max; vt++)
		{
			if (vt->face) // only supported faces!
			{
				int corner = (vt->face >> 3) & 7;
				// 0 if left,  1 if right
				// 0 if lower, 2 if upper
				// 0 if back,  4 if front
				
				int face = vt->face & 7;
				// 1 = +z front, 2 = -z back
				// 3 = +y top,   4 = -y bottom
				// 5 = +x right, 6 = -x left
				
				int x = (vt->x >> RenderConst::VERTEX_SHL) + sbx;
				int y = (vt->y >> RenderConst::VERTEX_SHL);
				int z = (vt->z >> RenderConst::VERTEX_SHL) + sbz;
				// move points back to where they should be
				x += (corner & 1) ? -1 : 0;
				y += (corner & 2) ? -1 : 0;
				z += (corner & 4) ? -1 : 0;
				
				int vdirX = (corner & 1) ? 1 : -1; // 2 * (corner & 1) - 1;
				int vdirY = (corner & 2) ? 1 : -1; // (corner & 2) - 1;
				int vdirZ = (corner & 4) ? 1 : -1; // (corner & 4) / 2 - 1;
				
				if (face < 3) // +/-z
				{
					vside1  = blockIsOccluder(x+vdirX, y,       z+vdirZ);
					vside2  = blockIsOccluder(x,       y+vdirY, z+vdirZ);
					vcorner = blockIsOccluder(x+vdirX, y+vdirY, z+vdirZ);
				}
				else if (face < 5) // +/-y
				{
					vside1  = blockIsOccluder(x+vdirX, y+vdirY, z);
					vside2  = blockIsOccluder(x,       y+vdirY, z+vdirZ);
					vcorner = blockIsOccluder(x+vdirX, y+vdirY, z+vdirZ);
				}
				else // +/-x
				{
					vside1  = blockIsOccluder(x+vdirX, y+vdirY, z);
					vside2  = blockIsOccluder(x+vdirX, y,       z+vdirZ);
					vcorner = blockIsOccluder(x+vdirX, y+vdirY, z+vdirZ);
				}
				
				unsigned char v = cubeAO(vside1, vside2, vcorner);
				((unsigned char*)&vt->c)[2] = shadowRamp[v];
			}
		} // vertices
		
	} // ambientOcclusionGradients()
	
}
