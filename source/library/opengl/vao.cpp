#include "vao.hpp"

#include "../log.hpp"
#include "opengl.hpp"
#include <cstdint>

namespace library
{
	GLuint VAO::lastVAO = 0;
	
	VAO::VAO()
	{
		vao = vbo = ibo = 0;
		isCreating = false;
	}
	
	// static drawing
	void VAO::begin(GLuint vertexSize, GLsizei vertices, GLvoid* data)
	{
		begin(vertexSize, vertices, data, GL_STATIC_DRAW);
	}
	
	// custom drawing
	void VAO::begin(GLuint vertexSize, GLsizei vertices, GLvoid* data, GLenum usage)
	{
		this->isCreating = true;
		
		// create & bind VAO
		if (vao == 0)
		{
			glGenVertexArrays(1, &vao);
		}
		bind();
		// create & bind VBO
		if (vbo == 0)
		{
			glGenBuffers(1, &vbo);
		}
		glBindBuffer(GL_ARRAY_BUFFER_ARB, vbo);
		
		this->vertexSize = vertexSize;
		this->vertices   = vertices;
		unsigned int totalBytes = vertices * vertexSize;
		
		// upload data
		glBufferData(GL_ARRAY_BUFFER_ARB, totalBytes, data, GL_STATIC_DRAW_ARB);
		
		if (ogl.checkError())
		{
			logger << Log::ERR << "VAO::beginCreate(): OpenGL error for vao = " << vao << ", vbo = " << vbo << Log::ENDL;
			throw std::string("VAO::beginCreate(): OpenGL error");
		}
	}
	
	void VAO::indexes(GLvoid* data, GLsizei count)
	{
		this->indices = count;
		
		if (ibo == 0)
		{
			glGenBuffers(1, &ibo);
		}
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER_ARB, count * sizeof(GLushort), data, GL_STATIC_DRAW_ARB);
		
		if (ogl.checkError())
		{
			logger << Log::ERR << "VAO::indexes(): OpenGL error for vao = " << vao << ", ibo = " << ibo << Log::ENDL;
			throw std::string("VAO::indexes(): OpenGL error");
		}
	}
	
	void VAO::attrib(GLuint index, GLsizei size, GLenum type, bool normalize, int offset)
	{
		glEnableVertexAttribArray(index);
		intptr_t dataPointer = offset; // convert offset to the pointer size of build target
		glVertexAttribPointer(index, size, type, (normalize) ? GL_TRUE : GL_FALSE, vertexSize, (GLvoid*) dataPointer);
		
		if (ogl.checkError())
		{
			logger << Log::ERR << "VAO::attrib(): OpenGL error for vao = " << vao << ", vbo = " << vbo << Log::ENDL;
			logger << Log::ERR << "VAO::attrib(): Attrib = { index = " << index << ", size = " << size << ", type = " << type << ", normalize = " << normalize << ", offset = " << offset << " }" << Log::ENDL;
			throw std::string("VAO::attrib(): OpenGL error");
		}
	}
	
	void VAO::createScreenspaceVAO()
	{
		struct screenvertex_t
		{
			float x, y;
		};
		
		screenvertex_t sv_t[4] = 
		{
			{ 0, 0 }, { 1, 0 }, { 1, 1 }, { 0, 1 }
		};
		
		begin(sizeof(screenvertex_t), 4, sv_t);
		attrib(0, 2, GL_FLOAT, GL_FALSE, 0);
		end();
	}
	
	void VAO::bind()
	{
		if (lastVAO != vao)
		{
			glBindVertexArray(vao);
			lastVAO = vao;
		}
	}
	
	void VAO::unbind()
	{
		glBindVertexArray(0);
		lastVAO = 0;
	}
	
	void VAO::render(GLenum mode, GLint first, GLsizei count)
	{
		bind();
		glDrawArrays(mode, first, count);
		
		if (ogl.checkError())
		{
			logger << Log::ERR << "VAO::render(): OpenGL error for vao = " << vao << Log::ENDL;
			logger << Log::ERR << "VAO::render(): render = { mode = " << mode << ", first = " << first << ", count = " << count << " }" << Log::ENDL;
			throw std::string("VAO::render(): OpenGL error");
		}
	}
	
	void VAO::renderIndexed(GLenum mode, GLuint first, GLint count)
	{
		bind();
		//glDrawRangeElements(mode, first, first + count, count, GL_UNSIGNED_SHORT, nullptr);
		
		if (ogl.checkError())
		{
			logger << Log::ERR << "VAO::renderIndexed(3): OpenGL error for vao = " << vao << Log::ENDL;
			throw std::string("VAO::renderIndexed(3): OpenGL error");
		}
	}
	void VAO::renderIndexed(GLenum mode)
	{
		bind();
		glDrawElements(mode, this->indices, GL_UNSIGNED_SHORT, 0);
		
		if (ogl.checkError())
		{
			logger << Log::ERR << "VAO::renderIndexed(1): OpenGL error for vao = " << vao << Log::ENDL;
			throw std::string("VAO::renderIndexed(1): OpenGL error");
		}
	}
}
