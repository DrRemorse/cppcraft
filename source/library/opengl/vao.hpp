#ifndef VAO_HPP
#define VAO_HPP

typedef unsigned int GLenum;
typedef unsigned int GLuint;
typedef int  GLint;
typedef int  GLsizei;
typedef void GLvoid;

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
		
		bool isGood() const;
		
		// create VAO
		void begin(GLuint vertexSize, GLsizei vertices, GLvoid* data);
		void attrib(GLuint index, GLsizei size, GLenum type, bool normalize, int offset);
		void end();
		
		// bind / unbind mesh
		void bind();
		void unbind();
		
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
