#include "chat.hpp"

namespace cppcraft
{
	
	
	void Chatbox::add(std::string text, color_t color, bool newline)
	{
		ChatboxLine cbl;
		cbl.text    = text;
		cbl.color   = color;
		cbl.newline = newline;
	}
	
	
	
}