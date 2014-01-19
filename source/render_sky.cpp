#include "render_sky.hpp"

#include <library/math/matrix.hpp>
#include <library/opengl/opengl.hpp>
#include <library/opengl/vao.hpp>
#include "atmosphere.hpp"
#include "camera.hpp"
#include "renderconst.hpp"
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
			
			satellite_t() {}
			satellite_t(float X, float Y, float Z) :
				vx(X), vy(Y), vz(Z) {}
		};
		satellite_t mv[4];
		
		mv[0] = satellite_t ( -0.5, -0.5, 0.0 );
		mv[1] = satellite_t (  0.5, -0.5, 0.0 );
		mv[2] = satellite_t (  0.5,  0.5, 0.0 );
		mv[3] = satellite_t ( -0.5,  0.5, 0.0 );
		
		satteliteVAO.begin(sizeof(satellite_t), 4, mv);
		satteliteVAO.attrib(0, 3, GL_FLOAT, GL_FALSE, 0);
		satteliteVAO.end();
	}
	
	// render background / atmosphere, sun and moon
	void SkyRenderer::render(Camera& camera, bool underwater)
	{
		// render sky dome
		Atmosphere::render(camera);
		
		glEnable(GL_BLEND);
		glColorMask(1, 1, 1, 0);
		
		// render regular sun
		if (underwater)
		{
			//renderSun();
		}
		
		// render moon texture
		renderMoon(camera);
		
		glDisable(GL_BLEND);
		glColorMask(1, 1, 1, 1);
	}
	
	void SkyRenderer::renderSun()
	{
		//if ogl.lastsun(1) < -0.15 then return
		
		Shader& sunshader = shaderman[Shaderman::SUN];
		sunshader.bind();
		
		// view matrix
		sunshader.sendMatrix("matview", thesun.getSunMatrix());
		// rotation matrix
		sunshader.sendMatrix("matrot", camera.getRotationMatrix());
		
		textureman.bind(0, Textureman::T_SUN);
		
		// rendering call
		satteliteVAO.render(GL_QUADS);
	}
	
	mat4 SkyRenderer::renderSunProj()
	{
		Shader& sunshader = shaderman[Shaderman::SUNPROJ];
		sunshader.bind();
		
		mat4 matsun = thesun.getSunMatrix();
		
		// view matrix
		sunshader.sendMatrix("matview", matsun);
		// rotation matrix
		sunshader.sendMatrix("matrot", camera.getRotationMatrix());
		
		// depth used as stencil buffer
		textureman.bind(1, Textureman::T_FOGBUFFER);
		textureman.bind(0, Textureman::T_SUN);
		
		// rendering call
		satteliteVAO.render(GL_QUADS);
		
		return matsun;
	}

	void SkyRenderer::renderMoon(Camera& camera)
	{
		//if ogl.lastsun(2) > 0.2 then return
		
		Shader& moon = shaderman[Shaderman::MOON];
		moon.bind();
		
		mat4 mattemp = rotationMatrix(0.0, -PI / 2, 0.0);
		mat4 matmoon = rotationMatrix(thesun.getRealtimeRadianAngle() + PI, 0.0, 0.0);
		mattemp *= matmoon;
		mattemp.translate(0.0, 0.0, -2.0);
		
		matmoon = camera.getRotationMatrix() * mattemp;
		
		// view matrix
		moon.sendMatrix("matview", matmoon);
		// rotation matrix
		moon.sendMatrix("matrot", camera.getRotationMatrix());
		// daylight level
		moon.sendFloat("daylight", thesun.getRealtimeDaylight());
		
		textureman.bind(0, Textureman::T_MOON);
		
		// rendering call
		satteliteVAO.render(GL_QUADS);
	}
	
}
