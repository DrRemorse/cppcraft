#ifndef VAO_HPP
#define VAO_HPP

typedef unsigned int GLenum;
typedef unsigned int GLuint;
typedef int  GLint;
typedef int  GLsizei;
typedef void GLvoid;

#include <string>

namespace library
{
	class VAO
	{
		GLuint  vao, vbo, ibo;
		GLuint  vertexSize;
		GLsizei vertices;
		GLsizei indices;
		
		static GLuint lastVAO;
		bool   isCreating;
		
	public:
		VAO();
		
		inline bool isGood() const
		{
			return (vao != 0 && vbo != 0 && isCreating == false);
		}
		// getters
		inline GLsizei getVertexCount() const
		{
			return this->vertices;
		}
		inline GLsizei getIndexCount() const
		{
			return this->indices;
		}
		
		// send data to VAO
		void begin(GLuint vertexSize, GLsizei vertices, GLvoid* data);
		void begin(GLuint vertexSize, GLsizei vertices, GLvoid* data, GLenum usage);
		void indexes(GLvoid* data, GLsizei count);
		void attrib(GLuint index, GLsizei size, GLenum type, bool normalize, int offset);
		inline void end()
		{
			this->isCreating = false;
		}
		
		// pre-made VAOs
		void createScreenspaceVAO();
		
		// bind / unbind mesh
		void bind();
		static void unbind();
		
		// bind and render all vertices
		inline void render(GLenum mode)
		{
			render(mode, 0, this->vertices);
		}
		
		// render a part of the mesh
		void render(GLenum mode, GLint first, GLsizei count);
		// render using indexed buffer
		void renderIndexed(GLenum mode);
		void renderIndexed(GLenum mode, GLuint first, GLint count);
		
	};
}
#endif
