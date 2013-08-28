#include "render_sky.hpp"

#include "library/opengl/opengl.hpp"
#include "frustum.hpp"
#include "render_scene.hpp"
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
		
		GLuint vao = 0;
	};
	Clouds clouds;
	
	void SkyRenderer::createClouds()
	{
		// create cloud VAO
		float skySize    = frustum.cameraViewSectors * Sector::BLOCKS_XZ * 1.6;
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
		glGenVertexArrays(1, &clouds.vao);
		// bind it
		glBindVertexArray(clouds.vao);
		
		GLuint vbo;
		// make vbo
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER_ARB, vbo);
		// upload cloud data to GPU
		glBufferData(GL_ARRAY_BUFFER_ARB, clouds.CloudVertices * sizeof(Clouds::cloudvertex_t), cdata, GL_STATIC_DRAW_ARB);
		
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Clouds::cloudvertex_t), 0);
		
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER_ARB, 0);
	}
	
	void SkyRenderer::renderClouds(SceneRenderer& scene, double frameCounter)
	{
		// bind cloud shader
		Shader& shd = shaderman[Shaderman::CLOUDS];
		shd.bind();
		
		shd.sendFloat("frameCounter", frameCounter);
		shd.sendVec3 ("worldOffset",  frustum.getWorldOffset());
		shd.sendVec3 ("lightVector",  thesun.getRealtimeAngle());
		shd.sendFloat("daylight",     thesun.getRealtimeDaylight());
		
		if (frustum.ref)
		{
			Matrix matclouds = frustum.getRotationMatrix();
			matclouds.translated(0.0, -scene.playerY, 0.0);
			
			// view matrix
			shd.sendMatrix("matview", matclouds);
			
			if (frustum.rotated)
			{
				// rotation matrix
				shd.sendMatrix("matrot", frustum.getRotationMatrix());
			}
		}
		
		textureman.bind(0, Textureman::T_CLOUDS);
		
		glBindVertexArray(clouds.vao);
		glDrawArrays(GL_QUADS, 0, clouds.CloudVertices);
	} // render
}
