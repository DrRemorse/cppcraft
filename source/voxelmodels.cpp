#include "voxelmodels.hpp"

//#include "engine/bitmap.hpp"
#include "library/voxels/voxelizer.hpp"
#include "library/opengl/opengl.hpp"
#include "blocks.hpp"
#include "items.hpp"
#include "tiles.hpp"
#include <string>

using namespace library;

namespace cppcraft
{
	
	void VoxelModels::createItemModels(Bitmap& bmp)
	{
		// roll through all items
		for (int i = 0; i < VOXELITEMS_MAX; i++)
		{
			createModel(voxelItems[i], bmp, items.tileByID(i), items.itemW, items.itemH);
		}
	}
	
	void VoxelModels::createBlockModels(Bitmap& bmp)
	{
		createModel(voxelBlocks[VOXB_LADDER], bmp, Block::cubeFaceById(_LADDER, 0, 0), tiles.txW, tiles.txH);
		
		createModelTall(voxelBlocks[VOXB_WOODDOOR], bmp, Block::cubeFaceById(_WOODDOOR, 0, 1), Block::cubeFaceById(_WOODDOOR, 0, 0), tiles.txW, tiles.txH);
		createModelTall(voxelBlocks[VOXB_STONEDOOR], bmp, Block::cubeFaceById(_STONEDOOR, 0, 1), Block::cubeFaceById(_STONEDOOR, 0, 0), tiles.txW, tiles.txH);
		
		createModel(voxelBlocks[VOXB_TORCH], bmp, Block::cubeFaceById(_TORCH, 0, 0), tiles.txW, tiles.txH);
		
		createModel(voxelBlocks[VOXB_FLOWERRED], bmp, Block::cubeFaceById(_FLOWERRED, 0, 0), tiles.txW, tiles.txH);
		createModel(voxelBlocks[VOXB_FLOWERYELLOW], bmp, Block::cubeFaceById(_FLOWERYELLOW, 0, 0), tiles.txW, tiles.txH);
		createModel(voxelBlocks[VOXB_FLOWERROSE], bmp, Block::cubeFaceById(_FLOWERROSE, 0, 0), tiles.txW, tiles.txH);
		createModel(voxelBlocks[VOXB_GRASS_LONG], bmp, Block::cubeFaceById(_GRASS_LONG, 0, 0), tiles.txW, tiles.txH);
		createModel(voxelBlocks[VOXB_GRASS_SHORT], bmp, Block::cubeFaceById(_GRASS_SHORT, 0, 0), tiles.txW, tiles.txH);
		createModel(voxelBlocks[VOXB_DESERTFLOWER], bmp, Block::cubeFaceById(_DESERTFLOWER, 0, 0), tiles.txW, tiles.txH);
		createModel(voxelBlocks[VOXB_FLOWERREDMAGENTA], bmp, Block::cubeFaceById(_FLOWERREDMAG, 0, 0), tiles.txW, tiles.txH);
		createModel(voxelBlocks[VOXB_PLANT_DRYBROWN], bmp, Block::cubeFaceById(_PLANT_DRYBROWN, 0, 0), tiles.txW, tiles.txH);
		createModel(voxelBlocks[VOXB_FLOWER_SMALL], bmp, Block::cubeFaceById(_FLOWER_SMALL, 0, 0), tiles.txW, tiles.txH);
		createModel(voxelBlocks[VOXB_REEDS], bmp, Block::cubeFaceById(_REEDS, 0, 0), tiles.txW, tiles.txH);
		
		createModel(voxelBlocks[VOXB_BUSH_HOLLY], bmp, Block::cubeFaceById(_BUSH_HOLLY, 0, 0), tiles.txW, tiles.txH);
		createModel(voxelBlocks[VOXB_BUSH2], bmp, Block::cubeFaceById(_BUSH2, 0, 0), tiles.txW, tiles.txH);
		createModel(voxelBlocks[VOXB_BUSH3], bmp, Block::cubeFaceById(_BUSH3, 0, 0), tiles.txW, tiles.txH);
		createModel(voxelBlocks[VOXB_BUSH4], bmp, Block::cubeFaceById(_BUSH4, 0, 0), tiles.txW, tiles.txH);
		createModel(voxelBlocks[VOXB_BUSH5], bmp, Block::cubeFaceById(_BUSH5, 0, 0), tiles.txW, tiles.txH);
		createModel(voxelBlocks[VOXB_BUSH_PINKROSE], bmp, Block::cubeFaceById(_BUSH_PINKROSE, 0, 0), tiles.txW, tiles.txH);
		createModel(voxelBlocks[VOXB_BUSH_MRSPIKY], bmp, Block::cubeFaceById(_BUSH_MRSPIKY, 0, 0), tiles.txW, tiles.txH);
		createModel(voxelBlocks[VOXB_FLOWER_CROCUS], bmp, Block::cubeFaceById(_FLOWER_CROCUS, 0, 0), tiles.txW, tiles.txH);
		
		createModel(voxelBlocks[VOXB_DARKSHROOM], bmp, Block::cubeFaceById(_DARKSHROOM, 0, 0), tiles.txW, tiles.txH);
		createModel(voxelBlocks[VOXB_REDSHROOM], bmp, Block::cubeFaceById(_REDSHROOM, 0, 0), tiles.txW, tiles.txH);
		createModel(voxelBlocks[VOXB_BLUESHROOM], bmp, Block::cubeFaceById(_BLUESHROOM, 0, 0), tiles.txW, tiles.txH);
		createModel(voxelBlocks[VOXB_WATERLILY], bmp, Block::cubeFaceById(_WATERLILY, 0, 0), tiles.txW, tiles.txH);
		
		createModel(voxelBlocks[VOXB_FIRE], bmp, Block::cubeFaceById(_FIRE, 0, 0), tiles.txW, tiles.txH);
		createModel(voxelBlocks[VOXB_HANGINGAPPLE], bmp, Block::cubeFaceById(_HANGINGAPPLE, 0, 0), tiles.txW, tiles.txH);
	}
	
