#include "network.hpp"

#include <library/log.hpp>
#include <library/config.hpp>
#include "netplayers.hpp"
#include "player.hpp"
#include "player_logic.hpp"
#include "sectors.hpp"
#include "spiders.hpp"
#include "world.hpp"
#include <cstring>
#include <cmath>

// REMOVE ME -->
#include <cstdlib>     /* srand, rand */
#include <ctime>       /* time */
// <--

static const double PI2 = 4 * atan(1) * 2;

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
	#include <liblattice/client_commands.h>
}

namespace cppcraft
{
	Network network;
	void message(lattice_message* mp);
	
	void Network::init()
	{
		this->connected = false;
		this->running = true;
		this->networkThread = std::thread(&Network::mainLoop, this);
		
		/// testing ///
		netplayers.createTestPlayer();
		
		timer.startNewRound();
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
	void userAdded(NetPlayer::userid_t userid, lattice_user* user)
	{
		UnpackCoordF coord(user->wpos, user->bpos);
		// create netplayer
		NetPlayer nplayer(userid, user->nickname, coord.wc, coord.bc);
		// add to active netplayers list
		netplayers.add(nplayer);
		
		logger << Log::INFO << "User joined: " << user->nickname << Log::ENDL;
	}
	void userQuits(NetPlayer::userid_t userid, lattice_quit* msg)
	{
		NetPlayer* np = netplayers.playerByUID(userid);
		if (np)
		{
			logger << Log::INFO << "User quit: " << np->getName() << " (" << &msg->desc << ")" << Log::ENDL;
			netplayers.remove(userid);
		}
	}
	void userMoved(NetPlayer::userid_t userid, lattice_p* movement)
	{
		NetPlayer* np = netplayers.playerByUID(userid);
		if (np)
		{
			if (movement)
			{
				UnpackCoordF coord(movement->wcoord, movement->bcoord);
				np->moveTo(coord.wc, coord.bc);
			}
			else np->stopMoving();
		}
	}
	void userRotated(NetPlayer::userid_t userid, lattice_pr* rotated)
	{
		NetPlayer* np = netplayers.playerByUID(userid);
		if (np)
		{
			vec2 rot(rotated->rot.xrot, rotated->rot.yrot);
			rot *= PI2 / 4096;
			//netplayers.updateRotation(rot);
			np->setRotation(rot);
		}
	}
	
	void bumpError(lattice_bump* bump)
	{
		PackCoord& pc = network.ntt.pcoord;
		logger << Log::INFO << "Player: (" << pc.wc.x << "," << pc.wc.y << "," << pc.wc.z << ") bxyz (" << pc.bc.x << "," << pc.bc.y << "," << pc.bc.z << ")" << Log::ENDL;
		//logger << Log::INFO << "I'm supposedly outside of all servers?" << Log::ENDL;
		logger << Log::INFO << "Bump: (" << bump->bad_wcoord.x << "," << bump->bad_wcoord.y << "," << bump->bad_wcoord.z << ") bxyz (" << bump->bad_bcoord.x << "," << bump->bad_bcoord.y << "," << bump->bad_bcoord.z << ")" << Log::ENDL;
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
			bumpError((lattice_bump*) mp->args);
			break;
		case T_CONNECTED:
			logger << Log::INFO << "Connected to server" << Log::ENDL;
			network.connected = true;
			break;
		case T_DISCONNECTED:
			logger << Log::WARN << "Disconnected from server" << Log::ENDL;
			network.connected = false;
			break;
			
		case T_SAT:
			logger << Log::INFO << "SAT angle: " << ((double*) mp->args)[0] << Log::ENDL;
			break;
			
		case T_SATSTEP:
			logger << Log::INFO << "SAT step: " << ((int*) mp->args)[0] << Log::ENDL;
			break;
			
		case T_P:
			userMoved(mp->fromuid, (lattice_p*) mp->args);
			break;
		case T_PR:
			userRotated(mp->fromuid, (lattice_pr*) mp->args);
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
			logger << Log::INFO << "SERVER  " << ((char*) mp->args) << Log::ENDL;
			break;
			
		case T_USER:
			userAdded(mp->fromuid, (lattice_user*) mp->args);
			break;
		case T_FADE:
		case T_QUIT:
			userQuits(mp->fromuid, (lattice_quit*) mp->args);
			break;
			
		default:
			logger << Log::INFO << "Unimplemented message type: " << mp->type << Log::ENDL;
		}
		
		return;
	}
	
