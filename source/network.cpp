#include "network.hpp"

#include "library/log.hpp"
#include "library/config.hpp"
#include "player.hpp"
#include "player_logic.hpp"
#include "world.hpp"

using namespace library;

extern "C"
{
	#include <liblattice/lattice_config.h>
	#ifdef _WIN32
		#include <windows.h>
	#else
		#include <arpa/inet.h>
	#endif
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
		lattice_init(-1, dummy);
		
		if (connect() == false)
		{
			logger << Log::ERR << "Network::mainLoop(): Connection failed" << Log::ENDL;
			return;
		}
		logger << Log::INFO << "Connected to network" << Log::ENDL;
		
		lattice_flush();
		
		timeval waitTime;
		
		while (this->running)
		{
			waitTime.tv_sec  = 0;
			waitTime.tv_usec = 5000;
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
		std::string  hostn = config.get("net.host", "127.0.0.1");
		unsigned int port  = config.get("net.port", 8805);
		
		std::string uname  = config.get("net.user", "guest");
		std::string upass  = config.get("net.pass", "guest");
		
		lattice_player.userid = 12345;
		lattice_player.nickname = (char*) uname.c_str();
		lattice_player.model = 5;
		
		lattice_player.color = plogic.shadowColor;
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
		
		logger << Log::INFO << "Connecting to " << hostn << ":" << port << Log::ENDL;
		
		if (lattice_connect((char*) hostn.c_str(), port) < 0) return false;
		return true;
	}
	
}
