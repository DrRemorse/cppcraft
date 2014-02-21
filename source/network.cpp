#include "network.hpp"

#include <library/log.hpp>
#include <library/config.hpp>
#include "player.hpp"
#include "player_logic.hpp"
#include "sector.hpp"
#include "spiders.hpp"
#include "world.hpp"
#include <random>

using namespace library;

extern "C"
{
	#include <liblattice/lattice_config.h>
	#ifdef _WIN32
		#include <windows.h>
	#else
		#include <arpa/inet.h>
	#endif
	#include <liblattice/serversocket.h>
	#include <liblattice/struct.h>
	#include <liblattice/globals.h>
	#include <liblattice/liblattice.h>
}

namespace cppcraft
{
	Network network;
	void message(lattice_message* mp);
	
	SpiderCoord::SpiderCoord(double x, double y, double z)
	{
		// get integral values
		int bx = x, by = y, bz = z;
		// get sector, and truncate integrals into sector-local coordinates
		Sector* s = Spiders::spiderwrap(bx, by, bz);
		// set final world coordinates
		wcoord[0] = world.getWX() + s->x;
		wcoord[1] = world.getWY() + s->y;
		wcoord[2] = world.getWZ() + s->z;
		
		// set final sector coordinates (with fractions)
		scoord = vec3(bx, by, bz) + vec3(x, y, z).frac();
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
		lattice_init(-1, message);
		
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
			
			// transfer stuff from client to network
			handleTransfer();
		}
	}
	
	void message(lattice_message* mp)
	{
		/*
		#define T_P       1
		#define T_QUIT    2
		#define T_PC      3
		#define T_PR      4
		#define T_PH      5
		#define T_CHAT    6
		#define T_ACTION  7
		#define T_S       8
		#define T_SC      9
		#define T_BO      10
		#define T_MO      11
		#define T_BADD    12
		#define T_BSET    13
		#define T_BREM    14
		#define T_PMINE   15
		#define T_SCHAT   16
		#define T_LUSERS  17
		#define T_LOG     18
		#define T_SATSTEP 19
		#define T_SAT     20
		#define T_FADE    21
		#define T_USER    22
		#define T_SERVER  23
		*/
		switch (mp->type)
		{
		case T_CONNECTED:
			logger << Log::INFO << "Connected to server" << Log::ENDL;
			break;
		case T_DISCONNECTED:
			logger << Log::WARN << "Disconnected from server" << Log::ENDL;
			break;
			
		case T_SAT:
			logger << Log::INFO << "SAT angle: " << ((double*) mp->args)[0] << Log::ENDL;
			break;
			
		case T_SATSTEP:
			logger << Log::INFO << "SAT step: " << ((int*) mp->args)[0] << Log::ENDL;
			break;
			
		case T_P:
			logger << Log::INFO << "Player moves: " << ((int*) mp->args)[0] << Log::ENDL;
			break;
			
		case T_LOG:
			logger << Log::INFO << "SERVER  " << ((char*) mp->args) << Log::ENDL;
			break;
			
		default:
			logger << Log::INFO << "got type " << mp->type << Log::ENDL;
		}
		
		return;
	}
	
	w_coord getPlayerWC()
	{
		w_coord wc;
		wc.x = world.getWX();
		wc.y = (int)player.Y >> 3;
		wc.z = world.getWZ();
		return wc;
	}
	
	bool Network::connect()
	{
		std::string  hostn = config.get("net.host", "127.0.0.1");
		unsigned int port  = config.get("net.port", 8805);
		
		std::string uname  = config.get("net.user", "guest");
		std::string upass  = config.get("net.pass", "guest");
		
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<unsigned int> dis;
		
		lattice_player.userid = dis(gen);
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
	
	
	/// from main (worldman) thread ///
	void Network::handleNetworking()
	{
		if (player.changedPosition || player.changedRotation)
		{
			mtx.lock();
			{
				ntt.pmoved   = player.changedPosition;
				ntt.protated = player.changedRotation;
				
				if (ntt.pmoved)   ntt.pcoord = SpiderCoord(player.X, player.Y, player.Z);
				if (ntt.protated) ntt.prot   = vec2(player.xrotrad, player.yrotrad);
			}
			mtx.unlock();
		}
	}
	
	/// from networking scheduler ///
	void Network::handleTransfer()
	{
		mtx.lock();
		{
			if (ntt.pmoved)
			{
				lattice_p lp;
				// world coordinates
				lp.wcoord.x = ntt.pcoord.wcoord[0];
				lp.wcoord.y = ntt.pcoord.wcoord[1];
				lp.wcoord.z = ntt.pcoord.wcoord[2];
				// block coordinates
				lp.bcoord.x = ntt.pcoord.scoord.x * 256;
				lp.bcoord.y = ntt.pcoord.scoord.y * 256;
				lp.bcoord.z = ntt.pcoord.scoord.z * 256;
				
				lattice_message lm;
				lm.type = T_P;
				lm.args = &lp;
				
				lattice_send(&lm);
			}
			if (ntt.protated)
			{
				lattice_pr pr;
				pr.rot.xrot = ntt.prot.x;
				pr.rot.yrot = ntt.prot.y;
				
				lattice_message lm;
				lm.type = T_PR;
				lm.args = &pr;
				
				lattice_send(&lm);
			}
		}
		mtx.unlock();
	}
}
