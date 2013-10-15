#include "network.hpp"

#include "library/log.hpp"
#include "player.hpp"
#include "player_logic.hpp"
#include "world.hpp"

using namespace library;

extern "C"
{
	#include <liblattice/lattice_config.h>
	#include <arpa/inet.h>
	#include <liblattice/struct.h>
	#include <liblattice/globals.h>
	#include <liblattice/liblattice.h>
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
		this->running = true;
		this->networkThread = std::thread(&Network::mainLoop, this);
	}
	
	void Network::stop()
	{
		if (this->running)
		{
			this->running = false;
			this->networkThread.join();
		}
	}
	
	void Network::mainLoop()
	{
		logger << Log::INFO << "Connecting to lattice" << Log::ENDL;
		lattice_init(-1, dummy);
		
		if (connect() == false)
		{
			logger << Log::ERR << "Network::mainLoop(): Connection failed" << Log::ENDL;
			return;
		}
		logger << Log::INFO << "Connected to network" << Log::ENDL;
		
		lattice_flush();
		
		timeval waitTime;
		waitTime.tv_sec  = 0;
		waitTime.tv_usec = 3000;
		
		while (this->running)
		{
			lattice_select(&waitTime);
			
			lattice_process();
			
			lattice_flush();
		}
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
	
	bool Network::connect()
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
		
		if (lattice_connect("98.243.47.31", 8805) < 0) return false;
		return true;
	}
	
}
