#include "network.hpp"

#include <library/log.hpp>
#include <library/config.hpp>
#include "player.hpp"
#include "player_logic.hpp"
#include "sectors.hpp"
#include "spiders.hpp"
#include "world.hpp"
#include <cstring>
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
	
	PackCoord::PackCoord(double x, double y, double z)
	{
		// get integral values
		int bx = x, by = y, bz = z;
		// get sector, and truncate integrals into sector-local coordinates
		Sector* s = Spiders::spiderwrap(bx, by, bz);
		if (s == nullptr) { valid = false; return; }
		
		// set world coordinates
		wc.x = world.getWX() + s->x;
		wc.y = world.getWY() + s->y;
		wc.z = world.getWZ() + s->z;
		
		// set block coordinates (with 8-bit fractions)
		vec3 f = vec3(x, y, z).frac();
		bc.x = (bx << 8) + (f.x * 255);
		bc.y = (by << 8) + (f.y * 255);
		bc.z = (bz << 8) + (f.z * 255);
		valid = true;
	}
	PackCoord::PackCoord(int bx, int by, int bz)
	{
		// get sector, and truncate integrals into sector-local coordinates
		Sector* s = Spiders::spiderwrap(bx, by, bz);
		if (s == nullptr) { valid = false; return; }
		
		// set world coordinates
		wc.x = world.getWX() + s->x;
		wc.y = world.getWY() + s->y;
		wc.z = world.getWZ() + s->z;
		
		// set block coordinates
		bc.x = bx;
		bc.y = by;
		bc.z = bz;
		valid = true;
	}
	UnpackCoord::UnpackCoord(w_coord& w, b_coord& b)
	{
		// set final world coordinates
		int sx = w.x - world.getWX();
		int sy = w.y - world.getWY();
		int sz = w.z - world.getWZ();
		// validate position is inside our grid
		if (sx < 0 || sx >= Sectors.getXZ() ||
			sy < 0 || sy >= Sectors.getY() ||
			sz < 0 || sz >= Sectors.getXZ())
		{
			valid = false; return;
		}
		// set values
		this->bx = (sx << Sector::BLOCKS_XZ_SH) + b.x;
		this->by = (sy << Sector::BLOCKS_Y_SH)  + b.y;
		this->bz = (sz << Sector::BLOCKS_XZ_SH) + b.z;
		valid = true;
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
	
	/// incoming blocks ///
	void blockAdded(lattice_badd* badd)
	{
		NetworkBlock block;
		block.type = NetworkBlock::BADD;
		block.wc = badd->wcoord;
		block.bc = badd->bcoord;
		block.block = Block(badd->block.id, badd->block.bf);
		
		network.addBlock(Network::INCOMING, block);
	}
	void blockAdded(lattice_bset* bset)
	{
		NetworkBlock block;
		block.type = NetworkBlock::BSET;
		block.wc = bset->wcoord;
		block.bc = bset->bcoord;
		block.block = Block(bset->block.id, bset->block.bf);
		
		network.addBlock(Network::INCOMING, block);
	}
	void blockAdded(lattice_brem* brem)
	{
		NetworkBlock block;
		block.type = NetworkBlock::BREM;
		block.wc = brem->wcoord;
		block.bc = brem->bcoord;
		
		network.addBlock(Network::INCOMING, block);
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
		case T_BUMP:
			logger << Log::INFO << "I'm supposedly outside of all servers?" << Log::ENDL;
			break;
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
			
		case T_BADD:
			blockAdded((lattice_badd*) mp->args);
			break;
		case T_BSET:
			blockAdded((lattice_bset*) mp->args);
			break;
		case T_BREM:
			blockAdded((lattice_brem*) mp->args);
			break;
			
		case T_LOG:
			//logger << Log::INFO << "SERVER  " << ((char*) mp->args) << Log::ENDL;
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
		lattice_player.nickname = strdup((char*) uname.c_str());
		
		PackCoord playerCoords(player.X, player.Y, player.Z);
		
		lattice_player.burstdist = 16;
		
		// network location
		lattice_player.centeredon.x = (playerCoords.wc.x >> 8);
		lattice_player.centeredon.y = (playerCoords.wc.y >> 8);
		lattice_player.centeredon.z = (playerCoords.wc.z >> 8);
		// world coordinates
		lattice_player.wpos = playerCoords.wc;
		// block coordinates
		lattice_player.bpos = playerCoords.bc;
		
		lattice_player.hrot.xrot = player.xrotrad;
		lattice_player.hrot.yrot = player.yrotrad;
		
		lattice_player.model = 5;
		lattice_player.usercolor = 15;
		lattice_player.color = plogic.shadowColor;
		
		lattice_player.hhold.item_id = 0;
		lattice_player.hhold.item_type = 0;
		lattice_player.mining = 0;
		
		logger << Log::INFO << "Connecting to " << hostn << ":" << port << Log::ENDL;
		
		if (lattice_connect((char*) hostn.c_str(), port) < 0) return false;
		return true;
	}
	
	/// from main (worldman) thread ///
	void Network::handleNetworking()
	{
		mtx.lock();
		if (player.changedPosition || player.changedRotation)
		{
			ntt.pmoved   = player.changedPosition;
			ntt.protated = player.changedRotation;
			
			if (ntt.pmoved)   ntt.pcoord = PackCoord(player.X, player.Y, player.Z);
			if (ntt.protated) ntt.prot   = vec2(player.xrotrad, player.yrotrad);
		}
		
		// receive blocks from network thread
		while (ntt.incoming.size())
		{
			NetworkBlock& block = ntt.incoming.front();
			
			// unpack position
			UnpackCoord coords(block.wc, block.bc);
			
			if (coords.valid)
			{
				block_t id = block.block.getID();
				block_t bf = block.block.getFacing();
				
				//logger << Log::INFO << "block: " << id << ", " << bf << Log::ENDL;
				
				// make world modification
				switch (block.type)
				{
					case NetworkBlock::BADD:
						Spiders::addblock(coords.bx, coords.by, coords.bz, id, bf, true);
						break;
					case NetworkBlock::BSET:
						Spiders::addblock(coords.bx, coords.by, coords.bz, id, bf, true);
						break;
					case NetworkBlock::BREM:
						Spiders::removeBlock(coords.bx, coords.by, coords.bz, true);
						break;
				}
				
			} // valid coordinates
			else
			{
				logger << Log::INFO << "Did not add block" << Log::ENDL;
			}
			ntt.incoming.pop_front();
			
		} // incoming block queue
		
		mtx.unlock();
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
				
				lp.wcoord = ntt.pcoord.wc;
				// block coordinates
				lp.bcoord = ntt.pcoord.bc;
				
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
	
	void Network::addBlock(direction_t dir, const NetworkBlock& block)
	{
		network.mtx.lock();
		if (dir == INCOMING)
		{
			network.ntt.incoming.push_front(block);
		}
		else
		{
			network.ntt.outgoing.push_front(block);
		}
		network.mtx.unlock();
		
	}
}
