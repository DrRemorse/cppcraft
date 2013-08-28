#ifndef SHADERMAN_HPP
#define SHADERMAN_HPP

#include "library/opengl/shader.hpp"
#include <string>

namespace library
{
	class Matrix;
	class WindowClass;
}

namespace cppcraft
{
	class Shaderman
	{
	public:
		void init(library::WindowClass& gamescr, const library::Matrix& matproj, const library::Matrix& matproj_long);
		
		typedef enum
		{
			ATMOSPHERE,
			CLOUDS,
			SUN, SUNPROJ,
			MOON,
			
			STD_BLOCKS,
			CULLED_BLOCKS,
			ALPHA_BLOCKS,
			BLOCKS_WATER,
			FSTERRAIN,
			
			OBJECT,
			VOXELOBJECT,
			ACTOR,
			PARTICLE,
			SELECTION,
			
			PLAYERHAND,
			HANDITEM,
			VOXEL,
			PLAYERMODEL,
			
			GAUSS,
			BLUR,
			LENSFLARE,
			CREPUSCULAR,
			POSTPROCESS,
			
			COMPASS,
			MINIMAP,
			FONT,
			GUI,
			MENUITEM,
			CHAT,
			
			NUMBER_OF_SHADERS
			
		} shaderlist_t;
		
		library::Shader& operator[] (shaderlist_t);
		
	private:
		library::Shader shaders[NUMBER_OF_SHADERS];
	};
	extern Shaderman shaderman;
}

#endif
