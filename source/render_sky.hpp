#ifndef RENDERSKY_HPP
#define RENDERSKY_HPP

#include "library/math/matrix.hpp"

namespace cppcraft
{
	class Camera;
	
	class SkyRenderer
	{
	private:
		void createClouds();
		void renderSun();
		void renderMoon(Camera& camera);
		
	public:
		void init();
		void render(Camera& camera, bool underwater);
		void renderClouds(float dy, Camera& camera, double frameCounter);
		
		library::Matrix renderSunProj();
		
	};
	extern SkyRenderer skyrenderer;
}

#endif
