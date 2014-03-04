#ifndef RENDERSKY_HPP
#define RENDERSKY_HPP

#include <library/math/matrix.hpp>

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
		void render(Camera& camera, float cloudLevel, double time, int mode);
		void renderClouds(float dy, Camera& camera, double frameCounter);
		
		library::mat4 renderSunProj();
		
	};
	extern SkyRenderer skyrenderer;
}

#endif
