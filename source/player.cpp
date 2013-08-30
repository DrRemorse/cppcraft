#include "player.hpp"

#include "library/log.hpp"
#include "library/math/vector.hpp"
#include "camera.hpp"
#include "player_actions.hpp"
#include "player_logic.hpp"
#include "player_physics.hpp"
#include "renderconst.hpp"
#include "sectors.hpp"
#include "spiders.hpp"
#include "threading.hpp"
#include <cmath>

using namespace library;

namespace cppcraft
{
	// player object
	PlayerClass player;
	
	void PlayerClass::initPlayer()
	{
		plogic.movestate = PMS_Normal;
		plogic.sector = nullptr;
		plogic.block = &air_block;
		
		plogic.Falling = false;
		plogic.FullySubmerged = plogic.PS_None;
		plogic.freefall = false;
		plogic.EscapeAttempt = false;
		plogic.Slowfall = false;
		plogic.Ladderized = false;
		//plogic.jetpacking = false;
		player.Flying = true;
		
		// initialize block selection system
		plogic.selection = playerselect_t();
		
		// initialize action/interaction system
		paction.init();
		
		// misc
		plogic.terrain = 0;
		plogic.shadowColor = 255 + (255 << 24);
		
		// center grid, center sector, center block
		player.X = ((float)Sectors.getXZ() / 2.0 - 0.5) * Sector::BLOCKS_XZ + 0.5;
		player.Y = Sector::BLOCKS_Y * 22; //RenderConst::WATER_LEVEL + Sector::BLOCKS_Y * 12; //2;
		player.Z = player.X;
		// acceleration
		player.pax = 0.0;
		player.paz = 0.0;
		// rotation
		player.xrotrad = 0.0;
		player.yrotrad = 0.0;
		
		player.snapStage = 0;
		
		// initialize keyboard / joystick input
		player.initInputs();
	}
	
	void PlayerClass::rotated()
	{
		plogic.Rotate = true;
	}
	
	int PlayerClass::getTerrain() const
	{
		return plogic.terrain;
	}
	
	library::vec3 PlayerClass::getLookVector() const
	{
		float dx =  sinf(player.yrotrad) * cosf(player.xrotrad);
		float dy = 		                  -sinf(player.xrotrad);
		float dz = -cosf(player.yrotrad) * cosf(player.xrotrad);
		
		return vec3(dx, dy, dz);
	}
	
	block_t PlayerClass::getBlockFacing() const
	{
		vec2 sightXZ( sinf(player.yrotrad), -cosf(player.yrotrad) );
		
		block_t facing = 0;
		
		if (sightXZ.x < -0.707) // left
			facing = 2;
		else if (sightXZ.x > 0.707) // right
			facing = 3;
		
		if (sightXZ.y < -0.707) // front -> back
			facing = 0;
		else if (sightXZ.y > 0.707) // back -> front
			facing = 1;
		
		return facing;
	}
	
	void PlayerClass::handlePlayerTicks()
	{
		// acceleration & movement inputs
		playerPhysics.handleMomentum();
		// jumping, big enough to get its own module
		plogic.handlePlayerJumping();
		// translate player (gravity & acceleration)
		plogic.translatePlayer();
		// play some sounds
		plogic.playerSounds();
	}
	
	void PlayerClass::handleActions(double frametime)
	{
		// if the player has moved from his snapshot, we need to synch with renderer
		mtx.playermove.lock();
		{
			// player could have been moved twice, the second time causing him to lose the moved flag
			// so we need to explicitly test all scalars
			if (snapX != X || snapY != Y || snapZ != Z)
			{
				camera.recalc = true;
				// get player shadows & torchlight
				vertex_color_t vcolor = Spiders::getLightNow(X, Y, Z);
				plogic.shadowColor = vcolor & 0xFFFFFFFF;
				plogic.torchColor  = vcolor >> 32;
			}
			
			snapX = X;
			snapY = Y;
			snapZ = Z;
			// true if the player is moving (on purpose)
			// used to modulate player camera
			JustMoved = plogic.Moved == true && (plogic.freefall == false || plogic.Ladderized);
			
			//logger << "JustMoved: " << JustMoved << Log::ENDL;
			
		}
		mtx.playermove.unlock();
		
		// handle player selection, actions and building
		paction.handle(frametime);
	}
	
}
