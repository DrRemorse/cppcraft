#include "precomp_thread.hpp"

#include "library/log.hpp"
#include "biome.hpp"
#include "blockmodels.hpp"
#include "flatlands.hpp"
#include "precompiler.hpp"
#include "renderconst.hpp"
#include "torchlight.hpp"
#include <cstring>

using namespace library;

namespace cppcraft
{
	#define PC_REGULARBLOCK(model)  (model < BlockModels::MI_MODEL_COUNT)
	#define PC_CROSS       10
	#define PC_TRICROSS    11
	#define PC_FLATBOTTOM  12
	
	#define PC_SLOPED      20
	
	#define PC_STAIR       32
	#define PC_DOOR        33
	#define PC_FENCE       34
	#define PC_POLE        35
	#define PC_LADDER      38
	
	#define PC_LANTERN     50
	
	
	
	void PrecompThreadData::process_block(const Block& currentBlock, int bx, int by, int bz)
	{
		block_t id = currentBlock.getID();
		
		// sides of a box primitive to generate mesh for, or alternatively used as just a visibility-test
		unsigned short sides = 1;
		// which model to emit
		int model = 0;
		worldY_extra = 0;
		
		if (id < HALFBLOCK_START)
		{
			// all solid blocks
			sides = currentBlock.visibleFaces(testdata, bx, by, bz);
			model = BlockModels::MI_BLOCK;
		}
		else if (id == _WATER)
		{
			// water
			sides = currentBlock.visibleFaces(testdata, bx, by, bz);
			model = BlockModels::MI_BLOCK;
		}
		else if (isHalfblock(id))
		{
			if (isStair(id))
			{
				// stairs belong in halfblock category
				sides = currentBlock.visibleFaces(testdata, bx, by, bz);
				model = PC_STAIR;
			}
			else
			{
				// halfblocks
				sides = currentBlock.visibleFaces(testdata, bx, by, bz);
				model = BlockModels::MI_HALFBLOCK;
				
				if (currentBlock.getSpecial() == 0)
				{
					// theres a half block of air above this halfblock, so
					sides |= 4; // pop back ze top!
				}
				else
				{
					// theres a half block of air under this halfblock, so
					sides |= 8; // pop back ze bottom!
					// and, lift it up half a block
					worldY_extra = 0.5 * RenderConst::VERTEX_SCALE;
				}
			} // stair or regular halfblocks
		}
		else if (isLowblock(id))
		{
			sides = currentBlock.visibleFaces(testdata, bx, by, bz);
			model  = BlockModels::MI_LOWBLOCK;
			
			worldY_extra = currentBlock.getSpecial() * RenderConst::VERTEX_SCALE * 0.125;
		}
		else if (isCross(id))
		{
			if (id == _WATERLILY)
			{
				model = PC_TRICROSS; // water lily (pad)
			}
			else if (id == _LILYPAD)
			{
				model = PC_FLATBOTTOM; // lilypad model
			}
			else
			{
				model = PC_CROSS; // cross model
			}
		}
		else if (isDoor(id))
		{
			model = PC_DOOR;
		}
		else if (id == _WOODFENCE)
		{
			model = PC_FENCE;
		}
		else if (id == _WOODPOLE)
		{
			// horizontal and vertical, def. vertical
			model = PC_POLE;
		}
		else if (id == _LANTERN || id == _LANTERN_OFF)
		{
			model = PC_LANTERN;
		}
		else if (id == _LADDER || id == _VINES)
		{
			model = PC_LADDER;
		}
		else
		{
			// normal transparent block
			sides = currentBlock.visibleFaces(testdata, bx, by, bz);
			model  = BlockModels::MI_BLOCK;
			
			// check if any are visible
			if (sides)
			{
				// check if leaves of any kind
				if (isLeaf(id))
				{
					if (id == _LEAF_NEEDLE)
					{
						// sloped leafs
						// check that at least one of the four sides (front, back, right, left) aren't visible
						if ((sides & (1 + 2 + 16 + 32)) != (1 + 2 + 16 + 32))
							model = PC_SLOPED;
					}
				}
				else if (id == _CACTUS)
				{
					model = BlockModels::MI_INSET;
				}
				
			}	// facing != 0
			
		} // determine facing
		
		// must have at least one visible face to continue
		if (sides)
		{
			// if last blockid <> current blockid
			if (lastid != id)
			{
				lastid = id;
				
				// determine shader index
				if (isDirt(id))
				{
					shaderLine = RenderConst::TX_REPEAT;
					// clamped Y for all *GRASS_S
					repeat_y = (((id - SOIL_START) % 3) != 1);
				}
				else if (id == _STONE || id == _DESERTSAND || id == _SANDBEACH)
				{
					shaderLine = RenderConst::TX_REPEAT;
					repeat_y = true;
				}
				else if (id == _LAVABLOCK)
				{
					shaderLine = RenderConst::TX_2SIDED;
				}
				else if (id < ALPHA_BARRIER)
				{
					shaderLine = RenderConst::TX_SOLID;
				}
				else if (id == _TORCH || id == _VINES || isLeaf(id))
				{
					shaderLine = RenderConst::TX_2SIDED;
				}
				else if (id < CROSS_START)
				{
					shaderLine = RenderConst::TX_TRANS;
				}
				else if (id < BLEND_BARRIER)
				{
					shaderLine = RenderConst::TX_CROSS;
				}
				else // if (id == _WATER)
				{
					shaderLine = RenderConst::TX_WATER;
				}
				
				// determine pointer position in shader arrays
				// set up lookup and reverse lookup tables
				if (this->vertices[shaderLine])
				{
					// get pointer, increase it by existing vertices
					indic = databuffer[shaderLine] + this->vertices[shaderLine];
				}
				else
				{
					// get new pointer
					indic = databuffer[shaderLine];
					
					// gather lighting ONCE if not yet gathered
					if (ldata.gathered == false)
					{
						// shortcut to light list
						LightList& list = ldata;
						
						// clear light scratch table
						list.table.clear();
						
						// light gathering
						if (sector->hasLight == 1)
						{
							// avoid gathering light data
							list.gathered = true;
							list.lights.clear();
						}
						else
						{
							// gather light data
							torchlight.lightGatherer(*sector, list);
						}
						
					} // initialize light list
					
				} // determine vertice index position
				
				// determine color index
				// no change if the material does not use color indexing
				fclid = Biomes::idToColorIndex(id);
				
				// we need to re-set the biome color
				// using the new fclid because of clid change
				if (lastclid != fclid)
				{
					lastclid = fclid;
					fbicrc = 256;
				}
				
			} // lastid != id
			
			/////////////////////////////////////////////////////////
			////  interpolated biome colors, data from flatland  ////
			/////////////////////////////////////////////////////////
			
			#ifdef USE_BIOMEDATA
				#define BIOME_CRC()  (bx * Sector::BLOCKS_XZ + bz)
				
				if (isColoredBlock(id))
				{
					fbiome[0] = Biomes::getSpecialColorBGRA(currentBlock.getSpecial());
					fbiome[1] = fbiome[0];
					fbiome[2] = fbiome[0];
					fbiome[3] = fbiome[0];
					fbicrc = 256;
				}
				else if (fbicrc != BIOME_CRC())
				{
					// collect { biome_r, biome_g_, biome_b, skylevel } into a vec4:
					fbiome[0] = flatl[0](bx, bz).fcolor[fclid];
					
					if (bx < Sector::BLOCKS_XZ-1)
					{
						fbiome[1] = flatl[0](bx+1, bz).fcolor[fclid];
					}
					else if (flatl_x != nullptr)
					{
						fbiome[1] = flatl_x[0](0, bz).fcolor[fclid];
					}
					if (bz < Sector::BLOCKS_XZ-1)
					{
						fbiome[2] = flatl[0](bx, bz+1).fcolor[fclid];
					}
					else if (flatl_z != nullptr)
					{
						fbiome[2] = flatl_z[0](bx, 0).fcolor[fclid];
					}
					
					if ((bx < Sector::BLOCKS_XZ-1) && (bz < Sector::BLOCKS_XZ-1))
					{
						fbiome[3] = flatl[0](bx+1, bz+1).fcolor[fclid];
					}
					else if (bx < Sector::BLOCKS_XZ-1)
					{
						if (flatl_z != nullptr)
						{
							fbiome[3] = flatl_z[0](bx+1, 0).fcolor[fclid];
						}
					}
					else if (bz < Sector::BLOCKS_XZ-1)
					{
						if (flatl_x != nullptr)
						{
							fbiome[3] = flatl_x[0](0, bz+1).fcolor[fclid];
						}
					}
					else if (flatl_xz != nullptr)
					{
						fbiome[3] = flatl_xz[0](0, 0).fcolor[fclid];
					}
					
					// unique value
					fbicrc = BIOME_CRC();
					
				} // biome colors
				
			#endif
			
			// vertex position in 16bits
			short vx = (bx << RenderConst::VERTEX_SHL);
			short vy = (by << RenderConst::VERTEX_SHL) + worldY + worldY_extra;
			short vz = (bz << RenderConst::VERTEX_SHL);
			
			///////////////////////////////
			//  now, emit some vertices  //
			///////////////////////////////
			int vertices = 0;
			
			if (PC_REGULARBLOCK(model))
			{
				///////////////////////////////////////////////
				////  loop through of all sides on a cube  ////
				////   ----   ----   ----   ----   ----    ////
				////  emit vertices & calculate lighting   ////
				///////////////////////////////////////////////
				
				vertices = emitCube(currentBlock, bx, by, bz, model, sides);
				
			}
			else if (model == PC_SLOPED)
			{
				vertices = emitSloped(id, bx, by, bz, currentBlock.getFacing(), sides);
			}
			else if (model == PC_POLE)
			{
				vertices = emitPole(id, bx, by, bz, currentBlock.getFacing());
			}
			else if (model == PC_STAIR)
			{
				vertices = emitStair(currentBlock, bx, by, bz, sides);
			}
			else if (model == PC_LADDER)
			{
				vertices = emitLadder(id, bx, by, bz, currentBlock.getFacing());
			}
			else if (model == PC_FENCE)
			{
				vertices = emitFence(currentBlock, bx, by, bz, sides);
			}
			else if (model == PC_DOOR)
			{
				vertices = emitDoor(currentBlock, bx, by, bz, sides);
			}
			else if (model == PC_CROSS)
			{
				vertices = emitCross(id, bx, by, bz);
			}
			else if (model == PC_TRICROSS)
			{
				vertices = emitCross(id, bx, by, bz);
				indic += vertices;
				emitCubeVertexPY(BlockModels::MI_BLOCK, id, bx, by-1, bz);
				indic -= vertices;
				vertices += 4;
				// lift ever so slightly upwards, avoiding zfight
				vy++;
			}
			else if (model == PC_FLATBOTTOM)
			{
				emitCubeVertexPY(BlockModels::MI_BLOCK, id, bx, by-1, bz);
				vertices = 4;
				// lift ever so slightly upwards, avoiding zfight
				vy++;
			}
			else if (model == PC_LANTERN)
			{
				vertices = emitLantern(id, bx, by, bz);
			}
			
			// NOTE: we are assuming vertices to be non-zero, since that is ~100% usage case
			// check for overflow
			if (this->vertices[shaderLine] + vertices > RenderConst::MAX_FACES_PER_SECTOR * 4)
			{
				logger << Log::ERR << "Vertices for " << shaderLine << " is too much: " << 
					this->vertices[shaderLine] << " / " << RenderConst::MAX_FACES_PER_SECTOR * 4 << Log::ENDL;
			}
			
			// move mesh object to local grid space
			for (int i = 0; i < vertices; i++)
			{
				indic->x += vx;
				indic->y += vy;
				indic->z += vz;
				indic += 1;
			}
			// increase vertex count for current shaderline
			this->vertices[shaderLine] += vertices;
			
			// resize shaderline if needed (96 vertices treshold)
			// NOTE: if any meshes are larger than 96 vertices there WILL be a crash
			
			if (this->vertices[shaderLine] + 96 >= pipelineSize[shaderLine])
			{
				//logger << shaderLine << ": Increasing size from " << pipelineSize[shaderLine] << " to " << pipelineSize[shaderLine] + 250 << Log::ENDL;
				// resize operation
				vertex_t* old = this->databuffer[shaderLine];
				int oldsize = pipelineSize[shaderLine];
				pipelineSize[shaderLine] += 250;
				this->databuffer[shaderLine] = new vertex_t[pipelineSize[shaderLine]];
				memcpy (this->databuffer[shaderLine], old, oldsize * sizeof(vertex_t));
				delete old;
				// set new vertex position
				indic = this->databuffer[shaderLine] + this->vertices[shaderLine];
			}
			
			
		} // if (facing)
		
	} // precompile_block()
	
}
