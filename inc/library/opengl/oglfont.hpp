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
	class mat4;
	
	class OglFont
	{
	public:
		struct font_vertex_t
		{
			float x, y, z;
			signed short s, t, p, q;
		};
		
		OglFont() : tilesize(0), lastUnit(-1), vertices(0), vdata(nullptr) {}
		OglFont(const std::string& filename, int fontsize);
		bool load(const std::string& filename, int fontsize);
		
		void bind(GLenum unit);
		// Y-axis is upwards
		void print(const vec3& location, const vec2& size, std::string text);
		// Y-axis is downwards
		void print2d(const vec3& location, const vec2& size, std::string text);
		
		// returns 2D size of string in pixels
		vec2 measure(std::string text) const;
		
		// size in pixels
		inline int getFontSize() const
		{
			return this->tilesize;
		}
		
		inline void sendMatrix(const mat4& matrix)
		{
			shader->sendMatrix("mvp", matrix);
		}
		inline void setBackColor(const vec4& color)
		{
			shader->sendVec4("bgcolor", color);
		}
		inline void setColor(const vec4& color)
		{
			shader->sendVec4("fcolor", color);
		}
		
		inline Shader& getShader() { return *shader; }
		inline void setShader(Shader& shd)
		{
			shader = &shd;
		}
		void createDefaultShader();
		
	private:
		// texture tile size
		int tilesize;
		// last texture unit for font
		GLenum lastUnit;
		Texture font;
		Shader* shader;
		int vertices;
		font_vertex_t* vdata;
		VAO vao;
	};
	
}

#endif
