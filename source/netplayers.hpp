#ifndef NETPLAYERS_HPP
#define NETPLAYERS_HPP

#include "network.hpp"
#include "netplayer.hpp"
#include <mutex>
#include <vector>

namespace cppcraft
{
	class NetPlayers
	{
	public:
		
		inline void add(NetPlayer& p)
		{
			players.push_back(p);
		}
		void remove(unsigned int uid);
		
		// returns -1 if no matching uid was found
		int indexByUID(unsigned int uid) const;
		// returns null if no matching uid was found
		NetPlayer* playerByUID(unsigned int uid);
		
		// network updates
		void updatePosition(NetPlayer*, UnpackCoordF& position);
		void updateRotation(NetPlayer*, library::vec2& rotation);
		
		// major functions
		void renderPlayers();
		void renderNameTags();
		
	private:
		std::vector<NetPlayer> players;
		std::mutex mtx;
		
	};
	extern NetPlayers netplayers;
}

#endif // NETPLAYERS_HPP
