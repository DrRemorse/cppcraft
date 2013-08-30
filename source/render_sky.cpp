#include "render_sky.hpp"

#include "library/math/matrix.hpp"
#include "library/opengl/opengl.hpp"
#include "library/opengl/vao.hpp"
#include "atmosphere.hpp"
#include "frustum.hpp"
#include "renderconst.hpp"
#include "render_scene.hpp"
#include "renderman.hpp"
#include "sectors.hpp"
#include "shaderman.hpp"
#include "sun.hpp"
#include "textureman.hpp"
#include "world.hpp"
#include <cmath>

using namespace library;

namespace cppcraft
{
	SkyRenderer skyrenderer;
	VAO satteliteVAO;
	const double PI = 4.0 * atan(1);
	
	void SkyRenderer::init()
	{
		/// initialize atmosphere ///
		Atmosphere::init();
		/// initialize clouds ///
		createClouds();
		
		/// create VAO for all satellites ///
		struct satellite_t
		{
			float vx, vy, vz;
		};
		satellite_t mv[4];
		
		mv[0] = (satellite_t) { -0.5, -0.5, 0.0 };
		mv[1] = (satellite_t) {  0.5, -0.5, 0.0 };
		mv[2] = (satellite_t) {  0.5,  0.5, 0.0 };
		mv[3] = (satellite_t) { -0.5,  0.5, 0.0 };
		
		satteliteVAO.begin(sizeof(satellite_t), 4, mv);
		satteliteVAO.attrib(0, 3, GL_FLOAT, GL_FALSE, 0);
		satteliteVAO.end();
	}
	
	void SkyRenderer::render(SceneRenderer& scene, bool underwater)
	{
		// render background / atmosphere, sun and moon
		glDisable(GL_BLEND);
		
		// render sky dome
		Atmosphere::render(scene.playerY);
		
		// unbind shits
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER_ARB, 0);
		
		glEnable(GL_BLEND);
		glColorMask(1, 1, 1, 0);
		
		// render regular sun
		if (underwater)
		{
			renderSun();
		}
		
		// render moon texture
		renderMoon();
		
		glDisable(GL_BLEND);
		glColorMask(1, 1, 1, 1);
		
	}
	
	void SkyRenderer::renderSun()
	{
		//if ogl.lastsun(1) < -0.15 then return
		
		Shader& sunshader = shaderman[Shaderman::SUN];
		sunshader.bind();
		
		Matrix matsun, mattemp;
		mattemp.rotateZYX(0.0, -PI / 2, 0.0);
		matsun.rotateZYX(thesun.getRealtimeRadianAngle(), 0.0, 0.0);
		mattemp *= matsun;
		mattemp.translated(0.0, 0.0, -thesun.renderDist);
		
		matsun = frustum.getRotationMatrix() * mattemp;
		
		// view matrix
		sunshader.sendMatrix("matview", matsun);
		
		// rotation matrix
		sunshader.sendMatrix("matrot", frustum.getRotationMatrix());
		
		textureman.bind(0, Textureman::T_SUN);
		
		// rendering call
		satteliteVAO.render(GL_QUADS);
	}
	
	Matrix SkyRenderer::renderSunProj(GLuint samples)
	{
		Shader& sunshader = shaderman[Shaderman::SUNPROJ];
		sunshader.bind();
		
		Matrix matsun, mattemp;
		mattemp.rotateZYX(0.0, -PI / 2, 0.0);
		matsun.rotateZYX(thesun.getRealtimeRadianAngle(), 0.0, 0.0);
		mattemp *= matsun;
		mattemp.translated(0.0, 0.0, -thesun.renderDist);
		
		matsun = frustum.getRotationMatrix() * mattemp;
		
		// view matrix
		sunshader.sendMatrix("matview", matsun);
		// rotation matrix
		sunshader.sendMatrix("matrot", frustum.getRotationMatrix());
		
		// depth used as stencil buffer
		textureman.bind(1, Textureman::T_RENDERBUFFER);
		textureman.bind(0, Textureman::T_SUN);
		
		// rendering call
		satteliteVAO.render(GL_QUADS);
		
		return matsun;
	}

	void SkyRenderer::renderMoon()
	{
		//if ogl.lastsun(2) > 0.2 then return
		
		Shader& moon = shaderman[Shaderman::MOON];
		moon.bind();
		
		Matrix matmoon, mattemp;
		
		mattemp.rotateZYX(0.0, -PI / 2, 0.0);
		matmoon.rotateZYX(thesun.getRealtimeRadianAngle() + PI, 0.0, 0.0);
		mattemp *= matmoon;
		mattemp.translated(0.0, 0.0, -2.0);
		
		matmoon = frustum.getRotationMatrix() * mattemp;
		
		// view matrix
		moon.sendMatrix("matview", matmoon);
		// rotation matrix
		moon.sendMatrix("matrot", frustum.getRotationMatrix());
		// daylight level
		moon.sendFloat("daylight", thesun.getRealtimeDaylight());
		
		textureman.bind(0, Textureman::T_MOON);
		
		// rendering call
		satteliteVAO.render(GL_QUADS);
	}
	
}