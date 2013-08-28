#include "vao.hpp"

#include "../log.hpp"
#include "opengl.hpp"
#include <cstdint>

namespace library
{
	GLuint VAO::lastVAO = 0;
	
	VAO::VAO()
	{
		vao = 0;
		vbo = 0;
		isCreating = false;
	}
	
	bool VAO::isGood() const
	{
		return (vao != 0 && vbo != 0 && isCreating == false);
	}
	
	void VAO::begin(GLuint vertexSize, GLsizei vertices, GLvoid* data)
	{
		this->isCreating = true;
		
		// create & bind VAO
		if (vao == 0) glGenVertexArrays(1, &vao);
		bind();
		// create & bind VBO
		if (vbo == 0) glGenBuffers(1, &vbo);
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
	
	void VAO::attrib(GLuint index, GLsizei size, GLenum type, bool normalize, int offset)
	{
		glEnableVertexAttribArray(index);
		intptr_t dataPointer = offset; // convert offset to the pointer size of this build target
		glVertexAttribPointer(index, size, type, (normalize) ? GL_TRUE : GL_FALSE, vertexSize, (GLvoid*) dataPointer);
		
		if (ogl.checkError())
		{
			logger << Log::ERR << "VAO::attrib(): OpenGL error for vao = " << vao << ", vbo = " << vbo << Log::ENDL;
			logger << Log::ERR << "VAO::attrib(): Attrib = { index = " << index << ", size = " << size << ", type = " << type << ", normalize = " << normalize << ", offset = " << offset << " }" << Log::ENDL;
			throw std::string("VAO::attrib(): OpenGL error");
		}
	}
	
	void VAO::end()
	{
		this->isCreating = false;
		
		// disable vao & vbo
		unbind();
	}
	
	void VAO::bind()
	{
		glBindVertexArray(vao);
		lastVAO = vao;
	}
	
	void VAO::unbind()
	{
		glBindVertexArray(0);
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
	void VAO::render(GLenum mode)
	{
		render(mode, 0, this->vertices);
	}
	
}