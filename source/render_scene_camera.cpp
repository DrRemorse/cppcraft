#include "render_scene.hpp"

#include "library/log.hpp"
#include "blocks.hpp"
#include "player.hpp"
#include "player_logic.hpp"
#include <cmath>

using namespace library;

namespace cppcraft
{
	static const double PI2 = 4 * atan(1) * 2;
	
	bool SceneRenderer::cameraDeviation(double frameCounter, double dtime)
	{
		if (playerMoved)
		{
			switch (plogic.movestate)
			{
				case PMS_Normal:
					motionTimed += 0.15 * dtime;
					break;
				case PMS_Crouch:
					motionTimed += 0.10 * dtime;
					break;
				case PMS_Sprint:
					motionTimed += 0.225 * dtime;
					break;
			}
		}
		else motionTimed = 0.0;
		
		#define NORMAL_CAMERA_DEV()  deviation = sin(motionTimed) * 0.05;
		#define LADDER_CAMERA_DEV()  deviation = (motionTimed != 0) ? sin(motionTimed * 1.5) * 0.07 : sin(frameCounter / 20) * 0.025;
		
		double deviation;
		bool deviating = false;
		
		if (player.Flying)
		{
			// do nothing
		}
		else if (plogic.FullySubmerged)
		{
			// always if underwater
			deviation = sin(frameCounter / 40) * 0.05;
			deviating = true;
		}
		else if (plogic.Submerged)
		{
			// only if NOT standing on ground (needs mutex)
			if (plogic.Falling)
			{
				deviation = sin(frameCounter / 20) * 0.1;
				deviating = true;
			}
		}
		else if (plogic.Ladderized)
		{
			if (plogic.block)
			{
				// determine what block we are standing on
				if (plogic.block->getID() == _LADDER || plogic.block->getID() == _AIR)
				{
					// ladder, air --> ladder
					LADDER_CAMERA_DEV();
					deviating = true;
				}
				else
				{
					// something else --> normal
					NORMAL_CAMERA_DEV();
					deviating = true;
				}
			}
		}
		else if (motionTimed != 0)
		{
			NORMAL_CAMERA_DEV();
			deviating = true;
		}
		
		const double lerp = 0.6;
		
		// interpolate deviation with old
		if (deviating)
			deviation = deviation * lerp + lastCameraDeviation * (1.0 - lerp);
		else
		{
			deviation = lastCameraDeviation * 0.7;
		}
		
		// modulate playerY when delta is high enough
		if (fabs(deviation) > 0.001)
		{
			playerY += deviation;
			deviating = true;
		}
		else deviating = false;
		
		// remember new deviation
		lastCameraDeviation = deviation;
		
		// when crouching we need to lower camera quite a bit
		if (plogic.movestate == PMS_Crouch)
		{
			playerY -= 0.25;
			return true;
		}
		return deviating;
	}
}
