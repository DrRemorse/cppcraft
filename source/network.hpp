#ifndef NETWORK_HPP
#define NETWORK_HPP

#include <library/math/vector.hpp>
#include "world.hpp"
#include <mutex>
#include <thread>

namespace cppcraft
{
	class SpiderCoord
	{
	public:
		SpiderCoord() {}
		SpiderCoord(double x, double y, double z);
		
		World::wcoord_t wcoord[3];
		library::vec3   scoord;
	};
	
	class NetworkThreadTransfer
	{
	public:
		bool pmoved, protated;
		// player coordinates
		SpiderCoord pcoord;
		// player rotation
		library::vec2 prot;
	};
	
	class Network
	{
	public:
		void init();
		void stop();
		
		void handleNetworking();
		
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
