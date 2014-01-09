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
		static const int CloudVertices = (SkyPattern + 1) * (SkyPattern + 1);
		
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
		
		GLushort  elements[clouds.SkyPattern * clouds.SkyPattern * 4];
		GLushort* ed = elements;
		
		for (int skyX = 0; skyX <= clouds.SkyPattern; skyX++)
		{
			float x = skyDelta * skyX - skySize;
			
			for (int skyZ = 0; skyZ <= clouds.SkyPattern; skyZ++)
			{
				float z = skyDelta * skyZ - skySize;
				
				// vertex
				cd->x = x;
				cd->y = skyLevel;
				cd->z = z;
				cd += 1;
			}
		}
		
		for (int skyX = 0; skyX < clouds.SkyPattern; skyX++)
		{
			for (int skyZ = 0; skyZ < clouds.SkyPattern; skyZ++)
			{
				// indices
				ed[0] = skyX * clouds.SkyPattern + skyZ;
				ed[1] = skyX * clouds.SkyPattern + skyZ + 1;
				ed[2] = (skyX + 1) * clouds.SkyPattern + skyZ + 1;
				ed[3] = (skyX + 1) * clouds.SkyPattern + skyZ;
				ed += 4;
			}
		}
		
		// make vertex array object
		clouds.vao.begin(sizeof(Clouds::cloudvertex_t), clouds.CloudVertices, cdata);
		clouds.vao.indexes(elements, clouds.SkyPattern * clouds.SkyPattern * 4);
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
		
		mat4 matclouds = camera.getRotationMatrix();
		matclouds.translate_xy(0.0, dy);
		
		// view matrix
		shd.sendMatrix("matview", matclouds);
		
		// clouds texture
		textureman.bind(0, Textureman::T_CLOUDS);
		
		// render
		clouds.vao.renderIndexed(GL_QUADS);
		
	} // render
}
