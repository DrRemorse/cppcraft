#ifndef OGLFONT_HPP
#define OGLFONT_HPP
/**
 * Simple OpenGL font implementation
 * 
 * Example usage (16x16 tile size)
 * 
 * *** initialization ***
 * // initialize our font renderer
 * OglFont font("bitmaps/font.png", 16);
 * // or:
 * font.load("bitmap/default/gui/font.png", 16);
 * 
 * // bind the font (to texture slot 0)
 * font.bind(0);
 * // send our projection or mvp matrix:
 * font.sendMatrix(ortho);
 * 
 * // set foreground & background color
 * font.setColor(vec4(1.0)); // white
 * font.setBackColor(vec4(0.0, 0.5)); // 50% transparent black
 * 
 * *** in rendering loop ***
 * // make sure the font object is bound
 * font.bind(0);
 * // print text to (0.5, 0.5, 0.0), scaled by (0.1, 0.1) with text: Hello world!
 * font.print(vec3(0.5, 0.5, 0.0), vec2(0.1), "Hello world!");
 * 
 * Some tips:
 * Try to bind to the same texture slot, so the font object
 * doesn't have to update the uniform in the shader program each frame.
**/

#include "../math/vector.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "vao.hpp"
#include <string>

namespace library
{
	class Matrix;
	
	class OglFont
	{
	public:
		OglFont();
		OglFont(std::string filename, int size);
		
		bool load(std::string filename, int size);
		
		void bind(GLenum unit);
		void sendMatrix(Matrix& matrix);
		
		void setBackColor(const vec4& color);
		void setColor(const vec4& color);
		
		void print(const vec3& location, const vec2& size, std::string text);
		
		// returns 2D size of string in pixels
		vec2 measure(std::string text) const;
		// size in pixels
		int getSize() const;
		
	private:
		void createShader();
		
		// font texture info
		int size;
		GLenum lastUnit;
		Texture font;
		Shader  shader;
		VAO vao;
	};
	
}

#endif
