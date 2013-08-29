#ifndef RENDERSKY_HPP
#define RENDERSKY_HPP

#include "library/math/matrix.hpp"

namespace cppcraft
{
	class SceneRenderer;
	
	class SkyRenderer
	{
	private:
		void createClouds();
		void renderSun();
		void renderMoon();
		
	public:
		void init();
		void render(SceneRenderer& scene, bool underwater);
		void renderClouds(SceneRenderer& scene, double frameCounter);
		
		library::Matrix renderSunProj(unsigned int samples);
		
	};
	extern SkyRenderer skyrenderer;
}

#endif
