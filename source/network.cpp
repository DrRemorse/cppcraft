#include "network.hpp"

#include "library/log.hpp"
#include "player.hpp"
#include "player_logic.hpp"
#include "world.hpp"

using namespace library;

extern "C"
{
	#include <arpa/inet.h>
	#include "network/lattice_config.h"
	#include "network/struct.h"
	#include "network/globals.h"
	#include "network/liblattice.h"
}

namespace cppcraft
{
	Network network;
	
	void dummy(lattice_message* mp)
	{
		logger << "got type " << mp->type << Log::ENDL;
		return;
	}
	
	void Network::init()
	{
		lattice_init(-1, (void(*)(void*)) dummy);
	}
	
	w_coord getPlayerWC()
	{
		return (w_coord)
		{
			world.getWX(),
			(int)player.Y >> 3,
			world.getWZ()
		};
		
	}
	
	void Network::connect()
	{
		lattice_player.userid = 12345;
		lattice_player.model = 5;
		lattice_player.color = plogic.shadowColor;
		lattice_player.nickname = "gonzo";
		lattice_player.burstdist = 16;
		lattice_player.wpos.x = world.getWX();
		lattice_player.wpos.y = 20;
		lattice_player.wpos.z = world.getWZ();
		lattice_player.bpos.x = 584;
		lattice_player.bpos.y = 434;
		lattice_player.bpos.z = 1180;
		lattice_player.hrot.xrot = 17;
		lattice_player.hrot.yrot = 86;
		lattice_player.hhold.item_id = 0;
		lattice_player.hhold.item_type = 0;
		lattice_player.mining = 0;
		lattice_player.usercolor = 15;
		
		lattice_player.centeredon.x = 524288;
		lattice_player.centeredon.y = 0;
		lattice_player.centeredon.z = 524288;
		
		struct in_addr server_ip;
		server_ip.s_addr = inet_addr("68.61.97.144");
		
		if (lattice_connect(server_ip, 8805) < 0) return;
		lattice_flush();
		
		while (true)
		{
			lattice_select(nullptr);
			
			lattice_process();
			
			lattice_flush();
		}
		
	}
	
}
