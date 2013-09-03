#include "particles.hpp"

#include "library/log.hpp"
#include "library/opengl/opengl.hpp"
#include "camera.hpp"
#include "shaderman.hpp"
#include "textureman.hpp"
#include "threading.hpp"
#include "sector.hpp"
#include "sun.hpp"
#include "world.hpp"

using namespace library;

namespace cppcraft
{
	void Particles::renderUpdate()
	{
		mtx.particles.lock();
		
		// set snapshots
		snapRenderCount = renderCount;
		
		if (snapRenderCount == 0)
		{
			mtx.particles.unlock();
			return;
		}
		snapWX = currentWX;
		snapWZ = currentWZ;
		
		if (particleSystem.updated)
		{
			particleSystem.updated = false;
			
			if (vao == 0)
			{
				glGenVertexArrays(1, &vao);
				glGenBuffers(1, &vbo);
			}
			glBindVertexArray(vao);
			
			// particles vbo
			glBindBuffer(GL_ARRAY_BUFFER_ARB, vbo);
			glBufferData(GL_ARRAY_BUFFER_ARB, renderCount * sizeof(particle_vertex_t), vertices, GL_STREAM_DRAW_ARB);
			
			for (int i = 0; i < 4; i++)
				glEnableVertexAttribArray(i);
			
			// position
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(particle_vertex_t), 0);
			// data
			glVertexAttribPointer(1, 2, GL_SHORT, GL_FALSE, sizeof(particle_vertex_t), (GLvoid*) 12);
			// normalized data
			glVertexAttribPointer(2, 2, GL_SHORT, GL_TRUE, sizeof(particle_vertex_t),  (GLvoid*) 16);
			// color
			glVertexAttribPointer(3, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(particle_vertex_t), (GLvoid*) 20);
			
			glBindVertexArray(0);
			
			for (int i = 0; i < 4; i++)
				glDisableVertexAttribArray(i);
		}
		mtx.particles.unlock();
	}
	
	void Particles::render(int snapWX, int snapWZ)
	{
		// start particles shader
		// BEFORE we potentially exit because there are no particles to render
		// this is done to get the first-frame update.. considering there's
		// always particles to render, we'll just go ahead and always bind the shader
		
		Shader& shd = shaderman[Shaderman::PARTICLE];
		shd.bind();
		shd.sendFloat("daylight", thesun.getRealtimeDaylight());
		
		if (camera.ref)
		{
			float tx = (this->snapWX - snapWX) * Sector::BLOCKS_XZ;
			float tz = (this->snapWZ - snapWZ) * Sector::BLOCKS_XZ;
			
			Matrix matview = camera.getViewMatrix();
			matview.translate_xz(tx, tz);
			
			// send view matrix
			shd.sendMatrix("matview", matview);
		}
		
		if (vao == 0 || snapRenderCount == 0) return;
		
		glEnable(GL_POINT_SPRITE);
		
		// bind 2d array of textures/tiles
		textureman.bind(0, Textureman::T_PARTICLES);
		
		// bind and render particles
		glBindVertexArray(vao);
		glDrawArrays(GL_POINTS, 0, snapRenderCount);
		
		glDisable(GL_POINT_SPRITE);
	}
	
}