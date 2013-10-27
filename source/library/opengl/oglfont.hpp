#ifndef OGLFONT_HPP
#define OGLFONT_HPP

/**
 * Cheap OpenGL font implementation
 * 
**/

#include "../math/vector.hpp"
#include "texture.hpp"
#include "vao.hpp"
#include <string>

namespace library
{
	class OglFont
	{
	public:
		OglFont(int size);
		bool load(std::string filename);
		void print(std::string text, int charset = 0);
		
		// returns 2D size of string in pixels
		vec2 measure(std::string text) const;
		// size in pixels
		int getSize() const;
		
	private:
		// font texture info
		int size;
		Texture font;
		VAO vao;
	};
	
}

#endif
