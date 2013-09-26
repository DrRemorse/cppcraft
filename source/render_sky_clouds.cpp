#include "render_sky.hpp"

#include "library/opengl/opengl.hpp"
#include "library/opengl/vao.hpp"
#include "camera.hpp"
#include "renderconst.hpp"
#include "sectors.hpp"
#include "shaderman.hpp"
#include "sun.hpp"
#include "textureman.hpp"

using namespace library;

namespace cppcraft
{
	class Clouds
	{
	public:
		struct cloudvertex_t
		{
			GLfloat x, y, z;
		};
		
		static const int SkyPattern = 32;
		static const int CloudVertices = SkyPattern * SkyPattern * 4;
		
		VAO vao;
	};
	Clouds clouds;
	
	void SkyRenderer::createClouds()
	{
		// create cloud VAO
		float skySize    = camera.cameraViewSectors * Sector::BLOCKS_XZ * 1.6;
		float skyDelta   = skySize / clouds.SkyPattern * 2.0;
		const float skyLevel = RenderConst::SKY_LEVEL - 0.5;
		
		Clouds::cloudvertex_t  cdata[clouds.CloudVertices];
		Clouds::cloudvertex_t* cd = cdata;
		
		for (int skyX = 0; skyX < clouds.SkyPattern; skyX++)
		{
			float x = skyDelta * skyX - skySize;
			
			for (int skyZ = 0; skyZ < clouds.SkyPattern; skyZ++)
			{
				float z = skyDelta * skyZ - skySize;
				
				// v0
				cd->x = x;
				cd->y = skyLevel;
				cd->z = z;
				cd += 1;
				
				// v1
				cd->x = x + skyDelta;
				cd->y = skyLevel;
				cd->z = z;
				cd += 1;
				
				// v2
				cd->x = x + skyDelta;
				cd->y = skyLevel;
				cd->z = z + skyDelta;
				cd += 1;
				
				// v3
				cd->x = x;
				cd->y = skyLevel;
				cd->z = z + skyDelta;
				cd += 1;
			}
		}
		
		// make vertex array object
		clouds.vao.begin(sizeof(Clouds::cloudvertex_t), clouds.CloudVertices, cdata);
		clouds.vao.attrib(0, 3, GL_FLOAT, GL_FALSE, 0);
		clouds.vao.end();
	}
	
	void SkyRenderer::renderClouds(float dy, Camera& camera, double frameCounter)
	{
		// bind cloud shader
		Shader& shd = shaderman[Shaderman::CLOUDS];
		shd.bind();
		
		shd.sendFloat("frameCounter", frameCounter);
		shd.sendVec3 ("worldOffset",  camera.getWorldOffset());
		shd.sendVec3 ("lightVector",  thesun.getRealtimeAngle());
		shd.sendFloat("daylight",     thesun.getRealtimeDaylight());
		
		Matrix matclouds = camera.getRotationMatrix();
		matclouds.translated(0.0, dy, 0.0);
		
		// view matrix
		shd.sendMatrix("matview", matclouds);
		
		// clouds texture
		textureman.bind(0, Textureman::T_CLOUDS);
		
		// render
		clouds.vao.render(GL_QUADS);
		
	} // render
}
