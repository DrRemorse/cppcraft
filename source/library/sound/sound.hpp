#ifndef SOUND_HPP
#define SOUND_HPP

#include "../math/vector.hpp"
#include "bass.h"
#include <string>

namespace library
{
	class Sound
	{
	private:
		HSAMPLE handle;
		
	public:
		Sound();
		Sound(std::string fname);
		Sound(std::string fname, int samples);
		
		void load(std::string filename, int samples);
		bool destroy();
		void play(vec3);
		void play();
		
		static const float MAX_PAN_DIST;
		static const float MAX_VOL_DIST;
	};
}

#endif
