#ifndef STREAM_CHANNEL_HPP
#define STREAM_CHANNEL_HPP

namespace library
{
	class Stream;
	
	class StreamChannel
	{
	public:
		StreamChannel(float delta, float maxv);
		
		void play(Stream& newStream);
		void stop();
		void integrate();
		
	private:
		Stream* current;
		Stream* previous;
		float volA;
		float volB;
		
		float delta;
		float maxVolume;
	};
}

#endif
