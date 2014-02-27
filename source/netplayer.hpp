#ifndef NETPLAYER_HPP
#define NETPLAYER_HPP

#include <library/math/vector.hpp>
#include <liblattice/coordinates.h>
//#include <network.hpp>
#include "world.hpp"
#include <string>

namespace cppcraft
{
	struct dpos_t
	{
		double x, y, z;
	};
	
	class NetPlayer
	{
	public:
		typedef unsigned int userid_t;
		
		NetPlayer();
		NetPlayer(userid_t uid, const std::string& name);
		
		inline userid_t getUserID() const
		{
			return userID;
		}
		inline const std::string& getName() const
		{
			return name;
		}
		
		dpos_t getPosition(int wx, int wy, int wz);
		const library::vec2& getRotation() const
		{
			return rotation;
		}
		
		void setPosition(w_coord& wc, library::vec3& pos);
		void setRotation(library::vec2& rot);
		
	private:
		std::string name;
		userid_t userID;
		
		// position & orientation
		w_coord wc;
		library::vec3 pos;
		library::vec2 rotation;
		
		// synchronized rendering data
		//vec2 grot;
		library::vec3 gxyz;
		
		friend class NetPlayers;
	};
	
}

#endif // NETPLAYER_HPP
