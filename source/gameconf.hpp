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
		
		int multisampling;
		float anisotropy;
		
		bool postprocess;
		bool highq_blur;
		bool lensflare;
		
		bool playerlight;
		bool fog;
		bool playerhand;
		
		
	};
	extern GameConfig gameconf;
}

#endif
