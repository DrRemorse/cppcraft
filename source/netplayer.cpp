#include "netplayer.hpp"

#include <library/log.hpp>
#include "sectors.hpp"
#include "world.hpp"
#include <cmath>

using namespace library;

namespace cppcraft
{
	NetPlayer::NetPlayer(NetPlayer::userid_t uid, const std::string& name, w_coord& wc, const vec3& pos)
	{
		this->userID = uid;
		this->name = name;
		this->render = false;
		this->wc_from = this->wc_to = wc;
		this->bc_from = this->bc_to = pos;
	}
	
	void NetPlayer::movementUpdate(double dtime)
	{
		/// check if player is in local grid ///
		if (wc_to.x < world.getWX() || wc_to.z < world.getWZ() ||
			wc_to.x >= world.getWX() + Sectors.getXZ() || wc_to.z >= world.getWZ() + Sectors.getXZ())
		{
			// since the player is obviously outside render distance, get out
			logger << Log::INFO << this->name << " was outside rendering distance" << Log::ENDL;
			wc_from = wc_to;
			bc_from = bc_to;
			render = false;
			return;
		}
		
		/// check if player FROM is too far from TO ///
		static const int S_MAX_DIST = 2;
		
		// make sure to-position isn't too far from from-position, if it is we need to just set the position and get out
		if (abs(wc_from.x-wc_to.x) > S_MAX_DIST || abs(wc_from.z-wc_to.z) > S_MAX_DIST)
		{
			// the position was too far to interpolate on, just set new from position
			//logger << Log::INFO << this->name << " was outside interpolation distance" << Log::ENDL;
			wc_from = wc_to;
			bc_from = bc_to;
			render = true;
			return;
		}
		
		//logger << Log::INFO << this->name << ": interpolating movement" << Log::ENDL;
		
		// since the player is inside render distance, and the to and from distances are close enough, we will just interpolate
		int wfx = (wc_from.x - world.getWX()) << Sector::BLOCKS_XZ_SH;
		int wfy = (wc_from.y - world.getWY()) << Sector::BLOCKS_Y_SH;
		int wfz = (wc_from.z - world.getWZ()) << Sector::BLOCKS_XZ_SH;
		
		int wtx = (wc_to.x - world.getWX()) << Sector::BLOCKS_XZ_SH;
		int wty = (wc_to.y - world.getWY()) << Sector::BLOCKS_Y_SH;
		int wtz = (wc_to.z - world.getWZ()) << Sector::BLOCKS_XZ_SH;
		
		// calculate local positions
		vec3 wfvec = vec3(wfx, wfy, wfz) + bc_from;
		vec3 wtvec = vec3(wtx, wty, wtz) + bc_to;
		
		// interpolate based on dtime
		const float interpolator = 0.5 * dtime;
		vec3 newpos = wfvec * (1.0 - interpolator) + wtvec * interpolator;
		
		/// recalculate new from position ///
		// split into fractional and integrals
		vec3 fracs = newpos.frac();
		int bx = newpos.x;
		int by = newpos.y;
		int bz = newpos.z;
		// reconstruct world position
		wc_from.x = (bx >> Sector::BLOCKS_XZ_SH) + world.getWX();
		wc_from.y = (by >> Sector::BLOCKS_Y_SH)  + world.getWY();
		wc_from.z = (bz >> Sector::BLOCKS_XZ_SH) + world.getWZ();
		// reconstruct sector position
		bc_from.x = (bx & (Sector::BLOCKS_XZ-1)) + fracs.x;
		bc_from.y = (by & (Sector::BLOCKS_Y-1))  + fracs.y;
		bc_from.z = (bz & (Sector::BLOCKS_XZ-1)) + fracs.z;
		
		render = true;
	}
	
	void NetPlayer::moveTo(w_coord& wc, const vec3& pos)
	{
		this->wc_to = wc;
		this->bc_to = pos;
		moving = true;
	}
	void NetPlayer::stopMoving()
	{
		moving = false;
	}
	void NetPlayer::setRotation(const vec2& rot)
	{
		this->rotation = rot;
	}
	
	vec3 NetPlayer::getPosition(int wx, int wy, int wz)
	{
		vec3 position;
		position.x = (this->wc_from.x - wx) * Sector::BLOCKS_XZ + bc_from.x;
		position.y = (this->wc_from.y - wy) * Sector::BLOCKS_Y  + bc_from.y;
		position.z = (this->wc_from.z - wz) * Sector::BLOCKS_XZ + bc_from.z;
		return position;
	}
}

