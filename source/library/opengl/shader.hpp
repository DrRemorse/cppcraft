#ifndef SHADER_HPP
#define SHADER_HPP

#include "../math/vector.hpp"
#include "../math/matrix.hpp"
#include <GL/gl.h>
#include <string>
#include <vector>
#include <map>

namespace library
{
	class Shader
	{
	public:
		typedef std::string (*processFunc)(std::string text);
		
		Shader() {};
		Shader(std::string filename, std::vector<std::string>& linkstage);
		Shader(std::string filename, processFunc tokenizer, std::vector<std::string>& linkstage);
		
		// binds/activates this shader
		void bind();
		
		// queries OpenGL for the location of a uniform
		void  prepareUniform(std::string);
		// returns a previocusly prepared uniform location
		GLint getUniform(std::string);
		// returns exposed handle
		GLuint getShader();
		
		// initializes common shader uniform types
		void sendFloat (GLint uniform, float v);
		void sendVec2  (GLint uniform, const vec2& v);
		void sendVec3  (GLint uniform, const vec3& v);
		void sendVec4  (GLint uniform, const vec4& v);
		void sendMatrix(GLint uniform, const Matrix& m);
		// std::string version
		void sendFloat (std::string uniform, float v);
		void sendVec2  (std::string uniform, const vec2& v);
		void sendVec3  (std::string uniform, const vec3& v);
		void sendVec4  (std::string uniform, const vec4& v);
		void sendMatrix(std::string uniform, const Matrix& m);
		// set-once senders
		void sendInteger(std::string uniform, int id);
		
		// unbinds any bound shader
		static void unbind();
		
	private:
		GLuint shader;
		std::map<std::string, GLint> uniforms;
		
		// get errors/warnings from OpenGL context
		void printShaderStatus(GLuint shader, bool linkstage);
		// preprocess files recursively
		std::string shaderProcessor(std::string filename, processFunc tokenizer, bool isVertex);
		
		// keep track of last bound shader
		static GLuint lastShader;
	};
	
}

#endif
