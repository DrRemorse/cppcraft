/**
 * 
 * OpenGL extension function pointers
 * getting entry points using *GetProcAdress()
 * 
**/

#include "opengl.hpp"

namespace library
{
	#define opengl_ext GLapi
	
	// openGL extensions
	opengl_ext void (*glGenerateMipmap)(GLenum target);
	
	// vertex buffer objects
	opengl_ext void (*glGenBuffers)(GLsizei n, GLuint* ids);
	opengl_ext void (*glBindBuffer)(GLenum target, GLuint id);
	opengl_ext void (*glBufferData)(GLenum target, GLint size, GLvoid* data, GLenum usage);
	opengl_ext void (*glBufferSubData)(GLenum target, GLint offset, GLsizei size, GLvoid* data);
	opengl_ext void (*glDeleteBuffers)(GLsizei n, GLuint* ids);
	
	// texturing extensions
	opengl_ext void (*glActiveTexture)(GLenum textureunit);
	opengl_ext void (*glTexImage3D)(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, GLvoid* data);
	opengl_ext void (*glTexImage2DMultisample)(GLenum target, GLsizei samples, GLint internalformat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations);
	
	// shaders
	opengl_ext GLuint (*glCreateProgram)();
	opengl_ext void (*glDeleteProgram)(GLuint program);
	opengl_ext GLuint (*glCreateShader)(GLenum shadertype);
	opengl_ext void (*glDeleteShader)(GLuint shader);
	opengl_ext void (*glCompileShader)(GLuint shader);
	opengl_ext void (*glAttachShader)(GLuint program, GLuint shader);
	opengl_ext void (*glDetachShader)(GLuint program, GLuint shader);
	opengl_ext void (*glShaderSource)(GLuint shader, GLsizei first, GLchar** source_string, GLint* length);
	opengl_ext void (*glGetShaderInfoLog)(GLuint shader, GLsizei maxlen, GLsizei* length, GLchar* source_string);
	opengl_ext void (*glGetShaderiv)(GLuint shader, GLenum flag, GLint* value);
	opengl_ext void (*glGetProgramiv)(GLuint program, GLenum flag, GLint* value);
	opengl_ext void (*glGetProgramInfoLog)(GLuint program, GLsizei maxlen, GLsizei* length, GLchar* source_string);
	opengl_ext void (*glLinkProgram)(GLuint program);
	opengl_ext void (*glUseProgram)(GLuint program);
	// shader uniforms
	opengl_ext GLint (*glGetUniformLocation)(GLuint program, GLchar* variable);
	opengl_ext void (*glUniform1i)(GLint location, GLint value);
	opengl_ext void (*glUniform2i)(GLint location, GLint v1, GLint v2);
	opengl_ext void (*glUniform3i)(GLint location, GLint v1, GLint v2, GLint v3);
	opengl_ext void (*glUniform1f)(GLint location, GLfloat value);
	opengl_ext void (*glUniform2f)(GLint location, GLfloat v1, GLfloat v2);
	opengl_ext void (*glUniform3f)(GLint location, GLfloat v1, GLfloat v2, GLfloat v3);
	opengl_ext void (*glUniform4f)(GLint location, GLfloat v1, GLfloat v2, GLfloat v3, GLfloat v4);
	opengl_ext void (*glUniform3fv)(GLint location, GLsizei instances, GLfloat* first);
	opengl_ext void (*glUniform4fv)(GLint location, GLsizei instances, GLfloat* first);
	
	opengl_ext void (*glUniformMatrix4fv)(GLint location, GLsizei count, GLboolean transpose, GLfloat* first);
	
	// vertex arrays
	opengl_ext void (*glGenVertexArrays)(GLsizei count, GLuint* ids);
	opengl_ext void (*glBindVertexArray)(GLuint target);
	opengl_ext void (*glDeleteVertexArrays)(GLsizei count, GLuint* ids);
	// vertex attrib arrays
	opengl_ext void (*glDisableVertexAttribArray)(GLuint index);
	opengl_ext void (*glEnableVertexAttribArray)(GLuint index);
	opengl_ext void (*glVertexAttribPointer)(GLuint index, GLsizei size, GLenum type, GLboolean normalized, GLsizei stride, GLvoid* dataoffset);
	opengl_ext void (*glBindAttribLocation)(GLuint program, GLuint index, GLchar* attrib);
	
	// occlusion queries
	opengl_ext void (*glGenQueries)(GLsizei count, GLuint* ids);
	opengl_ext void (*glDeleteQueries)(GLsizei count, GLuint* ids);
	opengl_ext void (*glGetQueryObjectuiv)(GLuint id, GLenum query, GLuint* result);
	opengl_ext void (*glBeginQuery)(GLenum mode, GLuint target);
	opengl_ext void (*glEndQuery)(GLenum mode);
	
	// framebuffers
	opengl_ext void (*glGenFramebuffers)(GLsizei count, GLuint* ids);
	opengl_ext void (*glDeleteFramebuffers)(GLsizei count, GLuint* ids);
	opengl_ext void (*glBindFramebuffer)(GLenum bufferType, GLuint fbo);
	opengl_ext void (*glFramebufferTexture2D)(GLenum bufferType, GLenum attachmentType, GLenum textureTarget, GLuint texture, GLint level);
	opengl_ext void (*glFramebufferTexture)(GLenum bufferType, GLenum attachmentType, GLuint texture, GLint level);
	opengl_ext void (*glBlitFramebuffer)(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1,
											  GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLint mask, GLenum filter);
	opengl_ext GLenum (*glCheckFramebufferStatus)(GLenum bufferType);
	opengl_ext void (*glDrawBuffers)(GLsizei count, GLenum* buffers);
	// framebuffer renderbuffers
	opengl_ext void (*glGenRenderbuffers)(GLsizei count, GLuint* rboIDs);
	opengl_ext void (*glBindRenderbuffer)(GLenum bufferType, GLuint rbo);
	opengl_ext void (*glRenderbufferStorage)(GLenum bufferType, GLenum storageType, GLsizei width, GLsizei height);
	opengl_ext void (*glRenderbufferStorageMultisample)(GLenum bufferType, GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height);
	opengl_ext void (*glFramebufferRenderbuffer)(GLenum frameBufferType, GLenum bindType, GLenum renderBufferType, GLuint rbo);
	
}
