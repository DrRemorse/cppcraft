#include "player.hpp"

#include "library/log.hpp"
#include "library/math/vector.hpp"
#include "library/opengl/input.hpp"
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
		player.pax = player.pay = player.paz = 0.0;
		// rotation
		player.xrotrad = player.yrotrad = 0.0;
		
		player.snapStage = 0;
		player.snapX = player.snapY = player.snapZ = 0;
		
		// initialize keyboard / joystick input
		player.initInputs();
	}
	
	int PlayerClass::getTerrain() const
	{
		return plogic.terrain;
	}
	
	library::vec3 PlayerClass::getLookVector() const
	{
		float dx =  sinf(player.yrotrad) * cosf(player.xrotrad);
		float dy = 		                -sinf(player.xrotrad);
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
		// handle player rotation
		handleRotation();
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
			player.changedPosition = (snapX != X || snapY != Y || snapZ != Z);
			
			if (player.changedPosition)
			{
				camera.recalc = true;
				
				// this needs to be a "better" test, ie. if someone added lights nearby
				if ((int)snapX != (int)X || (int)snapY != (int)Y || (int)snapZ != (int)Z)
				{
					// get player shadows & torchlight
					// a VERY costly operation, needs to be avoided at all costs
					vertex_color_t vcolor = Spiders::getLightNow(X, Y, Z);
					plogic.shadowColor = vcolor & 0xFFFFFFFF;
					plogic.torchColor  = vcolor >> 32;
				}
			}
			
			snapX = X;
			snapY = Y;
			snapZ = Z;
			
			// true if the player is moving (on purpose)
			// used to modulate player camera giving the effect of movement
			JustMoved = plogic.Moved == true && (plogic.freefall == false || plogic.Ladderized);
			
			//logger << "JustMoved: " << JustMoved << Log::ENDL;
			
		}
		mtx.playermove.unlock();
		
		// handle player selection, actions and building
		paction.handle(frametime);
	}
	
	void PlayerClass::handleRotation()
	{
		// measure closeness
		float dx = fabsf(player.xrotrad - input.getRotation().x);
		float dy = fabsf(player.yrotrad - input.getRotation().y);
		// rotate if too far apart
		player.changedRotation = (dx > 0.0001 || dy > 0.0001);
		if (player.changedRotation)
		{
			// get old look vector
			vec3 look1 = lookVector(vec2(player.xrotrad, player.yrotrad));
			// .. and current look vector
			vec3 look2 = lookVector(input.getRotation());
			
			// interpolate
			vec3 newLook = look1.mix(look2, 0.30);
			newLook.normalize();
			
			// back to pitch/yaw radians
			vec2 newRot = newLook.toPitchYaw();
			player.xrotrad = newRot.x; // pitch
			player.yrotrad = newRot.y; // yaw
			
			camera.recalc  = true; // rebuild visibility set
			camera.rotated = true; // resend all rotation matrices
		}
	}
	
}
