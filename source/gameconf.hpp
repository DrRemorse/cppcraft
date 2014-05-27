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
		
		bool clouds;
		bool reflections;
		bool reflectTerrain;
		bool postprocess;
		bool lensflare;
		
		bool playerlight;
		bool playerhand;
		
	};
	extern GameConfig gameconf;
}

#endif
