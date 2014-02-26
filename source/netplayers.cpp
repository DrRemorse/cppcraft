#include "netplayers.hpp"

namespace cppcraft
{
	NetPlayers netplayers;
	
	void NetPlayers::remove(unsigned int uid)
	{
		int index = indexByUID(uid);
		if (index != -1)
		{
			std::swap(players[index], players.back());
			players.pop_back();
		}
	}
	
	int NetPlayers::indexByUID(unsigned int uid) const
	{
		for (size_t i = 0; i < players.size(); i++)
		{
			if (players[i].getUserID() == uid) return i;
		}
		return -1;
	}
	NetPlayer* NetPlayers::playerByUID(unsigned int uid)
	{
		for (NetPlayer& p : players)
		{
			if (p.getUserID() == uid) return &p;
		}
		return nullptr;
	}
	
	void NetPlayers::updatePosition(NetPlayer* p, UnpackCoordF& position)
	{
		p->setPosition(position.wc, position.bc);
	}
	
	void NetPlayers::updateRotation(NetPlayer* p, library::vec2& rotation)
	{
		p->setRotation(rotation);
	}
}