	void VoxelModels::createModel(voxelmodel_t* model, Bitmap& bmp, int tileID, int tw, int th)
	{
		// create temporary bitmap
		Bitmap tmp(tw, th, 32);
		if (tmp.data() == nullptr) throw "VoxelModels::createModel(): bitmap buffer was null";
		
		// copy tile to temporary buffer
		memcpy(tmp.data(), bmp.data() + tileID * tw * th, tw * th * 4);
		
		vec3 offset(0.0, 0.0, 0.0);
		vec3 scale(1.0 / tw, 1.0 / th, VOXELMODELS_ZSCALE);
		XModel xmod;
		
		// create model from bitmap
		xmod.extrude(tmp, offset, scale);
		
		// create vao
		glGenVertexArrays(1, &model->vao);
		glGenBuffers     (1, &model->vbo);
		
		glBindVertexArray(model->vao);
		glBindBuffer(GL_ARRAY_BUFFER_ARB, model->vbo);
		
		glBufferData(GL_ARRAY_BUFFER_ARB, xmod.vertices() * sizeof(xvertex_t), xmod.data(), GL_STATIC_DRAW);
		
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT,         GL_FALSE, sizeof(xvertex_t), (GLvoid*)  0); // position
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_BYTE,          GL_TRUE,  sizeof(xvertex_t), (GLvoid*) 12); // normal
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE,  sizeof(xvertex_t), (GLvoid*) 16); // color
		
		glBindBuffer(GL_ARRAY_BUFFER_ARB, 0);
		glBindVertexArray(0);
		
		// set number of vertices to render
		model->vertices = xmod.vertices();
		
		for (unsigned int i = 0; i < 3; i++)
			glDisableVertexAttribArray(i);
	}
	
	void VoxelModels::createModelTall(voxelmodel_t* model, Bitmap& bmp, int tile_bot, int tile_top, int tw, int th)
	{
		// create temporary bitmap
		Bitmap tmp(tw, th * 2, 4);
		if (tmp.data() == nullptr) return;
		
		int size = tw * th;
		
		// copy tiles to temporary buffer
		memcpy (tmp.data() + 0,    bmp.data() + tile_bot * size, size * 4);
		memcpy (tmp.data() + size, bmp.data() + tile_top * size, size * 4);
		
		vec3 offset(0.0, 0.0, 0.0);
		vec3 scale (1.0 / tw, 1.0 / th, VOXELMODELS_ZSCALE);
		XModel xmod;
		
		// create model from bitmap
		xmod.extrude(tmp, offset, scale);
		
		// create vao
		glGenVertexArrays(1, &model->vao);
		glGenBuffers     (1, &model->vbo);
		
		glBindVertexArray(model->vao);
		
		glBindBuffer(GL_ARRAY_BUFFER_ARB, model->vbo);
		glBufferData(GL_ARRAY_BUFFER_ARB, xmod.vertices() * sizeof(xvertex_t), xmod.data(), GL_STATIC_DRAW);
		
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT,         GL_FALSE, sizeof(xvertex_t), (GLvoid*)  0); // position
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_BYTE,          GL_TRUE,  sizeof(xvertex_t), (GLvoid*) 12); // normal
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE,  sizeof(xvertex_t), (GLvoid*) 16); // color
		
		glBindBuffer(GL_ARRAY_BUFFER_ARB, 0);
		glBindVertexArray(0);
		
		// set number of vertices to render
		model->vertices = xmod.vertices();
		
		for (unsigned int i = 0; i < 3; i++)
			glDisableVertexAttribArray(i);
		
	}
	
	void VoxelModels::renderModel(voxelmodel_t* model)
	{
		if (model->vao == 0) return;
		
		glBindVertexArray(model->vao);
		//glBindBufferARB(GL_ARRAY_BUFFER_ARB, mymodel->vbo)
		
		glDrawArrays(GL_QUADS, 0, model->vertices);
		
		//glBindBufferARB(GL_ARRAY_BUFFER_ARB, null)
		glBindVertexArray(0);
	}
	
	void VoxelModels::renderItem(int modelid)
	{
		renderModel(voxelItems[modelid]);
	}
	
	int VoxelModels::isVoxelBlock(block_t id)
	{
		if (id == _LADDER || isDoor(id)) return true;
		return isCross(id);
	}
	
	int VoxelModels::getVoxelId(block_t id)
	{
		if (id == _LADDER)
		{
			return VOXB_LADDER;
		}
		else if (id == _WOODDOOR)
		{
			return VOXB_WOODDOOR;
		}
		else if (id == _STONEDOOR)
		{
			return VOXB_STONEDOOR;
		}
		else if (id == _TORCH)
		{
			return VOXB_TORCH;
		}
		else if (id >= _FLOWERRED && id <= _REEDS)
		{
			return id - _FLOWERRED + VOXB_FLOWERRED;
		}
		else if (id >= _BUSH_HOLLY && id <= _FLOWER_CROCUS)
		{
			return id - _BUSH_HOLLY + VOXB_BUSH_HOLLY;
		}
		else if (id >= _DARKSHROOM && id <= _BLUESHROOM)
		{
			return id - _DARKSHROOM + VOXB_DARKSHROOM;
		}
		else if (id == _WATERLILY)
		{
			return VOXB_WATERLILY;
		}
		else if (id == _FIRE)
		{
			return VOXB_FIRE;
		}
		else if (id == _HANGINGAPPLE)
		{
			return VOXB_HANGINGAPPLE;
		}
		
		throw "getVoxelId: Invalid voxel id";
		
	} // getVoxelId
	
	void VoxelModels::renderBlock(int modelid)
	{
		modelid = getVoxelId(modelid);
		if (modelid == -1) throw "VoxelModels::renderBlock(): Exigent circumstances";
		
		renderModel(voxelBlocks[modelid]);
	}
	
}