	bool Network::connect()
	{
		std::string  hostn = config.get("net.host", "127.0.0.1");
		unsigned int port  = config.get("net.port", 8805);
		
		std::string uname  = config.get("net.user", "guest");
		std::string upass  = config.get("net.pass", "guest");
		
		srand(time(0));
		/*
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<unsigned int> dis;
		
		lattice_player.userid = dis(gen);
		*/
		lattice_player.userid = rand();
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
		
		logger << Log::INFO << "Connecting to " << hostn << ":" << port << " as " << lattice_player.userid << Log::ENDL;
		
		if (lattice_connect((char*) hostn.c_str(), port) < 0) return false;
		return true;
	}
	
	/// from main (worldman) thread ///
	void Network::handleNetworking()
	{
		mtx.lock();
		{
			ntt.pmoved   = player.changedPosition;
			if (ntt.pmoved)   ntt.pcoord = PackCoord(player.X, player.Y, player.Z);
			ntt.protated = player.changedRotation;
			if (ntt.protated) ntt.prot   = vec2(player.xrotrad, player.yrotrad);
			
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
			
		}
		mtx.unlock();
		
		// handle players - interpolate movement and determine if renderable
		netplayers.handlePlayers(1.0);
	}
	
	/// from networking scheduler ///
	void Network::handleTransfer()
	{
		mtx.lock();
		if (this->connected)
		{
			static const double UPDATE_INTERVAL = 0.200;
			static double lastTime = 0.0;
			
			double deltat = timer.getDeltaTime();
			if (deltat > lastTime + UPDATE_INTERVAL)
			{
				lastTime = deltat;
				
				// update position & rotation
				if (ntt.pmoved)
				{
					// start moving
					c_p(ntt.pcoord.wc, ntt.pcoord.bc);
					ntt.psentmoved = true;
				}
				else if (ntt.psentmoved)
				{
					// stop player
					c_p_empty();
					ntt.psentmoved = false;
				}
				if (ntt.protated)
				{
					head_rot rot;
					rot.xrot = ntt.prot.x / PI2 * 4096;
					rot.yrot = ntt.prot.y / PI2 * 4096;
					c_pr(rot);
				}
				//logger << Log::INFO << "Send position updates at " << deltat << Log::ENDL;
			}
			
			// send blocks to remote host
			while (ntt.outgoing.size())
			{
				NetworkBlock& nb = ntt.outgoing.front();
				
				::block_t block;
				block.id = nb.block.getID();
				block.bf = nb.block.getData() >> 10;
				
				// make world modification
				switch (nb.type)
				{
				case NetworkBlock::BADD:
					c_badd(nb.wc, nb.bc, block);
					break;
				case NetworkBlock::BSET:
					c_bset(nb.wc, nb.bc, block);
					break;
				case NetworkBlock::BREM:
					c_brem(nb.wc, nb.bc);
					break;
				}
				// remove front block from queue
				ntt.outgoing.pop_front();
				
			} // outgoing block queue
		}
		mtx.unlock();
	}
	
	void Network::addBlock(direction_t dir, const NetworkBlock& block)
	{
		mtx.lock();
		if (dir == INCOMING)
		{
			ntt.incoming.push_front(block);
		}
		else
		{
			ntt.outgoing.push_front(block);
		}
		mtx.unlock();
	}
}
