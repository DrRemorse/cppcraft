#include "stream.hpp"

#include "../math/toolbox.hpp"
#include "../log.hpp"

namespace library
{
	float Stream::masterVolume = 1.0;
	Stream::Stream() {}
	
	Stream::Stream(std::string s): handle(0)
	{
		load(s);
	}
	
	void Stream::load (std::string s)
	{
		this->handle = BASS_StreamCreateFile(FALSE, s.c_str(), 0, 0, BASS_SAMPLE_LOOP);
		
		if (BASS_ErrorGetCode())
		{
			throw std::string("Stream::load(): BASS failed to load stream: " + s);
		}
	}
	
	bool Stream::destroy()
	{
		if (handle) return BASS_StreamFree(handle);
		return false;
	}
	
	bool Stream::play()
	{
		if (!handle) 
			throw std::string("Stream::play(): Failed to play stream");
		
		return BASS_ChannelPlay(handle, TRUE) != 0;
	}
	
	bool Stream::isPlaying() const noexcept
	{
		if (!handle) return false;
		return BASS_ChannelIsActive(handle) == BASS_ACTIVE_PLAYING;
	}
	
	void Stream::stop()
	{
		BASS_ChannelStop(handle);
	}
	
	void Stream::setVolume(float vol)
	{
		vol = toolbox::clamp(0.0, 1.0, vol);
		BASS_ChannelSetAttribute(handle, BASS_ATTRIB_VOL, vol * masterVolume);
		
		if (BASS_ErrorGetCode())
		{
			logger << Log::ERR << "Stream::setVolume(): failed to set volume " << vol << " to stream " << handle << Log::ENDL;
			throw std::string("Stream::setVolume(): BASS failed to set new volume");
		}
	}
	
	void Stream::setMasterVolume(float vol)
	{
		Stream::masterVolume = vol;
	}
}
