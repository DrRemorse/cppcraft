#ifndef NETPLAYER_HPP
#define NETPLAYER_HPP

#include <library/math/vector.hpp>
#include <liblattice/coordinates.h>
#include "world.hpp"
#include <string>

namespace cppcraft
{
	
	class NetPlayer
	{
	public:
		typedef unsigned int userid_t;
		
		NetPlayer();
		NetPlayer(userid_t uid, const std::string& name, const w_coord& w);
		
		inline userid_t getUserID() const
		{
			return userID;
		}
		inline const std::string& getName() const
		{
			return name;
		}
		
		void setPosition(const b_coord& pos);
		
	private:
		std::string name;
		userid_t userID;
		
		// position & orientation
		double pos[3];
		library::vec2 rotation;
		
		// synchronized rendering data
		//vec2 grot;
		library::vec3 gxyz;
	};
	
}

#endif // NETPLAYER_HPP
