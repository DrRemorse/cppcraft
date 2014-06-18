#include "worldmanager.hpp"

#include "chunks.hpp"
#include "columns.hpp"
#include "flatlands.hpp"
#include "player.hpp"
#include "player_inputs.hpp"
#include "precompq.hpp"
#include "sectors.hpp"
#include "threading.hpp"
#include "worldbuilder.hpp"
#include <GL/glfw3.h>

using namespace library;

namespace cppcraft
{
	void WorldManager::teleportHandler()
	{
		if (input.getKey(GLFW_KEY_P) == Input::KEY_PRESSED)
		{
			input.hold(GLFW_KEY_P);
			// flush chunk queue
			chunks.flushChunks();
			// finish running jobs
			precompq.finish();
			
			mtx.sectorseam.lock();
			{
				// transition to new location
				world.transitionTo(World::WORLD_STARTING_X+192, World::WORLD_STARTING_Z-232);
				
				// invalidate ALL sectors
				Sectors.regenerateAll();
				
				// center grid, center sector, center block
				player.X = ((float)Sectors.getXZ() / 2.0 - 0.5) * Sector::BLOCKS_XZ + 0.5;
				player.Z = player.X;
				player.Y = flatlands.getSkyLevel(player.X, player.Z) + 2.5;
				
				mtx.compiler.lock();
				{
					for (int x = 0; x < Sectors.getXZ(); x++)
					for (int z = 0; z < Sectors.getXZ(); z++)
					for (int y = 0; y < columns.getColumnsY(); y++)
					{
						columns(x, y, z).reset(y);
					}
				}
				mtx.compiler.unlock();
			}
			mtx.sectorseam.unlock();
			// reset world builder
			worldbuilder.reset();
			
		}
	}
	
}