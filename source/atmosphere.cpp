#include "atmosphere.hpp"

#include "library/config.hpp"
#include "library/opengl/opengl.hpp"
#include "library/opengl/vao.hpp"
#include "camera.hpp"
#include "lighting.hpp"
#include "player.hpp"
#include "sun.hpp"
#include "shaderman.hpp"
#include "renderman.hpp"
#include "textureman.hpp"
#include <cmath>

using namespace library;

namespace cppcraft
{
	class Scatterer
	{
	public:
		VAO vao;
		
		float m_nSamples;
		float m_Kr, m_Kr4PI;
		float m_Km, m_Km4PI;
		float m_ESun, m_g;
		
		float m_fInnerRadius;
		float m_fOuterRadius;
		float m_fScale;
		
		float m_fRayleighScaleDepth;
		float m_fMieScaleDepth;
		float m_fWavelength[3];
		float m_fWavelength4[3];
		
		void init();
		void render(float playerY);
	};
	Scatterer scatter;
	
	const float PI = 4 * atan(1);
	
	void createSkyDome(float r_inner, float r_outer);
	
	// entry functions into this closed module
	void Atmosphere::init()
	{
		scatter.init();
	}
	void Atmosphere::render(float playerY)
	{
		scatter.render(playerY);
	}
	
	// scatterer implementation
	
	void Scatterer::init()
	{
		m_nSamples = 3;		// Number of sample rays to use in integral equation
		m_Kr = 0.0025;		// Rayleigh scattering constant
		m_Kr4PI = m_Kr * 4.0 * PI;
		m_Km = 0.0025;		// Mie scattering constant
		m_Km4PI = m_Km * 4.0 * PI;
		m_ESun = 13.0;		// Sun brightness constant
		m_g = -0.50;		// The Mie phase asymmetry factor
		
		m_fInnerRadius = 10.0; // 9.99375
		m_fOuterRadius = 10.25;
		m_fScale = 1.0 / (m_fOuterRadius - m_fInnerRadius);
		
		m_fRayleighScaleDepth = 0.25;
		m_fMieScaleDepth = 0.1;
		
		m_fWavelength[0] = 0.650;	// 650 nm for red
		m_fWavelength[1] = 0.570;	// 570 nm for green
		m_fWavelength[2] = 0.475;	// 475 nm for blue
		m_fWavelength4[0] = powf(m_fWavelength[0], 4.0f);
		m_fWavelength4[1] = powf(m_fWavelength[1], 4.0f);
		m_fWavelength4[2] = powf(m_fWavelength[2], 4.0f);
		
		// create the skydome algorithmically
		createSkyDome(m_fInnerRadius, m_fOuterRadius);
		
		Shader& shd = shaderman[Shaderman::ATMOSPHERE];
		shd.bind();
		
		GLuint prog = shd.getShader();
		GLint location;
		
		#define setu1(uname, udata) \
			\
			location = glGetUniformLocation(prog, uname); \
			if (location+1) \
				glUniform1f(location, udata)
		
		#define setu3(uname, udata1, udata2, udata3) \
			\
			location = glGetUniformLocation(prog, uname); \
			if (location+1) \
				glUniform3f(location, udata1, udata2, udata3)
		
		
		setu3("v3InvWavelength", 1.0 / m_fWavelength4[0], 
								 1.0 / m_fWavelength4[1], 
								 1.0 / m_fWavelength4[2]);
		setu1("fInnerRadius",  m_fInnerRadius);
		setu1("fOuterRadius",  m_fOuterRadius);
		setu1("fKrESun", m_Kr * m_ESun);
		setu1("fKmESun", m_Km * m_ESun);
		setu1("fKr4PI",  m_Kr4PI);
		setu1("fKm4PI",  m_Km4PI);
		setu1("fScale",  m_fScale);
		setu1("fScaleDepth", m_fRayleighScaleDepth);
		setu1("fScaleOverScaleDepth", m_fScale / m_fRayleighScaleDepth);
		setu1("g",  m_g);
		setu1("g2", m_g * m_g);
		
		shd.sendInteger("texture", 0);
		shd.sendInteger("stars",   1);
		
		shd.sendVec3("v3CameraPos", vec3(0.0, m_fInnerRadius, 0.0));
		shd.sendFloat("fCameraHeight", m_fInnerRadius);
	}

