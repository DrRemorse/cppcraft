#ifndef NETWORK_HPP
#define NETWORK_HPP

#include <library/math/vector.hpp>
#include <liblattice/coordinates.h>
#include "blocks.hpp"
#include "world.hpp"
#include <deque>
#include <mutex>
#include <thread>

struct lattice_bump;

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
		PackCoord(Sector* sector, int bx, int by, int bz);
		
		w_coord wc;
		b_coord bc;
		bool valid;
	};
	class UnpackCoord
	{
	public:
		// unpack integral coordinates (0 bit fractions)
		UnpackCoord(w_coord& wc, b_coord& bc);
		
		int bx, by, bz;
		bool valid;
	};
	class UnpackCoordF
	{
	public:
		// unpack precision coordinates (8 bit fractions)
		UnpackCoordF(w_coord& wc, b_coord& bc);
		
		w_coord wc;
		library::vec3 bc;
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
		
		NetworkBlock() {}
		NetworkBlock(int bx, int by, int bz, const Block& block, btype_t type);
		
		btype_t type;
		Block block;
		w_coord wc;
		b_coord bc;
	};
	
	class NetworkThreadTransfer
	{
	public:
		bool pmoved, protated;
		// true if the last package was sent as player having moved
		bool psentmoved;
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
		
		friend void bumpError(lattice_bump* bump);
	};
	extern Network network;
}

#endif // NETWORK_HPP
