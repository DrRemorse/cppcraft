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
		multisampling = config.get("render.multisampling", 0);
		anisotropy    = config.get("render.anisotropy", 1.0f);
		
		clouds        = config.get("render.clouds",  false);
		reflections   = config.get("render.reflections", true);
		reflectTerrain= config.get("render.reflect_terrain", false);
		
		postprocess   = config.get("render.postprocess", true);
		lensflare     = config.get("render.lensflare",   true);
		
		playerlight   = config.get("playerlight", true);
		playerhand    = config.get("playerhand", true);
		
		// audio
		sounds   = config.get("sound.sounds", true);
		music    = config.get("sound.music", true);
		ambience = config.get("sound.ambient", true);
		
	}
}
