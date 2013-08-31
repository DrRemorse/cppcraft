#include "sound.hpp"

#include "../log.hpp"
#include "../math/toolbox.hpp"

namespace library
{
	float Sound::masterVolume = 1.0;
	const float Sound::MAX_PAN_DIST = 100.0f;
	const float Sound::MAX_VOL_DIST = 25.0f;
	
	Sound::Sound()
	{
		handle = 0;
	}
	
	Sound::Sound(std::string s): handle(0)
	{
		load(s, 1);
	}
	Sound::Sound(std::string s, int samples): handle(0)
	{
		load(s, samples);
	}
	
	void Sound::load(std::string s, int samples)
	{
		this->handle = BASS_SampleLoad(FALSE, s.c_str(), 0, 0, samples, BASS_SAMPLE_OVER_VOL);
		
		if (BASS_ErrorGetCode())
		{
			logger << Log::ERR << "Sound::load(): BASS_SampleLoad error: " << BASS_ErrorGetCode() << Log::ENDL;
			throw std::string("Sound::load(): BASS sample file: " + s);
		}
		
		if (this->handle == 0)
			throw std::string("Sound::load(): Invalid handle from BASS_SampleLoad");
		
		HCHANNEL ch = BASS_SampleGetChannel( this->handle, FALSE );
		if (ch == 0) throw std::string("Sound::load(): Invalid channel handle for " + s);
	}
	
	bool Sound::destroy()
	{
		if (handle) return BASS_SampleFree(handle);
		return false;
	}
	
	void Sound::play()
	{
		HCHANNEL ch = BASS_SampleGetChannel(this->handle, FALSE);
		if (ch == 0) throw std::string("Sound::play(): Invalid handle from BASS_SampleGetChannel");
		
		BASS_ChannelSetAttribute(ch, BASS_ATTRIB_VOL, masterVolume);
		BASS_ChannelPlay(ch, FALSE);
	}
	
	// play stereo sound based on positional offset vector
	void Sound::play(vec3 v)
	{
		float L = v.length(); // distance from origin
		v.normalize();
		float pan = v.x * toolbox::min(1.0, L / MAX_PAN_DIST);
		float vol = toolbox::clamp(0.0, 1.0, L / MAX_VOL_DIST);
		
		HCHANNEL ch = BASS_SampleGetChannel(handle, FALSE);
		BASS_ChannelSetAttribute(ch, BASS_ATTRIB_VOL, vol * masterVolume);
		BASS_ChannelSetAttribute(ch, BASS_ATTRIB_PAN, pan);

		if (!BASS_ChannelPlay(ch, FALSE))
			throw std::string("Sound::play(vec3): Error playing (stereoized) sound");
	}
	
	void Sound::setMasterVolume(float vol)
	{
		Sound::masterVolume = vol;
	}
}
