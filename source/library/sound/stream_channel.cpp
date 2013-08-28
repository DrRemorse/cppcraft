#include "stream_channel.hpp"

#include "stream.hpp"

namespace library
{
	
	StreamChannel::StreamChannel(float delta, float maxv)
	{
		this->current = this->previous = nullptr;
		this->volA = this->volB = 0;
		this->delta = delta;
		this->maxVolume = maxv;
	}
	
	void StreamChannel::stop()
	{
		// nothing to do if the stream is already ending
		if (current == nullptr) return;
	
		// fade-out old stream, set current to null
		previous = current;
		current  = nullptr;
		volB = volA;
		volA = 0;
	}
	
	void StreamChannel::play(Stream& newStream)
	{
		// nothing to do if the new stream is the same as the current
		if (&newStream == current) return;
		// now, find out if the new current is the previous
		if (&newStream == previous)
		{
			// ye olde switcharoo
			float v = volA;
			volA = volB; volB = v;
			Stream* m = current;
			current = previous; previous = m;
			return;
		}
		// otherwise, kill the previous stream, if it's still playing
		if (previous) previous->stop();
		// set current to previous
		previous = current;
		volB = volA;
		// set new current
		current = &newStream;
		current->play();
		current->setVolume(0);
		volA = 0;
	}
	
	void StreamChannel::integrate()
	{
		if (current)
		{
			if (volA < maxVolume)
			{
				volA += delta;
				current->setVolume(volA);
			}
		}
		if (previous)
		{
			volB -= delta;
			// if volume is not yet zero, continue fade-out
			if (volB > 0)
			{
				previous->setVolume(volB);
			}
			else
			{
				// stop stream entirely, and also setting it to null
				// telling us that there is no previous stream anymore
				previous->stop();
				previous = nullptr;
				volB = 0;
			}
		}
	}
	
}