	void Scatterer::render(float playerY)
	{
		Shader& shd = shaderman[Shaderman::ATMOSPHERE];
		shd.bind();
		
		shd.sendVec3("v3LightPos", thesun.getRealtimeAngle());
		shd.sendFloat("sunAngle", thesun.getRealtimeRadianAngle());
		
		// bind textures
		textureman.bind(1, Textureman::T_STARS);
		textureman.bind(0, Textureman::T_SKYBOX);
		
		// create view matrix
		Matrix matview = camera.getRotationMatrix();
		matview.translated(0.0, -m_fInnerRadius, 0.0);
		shd.sendMatrix("matview", matview);
		shd.sendFloat("above", 1.0);
		
		// render
		scatter.vao.render(GL_TRIANGLES);
		
		matview = camera.getRotationMatrix();
		// multiply with negative-Y scaling matrix
		matview *= Matrix(1.0, -0.8, 1.0);
		matview.translated(0.0, -m_fInnerRadius, 0.0);
		
		shd.sendMatrix("matview", matview);
		shd.sendFloat("above", 0.0);
		
		// render mirrored on y-axis
		scatter.vao.render(GL_TRIANGLES);
	}

	void createSkyDome(float r_inner, float r_outer)
	{
		struct domevertex_t
		{
			float x, y, z;
		};
		
		const int numX = 16;
		const int numY = 4;
		
		const int num_vertices = (2 * numY + 1) * numX * 3; // triangles * 3
		
		// total number of vertices allocated to buffer:
		domevertex_t* vertices = new domevertex_t[num_vertices];
		// vertices for triangle strip:
		domevertex_t va, vb, vc, vd;
		
		float stepa  = PI * 2.0f / numX;
		float startb = asin(r_inner / r_outer), stepb = (PI / 2.0f - startb) / 4.0f;
		
		int p = 0;
		
		for (int y = 0; y < numY; y++)
		{
			float b = startb + stepb * y;
			
			for (int x = 0; x < numX; x++)
			{
				float a = stepa * x;
				
				va = (domevertex_t) { sinf(a)         * cosf(b) * r_outer,         sinf(b) * r_outer,         -cosf(a)         * cosf(b) * r_outer };
				vb = (domevertex_t) { sinf(a + stepa) * cosf(b) * r_outer,         sinf(b) * r_outer,         -cosf(a + stepa) * cosf(b) * r_outer };
				vc = (domevertex_t) { sinf(a + stepa) * cosf(b + stepb) * r_outer, sinf(b + stepb) * r_outer, -cosf(a + stepa) * cosf(b + stepb) * r_outer };
				vd = (domevertex_t) { sinf(a)         * cosf(b + stepb) * r_outer, sinf(b + stepb) * r_outer, -cosf(a)         * cosf(b + stepb) * r_outer };
				
				vertices[p + 0] = va;
				vertices[p + 1] = vb;
				vertices[p + 2] = vc;
				p += 3;
				
				vertices[p + 0] = vc;
				vertices[p + 1] = vd;
				vertices[p + 2] = va;
				p += 3;
			}
		}
		
		float b = startb + stepb * 3;
		
		for (int x = 0; x < numX; x++)
		{
			float a = stepa * x;
			
			va = (domevertex_t) { sinf(a)         * cosf(b) * r_outer, sinf(b) * r_outer, -cosf(a)         * cosf(b) * r_outer };
			vb = (domevertex_t) { sinf(a + stepa) * cosf(b) * r_outer, sinf(b) * r_outer, -cosf(a + stepa) * cosf(b) * r_outer };
			vc = (domevertex_t) { 0.0f, r_outer, 0.0f };
			
			vertices[p + 0] = va;
			vertices[p + 1] = vb;
			vertices[p + 2] = vc;
			p += 3;
		}
		
		scatter.vao.begin(sizeof(domevertex_t), num_vertices, vertices);
		scatter.vao.attrib(0, 3, GL_FLOAT, GL_FALSE, 0);
		scatter.vao.end();
		scatter.vao.unbind();
		// cleanup
		delete vertices;
	}
}
