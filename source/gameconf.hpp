#ifndef GAMECONF_HPP
#define GAMECONF_HPP

namespace cppcraft
{
	class GameConfig
	{
	public:
		void init();
		
		bool sounds;
		bool music;
		bool ambience;
		float master_volume;
		
		int supersampling;
		int multisampling;
		float anisotropy;
		
		bool highq_sky;
		bool distance_blur;
		bool lensflare;
		
		bool clouds;
		bool reflections;
		bool hq_reflections;
		bool reflectTerrain;
		
		bool playerhand;
		
	};
	extern GameConfig gameconf;
}

#endif
