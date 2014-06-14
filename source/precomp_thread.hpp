#ifndef PRECOMP_THREAD_HPP
#define PRECOMP_THREAD_HPP

#define AMBIENT_OCCLUSION_GRADIENTS

#include "blocks.hpp"
#include "vertex_block.hpp"
#include "lighttable.hpp"
#include "renderconst.hpp"

namespace cppcraft
{
	class AmbientOcclusion;
	class Precomp;
	class FlatlandSector;
	class Sector;
	
	class PrecompThreadData
	{
	public:
		~PrecompThreadData();
		
		int pipelineSize[RenderConst::MAX_UNIQUE_SHADERS];
		// working buffers
		vertex_t* databuffer[RenderConst::MAX_UNIQUE_SHADERS];
		int       vertices[RenderConst::MAX_UNIQUE_SHADERS];
		
		Sector* sector;
		visiblefaces_t testdata;
		
		// lightdata scratch table
		LightList ldata;
		
		// blocks counter, for exiting early when all blocks are processed
		int blocks;
		// vertex data pointer
		vertex_t* indic;
		// last block id to be processed
		block_t lastid;
		
		// selected shader type (determined by block id)
		int shaderLine;
		
		// vertex offset positions
		short worldY, worldY_extra;
		
		// special properties of big tiles
		bool repeat_y;
		static const int repeat_factor;
		int bigTextures;
		
		// flatlands biome color index
		int colorIndex;
		
		FlatlandSector* flatl;
		FlatlandSector* flatl_x;
		FlatlandSector* flatl_z;
		FlatlandSector* flatl_xz;
		
		// vertex biome color index
		int lastclid;
		// resulting vertex biome colors
		unsigned int fbiome[4];
		// cheap CRC, to prevent calculating the same color position twice
		int fbicrc;
		
		// processes a Block, outputs a mesh w/lighting
		void process_block(const Block& currentBlock, int bx, int by, int bz);
		
		// vertex emitters for each side of a cube
		int emitCube(const Block& currentBlock, int bx, int by, int bz, int model, block_t facing);
		
		void emitCubeVertexPZ(int model, block_t id, int bx, int by, block_t block_facing);
		void emitCubeVertexNZ(int model, block_t id, int bx, int by, block_t block_facing);
		
		void emitCubeVertexPY(int model, block_t id, int bx, int by, int bz);
		void emitCubeVertexNY(int model, block_t id, int bx, int by, int bz);
		
		void emitCubeVertexPX(int model, block_t id, int by, int bz, block_t block_facing);
		void emitCubeVertexNX(int model, block_t id, int by, int bz, block_t block_facing);
		
		// sloped leafs
		int emitSloped(block_t id, int bx, int by, int bz, block_t model, block_t facing);
		
		// ladders
		int emitLadder(block_t id, int bx, int by, int bz, block_t block_facing);
		// poles
		int emitPole(block_t id, int bx, int by, int bz, block_t model);
		// stairs
		int emitStair(const Block& currentBlock, int bx, int by, int bz, block_t sides);
		// fences
		int emitFence(const Block& currentBlock, int bx, int by, int bz, block_t sides);
		// doors
		int emitDoor(const Block& currentBlock, int bx, int by, int bz, block_t sides);
		// lanterns
		int emitLantern(block_t id, int bx, int by, int bz);
		// crosses
		int emitCross(block_t id, int bx, int by, int bz);
		
		// light raytracer & emission functions for each side of a cube
		
		void applyFaceLighting_PZ(int bx, int by, int bz);
		void applyFaceLighting_NZ(int bx, int by, int bz);
		
		void applyFaceLighting_PY(int bx, int by, int bz);
		void applyFaceLighting_NY(int bx, int by, int bz);
		
		void applyFaceLighting_PX(int bx, int by, int bz);
		void applyFaceLighting_NX(int bx, int by, int bz);
		
		void applyFaceLightingAll(int bx, int by, int bz);
	};
	
	class PrecompThread
	{
	public:
		PrecompThread();
		~PrecompThread();
		
		Precomp* precomp;
		PrecompThreadData pcg;
		AmbientOcclusion* occ;
		
		// stage 1, gathering info
		bool isolator();
		// stage 2, generating mesh
		void precompile();
		// stage 3, computing AO
		void ambientOcclusion();
		
		void killPrecomp();
		void cancelPrecomp();
		
		// ao gradient program, adding corner shadows to a completed mesh
		void ambientOcclusionGradients(AmbientOcclusion* ambocc, vertex_t* datadump, int vertexCount);
		// mesh optimizers
		void optimizeMesh(int shaderline, int txsize);
		void optimizeShadedMesh(int shaderline);
	};
	
}

#endif
