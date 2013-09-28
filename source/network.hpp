#ifndef NETWORK_HPP
#define NETWORK_HPP

#include <thread>

namespace cppcraft
{
	
	class Network
	{
	public:
		void init();
		void stop();
		
	private:
		void mainLoop();
		bool connect();
		
		bool running;
		std::thread networkThread;
		
	};
	extern Network network;
}

#endif // NETWORK_HPP
