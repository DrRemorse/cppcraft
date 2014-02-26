#include "netplayer.hpp"

namespace cppcraft
{
	void NetPlayer::setPosition(w_coord& wc, library::vec3& pos)
	{
		this->wc = wc;
		this->pos = pos;
	}
	void NetPlayer::setRotation(library::vec2& rot)
	{
		this->rotation = rot;
	}
	
	dpos_t NetPlayer::getPosition(int wx, int wy, int wz)
	{
		dpos_t position;
		position.x = (this->wc.x - wx) + pos.x;
		position.y = (this->wc.y - wy) + pos.y;
		position.z = (this->wc.z - wz) + pos.z;
		return position;
	}
}

