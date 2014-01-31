#ifndef NETPLAYER_HPP
#define NETPLAYER_HPP

#include "world.hpp"
#include <string>

namespace cppcraft
{
	
	class NetPlayer
	{
	public:
		typedef unsigned int userid_t;
		
		NetPlayer();
		NetPlayer(userid_t uid, const std::string& name, World::wcoord_t wx, World::wcoord_t wy, World::wcoord_t wz);
		//~NetPlayer();
		
		inline userid_t getUserID() const
		{
			return userID;
		}
		inline const std::string& getName() const
		{
			return name;
		}
		
	private:
		std::string name;
		userid_t userID;
		
	};
	
}

#endif // NETPLAYER_HPP
