#include <library/log.hpp>
#include "gameconf.hpp"
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
	
	inline unsigned char cubeAO(unsigned char side1, unsigned char side2, unsigned char corner)
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
			{ 255, 255 - 76, 255 - 100, 255 - 140 };
		
		// world height in block units
		int sbx = sector.getX() << Sector::BLOCKS_XZ_SH;
		int sbz = sector.getZ() << Sector::BLOCKS_XZ_SH;
		
		// calculate face counts for each integral vertex position
		vertex_t* vt; // first vertex
		vertex_t* vt_max = datadump + vertexCount;
		// the channel used for AO in a vertex
		#define vertexAO(vt)  ((unsigned char*)&((vt)->c))[2]
		
		unsigned char vside1;
		unsigned char vside2;
		unsigned char vcorner;
		
		for (vt = datadump; vt < vt_max; vt++)
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
				
				int x = ((vt->x + 4) >> RenderConst::VERTEX_SHL) + sbx;
				int y = ((vt->y + 4) >> RenderConst::VERTEX_SHL);
				int z = ((vt->z + 4) >> RenderConst::VERTEX_SHL) + sbz;
				// move points back to where they should be
				x -= (corner & 1);
				y -= ((corner & 2) >> 1) - (vt->face >> 6);
				z -= (corner & 4) >> 2;
				// the direction we should move to catch neighbors
				int vdirX = ((corner & 1) * 2) - 1;
				int vdirY =  (corner & 2) - 1;
				int vdirZ = ((corner & 4) - 2) / 2;
				
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
				vertexAO(vt) = shadowRamp[v];
			}
		} // vertices
		
		// flip quads that need it to avoid anisotropy
		for (vt = datadump; vt < vt_max; vt += 4)
		{
			if (vt->face) // only supported faces!
			{
				if (vertexAO(vt+0) + vertexAO(vt+2) < vertexAO(vt+1) + vertexAO(vt+3))
				{
					vertex_t vt0 = vt[0];
					// flip quad
					vt[0] = vt[1];
					vt[1] = vt[2];
					vt[2] = vt[3];
					vt[3] = vt0;
				}
			}
		}
		
	} // ambientOcclusionGradients()
	
}
