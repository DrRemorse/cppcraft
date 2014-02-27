#ifndef NETPLAYERS_HPP
#define NETPLAYERS_HPP

#include <library/opengl/vao.hpp>
#include "netplayer.hpp"
#include <mutex>
#include <vector>

namespace cppcraft
{
	class UnpackCoordF;
	
	class NetPlayers
	{
	public:
		typedef unsigned int userid_t;
		
		inline void add(NetPlayer& p)
		{
			players.push_back(p);
		}
		void remove(userid_t uid);
		
		// returns -1 if no matching uid was found
		int indexByUID(userid_t uid) const;
		// returns null if no matching uid was found
		NetPlayer* playerByUID(userid_t uid);
		
		// network updates
		void updatePosition(NetPlayer*, UnpackCoordF& position);
		void updateRotation(NetPlayer*, library::vec2& rotation);
		
		// convert positions from relative (w, b) to snapshot/renderable (g)
		void positionSnapshots(int wx, int wz);
		
		// major functions
		void renderPlayers();
		void renderNameTags();
		
	private:
		std::vector<NetPlayer> players;
		std::mutex mtx;
		library::VAO vao;
	};
	extern NetPlayers netplayers;
}

#endif // NETPLAYERS_HPP
