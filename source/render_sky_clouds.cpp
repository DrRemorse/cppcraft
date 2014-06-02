#include "render_sky.hpp"

#include <library/opengl/opengl.hpp>
#include <library/opengl/vao.hpp>
#include <library/log.hpp>
#include "camera.hpp"
#include "renderconst.hpp"
#include "shaderman.hpp"
#include "sun.hpp"
#include "textureman.hpp"
#include <cmath>

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
		
		static const int SkyPattern = 16;
		
		VAO vao;
	};
	Clouds clouds;
	
	void SkyRenderer::createClouds()
	{
		static const int CLOUD_VERTICES = (Clouds::SkyPattern + 1) * (Clouds::SkyPattern + 1);
		
		// create cloud VAO
		float skySize    = camera.getZFar() * 1.5;
		float skyDelta   = skySize * 2.0 / clouds.SkyPattern;
		const float skyLevel = RenderConst::SKY_LEVEL + 0.5;
		
		Clouds::cloudvertex_t  cdata[CLOUD_VERTICES];
		Clouds::cloudvertex_t* cd = cdata;
		
		for (int skyZ = 0; skyZ <= clouds.SkyPattern; skyZ++)
		{
			float z = skyDelta * skyZ - skySize;
			for (int skyX = 0; skyX <= clouds.SkyPattern; skyX++)
			{
				float x = skyDelta * skyX - skySize;
				
				float rad = std::sqrt(x*x + z*z);
				
				// vertex
				cd->x = x;
				cd->y = skyLevel - rad / skySize * 80.0;
				cd->z = z;
				cd += 1;
			}
		}
		
		GLushort  elements[clouds.SkyPattern * clouds.SkyPattern * 4];
		GLushort* ed = elements;
		
		const int width = Clouds::SkyPattern + 1;
		const int height = Clouds::SkyPattern;
		int i = 0;
		
		for(int y = 0; y < height; y++)
		{
			int base = y * width;
			
			for(int x = 0; x < width; x++)
			{
				ed[i++] = base + x;
				ed[i++] = base + width + x;
			}
			// add a degenerate triangle (except in a last row)
			if (y < height - 1)
			{
				ed[i++] = (y + 1) * width + (width-1);
				ed[i++] = (y + 1) * width;
			}
		}		
		
		// make vertex array object
		clouds.vao.begin(sizeof(Clouds::cloudvertex_t), CLOUD_VERTICES, cdata);
		clouds.vao.indexes(elements, i);
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
		clouds.vao.renderIndexed(GL_TRIANGLE_STRIP);
		
	} // render
}
