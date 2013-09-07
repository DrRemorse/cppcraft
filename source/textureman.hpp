#ifndef TEXTUREMAN_HPP
#define TEXTUREMAN_HPP

#include "library/opengl/texture.hpp"

namespace library
{
	class WindowClass;
}

namespace cppcraft
{
	class Textureman
	{
	public:
		typedef enum
		{
			T_BIG_DIFF,
			T_BIG_TONE,
			
			T_DIFFUSE,
			T_TONEMAP,
			
			T_ITEMS,
			T_ACTORS,
			T_PLAYERMODELS,
			T_PARTICLES,
			
			T_SELECTION,
			T_MINING,
			
			T_FONT,
			T_MINIMAP,
			T_CROSSHAIR,
			T_COMPASS,
			T_BARMETER,
			
			T_QUICKBAR,
			T_INVENTORY,
			T_WORKBENCH,
			T_CHEST,
			
			T_SKYBOX,
			T_CLOUDS,
			T_SUN,
			T_MOON,
			T_STARS,
			
			T_NOISE,
			T_LENSDIRT,
			T_LENSFLARE,
			T_LENSFLARE2,
			T_LENSFLARE3,
			
			T_BLURBUFFER1,
			T_BLURBUFFER2,
			
			T_FOGBUFFER,
			T_RENDERBUFFER,
			T_DEPTHBUFFER,
			T_SKYBUFFER,
			
			T_UNDERWATERMAP,
			T_FSNORMALS,
			
			NUM_TEXTURES
			
		} named_textures_t;
		
		Textureman();
		
		void init(library::WindowClass& gamescr);
		void bind(int, named_textures_t);
		void unbind(int);
		// returns texture handle
		GLuint get(named_textures_t);
		
		void copyScreen(library::WindowClass& gamescr, named_textures_t tx);
		
	private:
		library::Texture textures[NUM_TEXTURES];
		
		void generateTextures();
	};
	extern Textureman textureman;
}

#endif
