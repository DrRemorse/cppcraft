#ifndef NETWORK_HPP
#define NETWORK_HPP

#include <library/math/vector.hpp>
#include <liblattice/coordinates.h>
#include "blocks.hpp"
#include "world.hpp"
#include <deque>
#include <mutex>
#include <thread>

namespace cppcraft
{
	class Sector;
	
	class PackCoord
	{
	public:
		PackCoord() {}
		// packed precision coordinates (8 bit fractions)
		PackCoord(double x, double y, double z);
		// packed integral coordinates (0 bit fractions)
		PackCoord(int bx, int by, int bz);
		
		w_coord wc;
		b_coord bc;
		bool valid;
	};
	class UnpackCoord
	{
	public:
		UnpackCoord() {}
		// unpack integral coordinates (0 bit fractions)
		UnpackCoord(w_coord& wc, b_coord& bc);
		
		int bx, by, bz;
		bool valid;
	};
	
	class NetworkBlock
	{
	public:
		enum btype_t
		{
			BADD,
			BREM,
			BSET
		};
		
		btype_t type;
		Block block;
		w_coord wc;
		b_coord bc;
	};
	
	class NetworkThreadTransfer
	{
	public:
		bool pmoved, protated;
		// player coordinates
		PackCoord pcoord;
		// player rotation
		library::vec2 prot;
		
		std::deque<NetworkBlock> incoming;
		std::deque<NetworkBlock> outgoing;
	};
	
	class Network
	{
	public:
		enum direction_t
		{
			INCOMING,
			OUTGOING
		};
		
		void init();
		void stop();
		
		void handleNetworking();
		
		void addBlock(direction_t, const NetworkBlock&);
		
	private:
		bool connect();
		
		void mainLoop();
		void handleTransfer();
		
		bool running;
		std::mutex  mtx;
		std::thread networkThread;
		// all the stuff we want to copy between the threads now and then, 
		// just to have it somewhere visible
		NetworkThreadTransfer ntt;
		
	};
	extern Network network;
}

#endif // NETWORK_HPP
