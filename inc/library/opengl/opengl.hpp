/**
 * OpenGL state & resource manager
**/

#ifndef OPENGL_HPP
#define OPENGL_HPP

#include <GL/glfw3.h>
#include <GL/glext.h>

namespace library
{
	class WindowClass;
	
	class OpenGL
	{
	public:
		OpenGL();
		
		GLint imageformat;
		GLint storageformat;
		
		bool supportsVBO;
		bool supportsVAO;
		
		bool supportsFramebuffers;
		bool supportsShaders;
		bool supportsAttribs;
		
		bool supportsGenMipmap;
		bool supportsTextureArrays;
		
		void init();
		
		static bool checkError();
		
	};
	extern OpenGL ogl;

// texture constants & rendering hints
#define GL_MIRRORED_REPEAT 0x8370
#define GL_TEXTURE_3D 0x806F
#define GL_TEXTURE_2D_ARRAY 0x8C1A
#define GL_DEPTH_TEXTURE_MODE 0x884B
#define GL_TEXTURE_COMPARE_FUNC 0x884D
#define GL_TEXTURE_COMPARE_MODE 0x884C
#define GL_COMPARE_R_TO_TEXTURE 0x884E
#define GL_TEXTURE_COMPRESSION_HINT 0x84EF
#define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE
#define GL_GENERATE_MIPMAP 0x8191
#define GL_GENERATE_MIPMAP_HINT 0x8192
#define GL_MULTISAMPLE_ARB 0x809D
#define GL_TEXTURE_CUBE_MAP_SEAMLESS 0x884F

// framebuffer formats
#define GL_RGBA32F_ARB 0x8814
#define GL_RGB32F_ARB 0x8815
#define GL_ALPHA32F_ARB 0x8816
#define GL_INTENSITY32F_ARB 0x8817
#define GL_LUMINANCE32F_ARB 0x8818
#define GL_LUMINANCE_ALPHA32F_ARB 0x8819
#define GL_RGBA16F_ARB 0x881A
#define GL_RGB16F_ARB 0x881B
#define GL_ALPHA16F_ARB 0x881C
#define GL_INTENSITY16F_ARB 0x881D
#define GL_LUMINANCE16F_ARB 0x881E
#define GL_LUMINANCE_ALPHA16F_ARB 0x881F

// shader constants
#define GL_FRAGMENT_PROGRAM_ARB 0x8804
#define GL_PROGRAM_ALU_INSTRUCTIONS_ARB 0x8805
#define GL_PROGRAM_TEX_INSTRUCTIONS_ARB 0x8806
#define GL_PROGRAM_TEX_INDIRECTIONS_ARB 0x8807
#define GL_PROGRAM_NATIVE_ALU_INSTRUCTIONS_ARB 0x8808
#define GL_PROGRAM_NATIVE_TEX_INSTRUCTIONS_ARB 0x8809
#define GL_PROGRAM_NATIVE_TEX_INDIRECTIONS_ARB 0x880A
#define GL_MAX_PROGRAM_ALU_INSTRUCTIONS_ARB 0x880B
#define GL_MAX_PROGRAM_TEX_INSTRUCTIONS_ARB 0x880C
#define GL_MAX_PROGRAM_TEX_INDIRECTIONS_ARB 0x880D
#define GL_MAX_PROGRAM_NATIVE_ALU_INSTRUCTIONS_ARB 0x880E
#define GL_MAX_PROGRAM_NATIVE_TEX_INSTRUCTIONS_ARB 0x880F
#define GL_MAX_PROGRAM_NATIVE_TEX_INDIRECTIONS_ARB 0x8810
#define GL_MAX_TEXTURE_COORDS_ARB 0x8871
#define GL_MAX_TEXTURE_IMAGE_UNITS_ARB 0x8872
#define GL_PROGRAM_OBJECT_ARB 0x8B40
#define GL_SHADER_OBJECT_ARB 0x8B48
#define GL_OBJECT_TYPE_ARB 0x8B4E
#define GL_OBJECT_SUBTYPE_ARB 0x8B4F
#define GL_FLOAT_VEC2_ARB 0x8B50
#define GL_FLOAT_VEC3_ARB 0x8B51
#define GL_FLOAT_VEC4_ARB 0x8B52
#define GL_INT_VEC2_ARB 0x8B53
#define GL_INT_VEC3_ARB 0x8B54
#define GL_INT_VEC4_ARB 0x8B55
#define GL_BOOL_ARB 0x8B56
#define GL_BOOL_VEC2_ARB 0x8B57
#define GL_BOOL_VEC3_ARB 0x8B58
#define GL_BOOL_VEC4_ARB 0x8B59
#define GL_FLOAT_MAT2_ARB 0x8B5A
#define GL_FLOAT_MAT3_ARB 0x8B5B
#define GL_FLOAT_MAT4_ARB 0x8B5C
#define GL_SAMPLER_1D_ARB 0x8B5D
#define GL_SAMPLER_2D_ARB 0x8B5E
#define GL_SAMPLER_3D_ARB 0x8B5F
#define GL_SAMPLER_CUBE_ARB 0x8B60
#define GL_SAMPLER_1D_SHADOW_ARB 0x8B61
#define GL_SAMPLER_2D_SHADOW_ARB 0x8B62
#define GL_SAMPLER_2D_RECT_ARB 0x8B63
#define GL_SAMPLER_2D_RECT_SHADOW_ARB 0x8B64
#define GL_OBJECT_DELETE_STATUS_ARB 0x8B80
#define GL_OBJECT_COMPILE_STATUS_ARB 0x8B81
#define GL_OBJECT_LINK_STATUS_ARB 0x8B82
#define GL_OBJECT_VALIDATE_STATUS_ARB 0x8B83
#define GL_OBJECT_INFO_LOG_LENGTH_ARB 0x8B84
#define GL_OBJECT_ATTACHED_OBJECTS_ARB 0x8B85
#define GL_OBJECT_ACTIVE_UNIFORMS_ARB 0x8B86
#define GL_OBJECT_ACTIVE_UNIFORM_MAX_LENGTH_ARB 0x8B87
#define GL_OBJECT_SHADER_SOURCE_LENGTH_ARB 0x8B88
#define GL_VERTEX_SHADER_ARB 0x8B31
#define GL_MAX_VERTEX_UNIFORM_COMPONENTS_ARB 0x8B4A
#define GL_MAX_VARYING_FLOATS_ARB 0x8B4B
#define GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS_ARB 0x8B4C
#define GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS_ARB 0x8B4D
#define GL_OBJECT_ACTIVE_ATTRIBUTES_ARB 0x8B89
#define GL_OBJECT_ACTIVE_ATTRIBUTE_MAX_LENGTH_ARB 0x8B8A
#define GL_FRAGMENT_SHADER_ARB 0x8B30
#define GL_MAX_FRAGMENT_UNIFORM_COMPONENTS_ARB 0x8B49
#define GL_FRAGMENT_SHADER_DERIVATIVE_HINT_ARB 0x8B8B
#define GL_SHADING_LANGUAGE_VERSION_ARB 0x8B8C
#define GL_COMPILE_STATUS 0x8B81
#define GL_LINK_STATUS 0x8B82
#define GL_VALIDATE_STATUS 0x8B83
#define GL_INFO_LOG_LENGTH 0x8B84
// vertex buffer objects
#define GL_BUFFER_SIZE_ARB 0x8764
#define GL_BUFFER_USAGE_ARB 0x8765
#define GL_ARRAY_BUFFER_ARB 0x8892
#define GL_ELEMENT_ARRAY_BUFFER_ARB 0x8893
#define GL_ARRAY_BUFFER_BINDING_ARB 0x8894
#define GL_ELEMENT_ARRAY_BUFFER_BINDING_ARB 0x8895
#define GL_VERTEX_ARRAY_BUFFER_BINDING_ARB 0x8896
#define GL_NORMAL_ARRAY_BUFFER_BINDING_ARB 0x8897
#define GL_COLOR_ARRAY_BUFFER_BINDING_ARB 0x8898
#define GL_INDEX_ARRAY_BUFFER_BINDING_ARB 0x8899
#define GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING_ARB 0x889A
#define GL_EDGE_FLAG_ARRAY_BUFFER_BINDING_ARB 0x889B
#define GL_SECONDARY_COLOR_ARRAY_BUFFER_BINDING_ARB 0x889C
#define GL_FOG_COORDINATE_ARRAY_BUFFER_BINDING_ARB 0x889D
#define GL_WEIGHT_ARRAY_BUFFER_BINDING_ARB 0x889E
#define GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING_ARB 0x889F
#define GL_READ_ONLY_ARB 0x88B8
#define GL_WRITE_ONLY_ARB 0x88B9
#define GL_READ_WRITE_ARB 0x88BA
#define GL_BUFFER_ACCESS_ARB 0x88BB
#define GL_BUFFER_MAPPED_ARB 0x88BC
#define GL_BUFFER_MAP_POINTER_ARB 0x88BD
#define GL_STREAM_DRAW_ARB 0x88E0
#define GL_STREAM_READ_ARB 0x88E1
#define GL_STREAM_COPY_ARB 0x88E2
#define GL_STATIC_DRAW_ARB 0x88E4
#define GL_STATIC_READ_ARB 0x88E5
#define GL_STATIC_COPY_ARB 0x88E6
#define GL_DYNAMIC_DRAW_ARB 0x88E8
#define GL_DYNAMIC_READ_ARB 0x88E9
#define GL_DYNAMIC_COPY_ARB 0x88EA

// query objects/occlusion
#define GL_QUERY_RESULT 0x8866
#define GL_QUERY_RESULT_AVAILABLE 0x8867
#define GL_SAMPLES_PASSED 0x8914
#define GL_ANY_SAMPLES_PASSED 0x8C2F
#define GL_ANY_SAMPLES_PASSED_CONSERVATIVE 0x8D6A

// framebuffers
#define GL_FRAMEBUFFER_ATTACHMENT_LAYERED 0x8DA7
#define GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS 0x8DA8
#define GL_INVALID_FRAMEBUFFER_OPERATION  0x0506
#define GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING 0x8210
#define GL_FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE 0x8211
#define GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE 0x8212
#define GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE 0x8213
#define GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE 0x8214
#define GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE 0x8215
#define GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE 0x8216
#define GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE 0x8217
#define GL_FRAMEBUFFER_DEFAULT            0x8218
#define GL_FRAMEBUFFER_UNDEFINED          0x8219
#define GL_DEPTH_STENCIL_ATTACHMENT       0x821A
#define GL_MAX_RENDERBUFFER_SIZE          0x84E8
#define GL_DEPTH_STENCIL                  0x84F9
#define GL_UNSIGNED_INT_24_8              0x84FA
#define GL_DEPTH24_STENCIL8               0x88F0
#define GL_TEXTURE_STENCIL_SIZE           0x88F1
#define GL_TEXTURE_RED_TYPE               0x8C10
#define GL_TEXTURE_GREEN_TYPE             0x8C11
#define GL_TEXTURE_BLUE_TYPE              0x8C12
#define GL_TEXTURE_ALPHA_TYPE             0x8C13
#define GL_TEXTURE_DEPTH_TYPE             0x8C16
#define GL_UNSIGNED_NORMALIZED            0x8C17
#define GL_FRAMEBUFFER_BINDING            0x8CA6
//#define GL_DRAW_FRAMEBUFFER_BINDING       GL_FRAMEBUFFER_BINDING
#define GL_RENDERBUFFER_BINDING           0x8CA7
#define GL_READ_FRAMEBUFFER               0x8CA8
#define GL_DRAW_FRAMEBUFFER               0x8CA9
#define GL_READ_FRAMEBUFFER_BINDING       0x8CAA
#define GL_RENDERBUFFER_SAMPLES           0x8CAB
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE 0x8CD0
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME 0x8CD1
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL 0x8CD2
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE 0x8CD3
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER 0x8CD4
#define GL_FRAMEBUFFER_COMPLETE           0x8CD5
#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT 0x8CD6
#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT 0x8CD7
#define GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER 0x8CDB
#define GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER 0x8CDC
#define GL_FRAMEBUFFER_UNSUPPORTED        0x8CDD
#define GL_MAX_COLOR_ATTACHMENTS          0x8CDF
#define GL_COLOR_ATTACHMENT0              0x8CE0
#define GL_COLOR_ATTACHMENT1              0x8CE1
#define GL_COLOR_ATTACHMENT2              0x8CE2
#define GL_COLOR_ATTACHMENT3              0x8CE3
#define GL_COLOR_ATTACHMENT4              0x8CE4
#define GL_COLOR_ATTACHMENT5              0x8CE5
#define GL_COLOR_ATTACHMENT6              0x8CE6
#define GL_COLOR_ATTACHMENT7              0x8CE7
#define GL_COLOR_ATTACHMENT8              0x8CE8
#define GL_COLOR_ATTACHMENT9              0x8CE9
#define GL_COLOR_ATTACHMENT10             0x8CEA
#define GL_COLOR_ATTACHMENT11             0x8CEB
#define GL_COLOR_ATTACHMENT12             0x8CEC
#define GL_COLOR_ATTACHMENT13             0x8CED
#define GL_COLOR_ATTACHMENT14             0x8CEE
#define GL_COLOR_ATTACHMENT15             0x8CEF
#define GL_DEPTH_ATTACHMENT               0x8D00
#define GL_STENCIL_ATTACHMENT             0x8D20
#define GL_FRAMEBUFFER                    0x8D40
#define GL_RENDERBUFFER                   0x8D41
#define GL_RENDERBUFFER_WIDTH             0x8D42
#define GL_RENDERBUFFER_HEIGHT            0x8D43
#define GL_RENDERBUFFER_INTERNAL_FORMAT   0x8D44
#define GL_STENCIL_INDEX1                 0x8D46
#define GL_STENCIL_INDEX4                 0x8D47
#define GL_STENCIL_INDEX8                 0x8D48
#define GL_STENCIL_INDEX16                0x8D49
#define GL_RENDERBUFFER_RED_SIZE          0x8D50
#define GL_RENDERBUFFER_GREEN_SIZE        0x8D51
#define GL_RENDERBUFFER_BLUE_SIZE         0x8D52
#define GL_RENDERBUFFER_ALPHA_SIZE        0x8D53
#define GL_RENDERBUFFER_DEPTH_SIZE        0x8D54
#define GL_RENDERBUFFER_STENCIL_SIZE      0x8D55
#define GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE 0x8D56
#define GL_MAX_SAMPLES                    0x8D57
#define GL_INDEX                          0x8222
#define GL_TEXTURE_LUMINANCE_TYPE         0x8C14
#define GL_TEXTURE_INTENSITY_TYPE         0x8C15
// framebuffer SRGB colorspace
#define GL_FRAMEBUFFER_SRGB               0x8DB9

// depth precision
#define GL_DEPTH_COMPONENT16 0x81A5
#define GL_DEPTH_COMPONENT24 0x81A6
#define GL_DEPTH_COMPONENT32 0x81A7

// point sprites
#define GL_POINT_SPRITE                   0x8861
#define GL_COORD_REPLACE                  0x8862
#define GL_POINT_SIZE_MIN_ARB             0x8126
#define GL_POINT_SIZE_MAX_ARB             0x8127
#define GL_POINT_FADE_THRESHOLD_SIZE_ARB  0x8128
#define GL_POINT_DISTANCE_ATTENUATION_ARB 0x8129
#define GL_VERTEX_PROGRAM_POINT_SIZE_ARB  0x8642

#ifdef WIN32
#define GLapi __stdcall
#else
#define GLapi
#endif

typedef char GLchar;

#define opengl_ext_decl extern GLapi

// openGL extensions
opengl_ext_decl void (*glGenerateMipmap)(GLenum target);

// vertex buffer objects
opengl_ext_decl void (*glGenBuffers)(GLsizei n, GLuint* ids);
opengl_ext_decl void (*glBindBuffer)(GLenum target, GLuint id);
opengl_ext_decl void (*glBufferData)(GLenum target, GLint size, GLvoid* data, GLenum usage);
opengl_ext_decl void (*glBufferSubData)(GLenum target, GLint offset, GLsizei size, GLvoid* data);
opengl_ext_decl void (*glDeleteBuffers)(GLsizei n, GLuint* ids);

opengl_ext_decl void (*glActiveTexture)(GLenum textureunit);
opengl_ext_decl void (*glTexImage3D)(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, GLvoid* data);
opengl_ext_decl void (*glTexImage2DMultisample)(GLenum target, GLsizei samples, GLint internalformat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations);

// shaders
opengl_ext_decl GLuint (*glCreateProgram)();
opengl_ext_decl void (*glDeleteProgram)(GLuint program);
opengl_ext_decl GLuint (*glCreateShader)(GLenum shadertype);
opengl_ext_decl void (*glDeleteShader)(GLuint shader);
opengl_ext_decl void (*glCompileShader)(GLuint shader);
opengl_ext_decl void (*glAttachShader)(GLuint program, GLuint shader);
opengl_ext_decl void (*glDetachShader)(GLuint program, GLuint shader);
opengl_ext_decl void (*glShaderSource)(GLuint shader, GLsizei first, GLchar** source_string, GLint* length);
opengl_ext_decl void (*glGetShaderInfoLog)(GLuint shader, GLsizei maxlen, GLsizei* length, GLchar* source_string);
opengl_ext_decl void (*glGetShaderiv)(GLuint shader, GLenum flag, GLint* value);
opengl_ext_decl void (*glLinkProgram)(GLuint program);
opengl_ext_decl void (*glGetProgramiv)(GLuint program, GLenum flag, GLint* value);
opengl_ext_decl void (*glGetProgramInfoLog)(GLuint program, GLsizei maxlen, GLsizei* length, GLchar* source_string);
opengl_ext_decl void (*glUseProgram)(GLuint program);
// shader uniforms
opengl_ext_decl GLint (*glGetUniformLocation)(GLuint program, GLchar* variable);
opengl_ext_decl void (*glUniform1i)(GLint location, GLint value);
opengl_ext_decl void (*glUniform2i)(GLint location, GLint v1, GLint v2);
opengl_ext_decl void (*glUniform3i)(GLint location, GLint v1, GLint v2, GLint v3);
opengl_ext_decl void (*glUniform1f)(GLint location, GLfloat value);
opengl_ext_decl void (*glUniform2f)(GLint location, GLfloat v1, GLfloat v2);
opengl_ext_decl void (*glUniform3f)(GLint location, GLfloat v1, GLfloat v2, GLfloat v3);
opengl_ext_decl void (*glUniform4f)(GLint location, GLfloat v1, GLfloat v2, GLfloat v3, GLfloat v4);
opengl_ext_decl void (*glUniform3fv)(GLint location, GLsizei instances, GLfloat* first);
opengl_ext_decl void (*glUniform4fv)(GLint location, GLsizei instances, GLfloat* first);

opengl_ext_decl void (*glUniformMatrix4fv)(GLint location, GLsizei count, GLboolean transpose, GLfloat* first);

// vertex arrays
opengl_ext_decl void (*glGenVertexArrays)(GLsizei count, GLuint* ids);
opengl_ext_decl void (*glBindVertexArray)(GLuint target);
opengl_ext_decl void (*glDeleteVertexArrays)(GLsizei count, GLuint* ids);
// vertex attrib arrays
opengl_ext_decl void (*glDisableVertexAttribArray)(GLuint index);
opengl_ext_decl void (*glEnableVertexAttribArray)(GLuint index);
opengl_ext_decl void (*glVertexAttribPointer)(GLuint index, GLsizei size, GLenum type, GLboolean normalized, GLsizei stride, GLvoid* dataoffset);
opengl_ext_decl void (*glBindAttribLocation)(GLuint program, GLuint index, GLchar* attrib);
// occlusion queries
opengl_ext_decl void (*glGenQueries)(GLsizei count, GLuint* ids);
opengl_ext_decl void (*glDeleteQueries)(GLsizei count, GLuint* ids);
opengl_ext_decl void (*glGetQueryObjectuiv)(GLuint id, GLenum query, GLuint* result);
opengl_ext_decl void (*glBeginQuery)(GLenum mode, GLuint target);
opengl_ext_decl void (*glEndQuery)(GLenum mode);
// framebuffers
opengl_ext_decl void (*glGenFramebuffers)(GLsizei count, GLuint* ids);
opengl_ext_decl void (*glDeleteFramebuffers)(GLsizei count, GLuint* ids);
opengl_ext_decl void (*glBindFramebuffer)(GLenum bufferType, GLuint fbo);
opengl_ext_decl void (*glFramebufferTexture2D)(GLenum bufferType, GLenum attachmentType, GLenum textureTarget, GLuint texture, GLint level);
opengl_ext_decl void (*glFramebufferTexture)(GLenum bufferType, GLenum attachmentType, GLuint texture, GLint level);
opengl_ext_decl void (*glBlitFramebuffer)(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1,
										  GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLint mask, GLenum filter);
opengl_ext_decl GLenum (*glCheckFramebufferStatus)(GLenum bufferType);
opengl_ext_decl void (*glDrawBuffers)(GLsizei count, GLenum* buffers);
// framebuffer renderbuffers
opengl_ext_decl void (*glGenRenderbuffers)(GLsizei count, GLuint* rboIDs);
opengl_ext_decl void (*glBindRenderbuffer)(GLenum bufferType, GLuint rbo);
opengl_ext_decl void (*glRenderbufferStorage)(GLenum bufferType, GLenum storageType, GLsizei width, GLsizei height);
opengl_ext_decl void (*glRenderbufferStorageMultisample)(GLenum bufferType, GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height);
opengl_ext_decl void (*glFramebufferRenderbuffer)(GLenum frameBufferType, GLenum bindType, GLenum renderBufferType, GLuint rbo);

}

#endif
