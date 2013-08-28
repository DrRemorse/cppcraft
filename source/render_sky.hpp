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
		void render(SceneRenderer& scene, double dtime);
		void renderClouds(SceneRenderer& scene, double dtime);
		
		library::Matrix renderSunProj(unsigned int samples);
		
	};
	extern SkyRenderer skyrenderer;
}

#endif
