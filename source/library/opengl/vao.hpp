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
		GLuint  vao, vbo;
		GLuint  vertexSize;
		GLsizei vertices;
		
		static GLuint lastVAO;
		bool   isCreating;
		
	public:
		VAO();
		
		inline bool isGood() const
		{
			return (vao != 0 && vbo != 0 && isCreating == false);
		}
		
		// send data to VAO
		void begin(GLuint vertexSize, GLsizei vertices, GLvoid* data);
		void begin(GLuint vertexSize, GLsizei vertices, GLvoid* data, GLenum usage);
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
		void render(GLenum mode);
		// render a part of the mesh
		void render(GLenum mode, GLint first, GLsizei count);
		
		// getters
		inline GLsizei getVertices() const
		{
			return this->vertices;
		}
	};
}
#endif
