#ifndef NETWORK_HPP
#define NETWORK_HPP

#include <thread>

namespace cppcraft
{
	class NetworkThreadTransfer
	{
	public:
		double playerXYZ[3];
		
	};
	
	class Network
	{
	public:
		void init();
		void stop();
		
		void handleNetworking();
		
	private:
		void mainLoop();
		bool connect();
		
		bool running;
		std::thread networkThread;
		// all the stuff we want to copy between the threads now and then, 
		// just to have it somewhere visible
		NetworkThreadTransfer ntt;
		
	};
	extern Network network;
}

#endif // NETWORK_HPP
