#include "gameconf.hpp"

#include "library/log.hpp"
#include "library/config.hpp"

using namespace library;

namespace cppcraft
{
	GameConfig gameconf;
	
	void GameConfig::init()
	{
		// rendering
		multisampling = config.get("multisampling", 0);
		anisotropy    = config.get("anisotropy", 2.0f);
		
		postprocess   = config.get("postprocess", true);
		highq_blur    = config.get("highq_blur", true);
		lensflare     = config.get("lensflare", true);
		
		playerlight   = config.get("playerlight", true);
		playerhand    = config.get("playerhand", true);
		
		// audio
		sounds   = config.get("sound.sounds", true);
		music    = config.get("sound.music", true);
		ambience = config.get("sound.ambient", true);
		
		
	}
}