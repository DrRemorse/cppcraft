#ifndef VOXELMODELS_HPP
#define VOXELMODELS_HPP

#include <vector>
#include "library/bitmap/bitmap.hpp"

namespace cppcraft
{
#define VOXB_LADDER         0
#define VOXB_WOODDOOR       1
#define VOXB_STONEDOOR      2

#define VOXB_TORCH          5

#define VOXB_FLOWERRED         16
#define VOXB_FLOWERYELLOW      17
#define VOXB_FLOWERROSE        18
#define VOXB_GRASS_LONG        19
#define VOXB_GRASS_SHORT       20
#define VOXB_DESERTFLOWER      21
#define VOXB_FLOWERREDMAGENTA  22
#define VOXB_PLANT_DRYBROWN    23
#define VOXB_FLOWER_SMALL      24
#define VOXB_REEDS             25

#define VOXB_BUSH_HOLLY        30
#define VOXB_BUSH2             31
#define VOXB_BUSH3             32
#define VOXB_BUSH4             33
#define VOXB_BUSH5             34
#define VOXB_BUSH_PINKROSE     35
#define VOXB_BUSH_MRSPIKY      36
#define VOXB_FLOWER_CROCUS     37

#define VOXB_DARKSHROOM        40
#define VOXB_REDSHROOM         41
#define VOXB_BLUESHROOM        42
#define VOXB_WATERLILY         43

#define VOXB_FIRE              50
#define VOXB_HANGINGAPPLE      51

	struct voxelmodel_t;
	typedef unsigned short block_t;
	
	class VoxelModels
	{
	public:
		static const int VOXELITEMS_MAX = 256;
		static const int VOXELBLOCKS_MAX = 64;
		
		// z extrude scale 1:Z
		const float VOXELMODELS_ZSCALE = 0.05;
		
		typedef struct
		{
			unsigned int vao, vbo;
			int vertices;
			
		} voxelmodel_t;
		
		int  isVoxelBlock(block_t id);
		int  getVoxelId  (block_t id);
		
		void createItemModels (library::Bitmap& bmp);
		void createBlockModels(library::Bitmap& bmp);
		
		void renderItem (int modelid);
		void renderBlock(int modelid);
		
	private:
		// voxelized blocks
		voxelmodel_t* voxelBlocks[VOXELBLOCKS_MAX] = {nullptr};
		// voxelized items
		voxelmodel_t* voxelItems[VOXELITEMS_MAX] = {nullptr};
		
		void createModel(voxelmodel_t* model, library::Bitmap& bmp, int tileID, int tw, int th);
		void createModelTall(voxelmodel_t* model, library::Bitmap& bmp, int tileBot, int tileTop, int tw, int th);
		void renderModel(voxelmodel_t* model);
	};
	extern VoxelModels voxels;
}

#endif
