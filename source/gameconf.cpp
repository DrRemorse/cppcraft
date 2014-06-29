#include "gameconf.hpp"

#include <library/log.hpp>
#include <library/config.hpp>

using namespace library;

namespace cppcraft
{
	GameConfig gameconf;
	
	void GameConfig::init()
	{
		// rendering
		supersampling = config.get("render.supersampling", 1);
		multisampling = config.get("render.multisampling", 0);
		anisotropy    = config.get("render.anisotropy", 1.0f);
		
		highq_water   = config.get("render.highq_water",   true);
		highq_sky     = config.get("render.highq_sky",     false);
		distance_blur = config.get("render.distance_blur", true);
		lensflare     = config.get("render.lensflare",     true);
		
		clouds         = config.get("render.clouds",  false);
		reflections    = config.get("render.reflections", true);
		hq_reflections = config.get("render.hq_reflections", false);
		reflectTerrain = config.get("render.reflect_terrain", false);
		
		playerhand    = config.get("playerhand", true);
		
		// audio
		sounds   = config.get("sound.sounds", true);
		music    = config.get("sound.music", true);
		ambience = config.get("sound.ambient", true);
		
		// minimum 1x supersampling
		if (supersampling < 1) supersampling = 1;
		// "disable" 1x multisampling
		if (multisampling < 2) multisampling = 0; else supersampling = 1;
		// minimum 1x anisotropic filter size
		if (anisotropy < 1.0f) anisotropy = 1.0f;
	}
}
