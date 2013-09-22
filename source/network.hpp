#ifndef NETWORK_HPP
#define NETWORK_HPP

namespace cppcraft
{
	
	class Network
	{
	public:
		void init();
		
		void start();
		
	private:
		void connect();
		
	};
	extern Network network;
}

#endif // NETWORK_HPP
