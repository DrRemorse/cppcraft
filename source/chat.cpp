#include "chat.hpp"

#include <library/opengl/oglfont.hpp>
#include "renderman.hpp"
#include <sstream>

using namespace library;

namespace cppcraft
{
	Chatbox chatbox;
	static const float CHAT_FADEOUT = 256.0;
	
	time_t currentTime()
	{
		time_t rawtime; // long int
		time(&rawtime);
		return rawtime;
		
		/*
		tm* t = localtime(&rawtime);
		if (!t) return "[TIME ERROR]";
		
		std::stringstream ss;
		ss << "[" << t->tm_hour << ":" << t->tm_min << ":" << t->tm_sec << "]";
		return ss.str();
		*/
	}
	
	Chatbox::Chatbox()
	{
		fadeout = CHAT_FADEOUT;
	}
	
	void Chatbox::add(const std::string& text, chattype_t type)
	{
		mtx.lock();
		lines.emplace_back(text, type);
		mtx.unlock();
	}
	void Chatbox::add(const std::string& src, const std::string& text, chattype_t type)
	{
		mtx.lock();
		lines.emplace_back(src, text, type);
		mtx.unlock();
	}
	
	void Chatbox::render(SimpleFont& font, Renderer& renderer)
	{
		//font.setColor();
		float SH = 1.0 / renderer.getScreen().SA;
		
		vec3 cbPos(0.025, SH * 0.85, 0.0);
		vec2 textScale(0.01);
		
		mtx.lock();
		
		// fade out text and remove after time has passed
		this->fadeout -= renderer.dtime;
		if (this->fadeout < 0)
		{
			if (this->lines.size())
			{
				lines.erase(lines.begin(), lines.begin()+1);
			}
			this->fadeout = CHAT_FADEOUT;
		}
		// clone lines before leaving lock
		std::vector<ChatLine> copy = this->lines;
		
		mtx.unlock();
		
		font.setColor(vec4(1.0));
		
		for (size_t i = 0; i < copy.size(); i++)
		{
			vec3 textPos(cbPos);
			textPos.y -= i * textScale.y;
			
			ChatLine& cl = copy[copy.size()-1 - i];
			
			if (i == copy.size()-1)
			{
				font.setColor(vec4(1.0, fadeout / CHAT_FADEOUT));
			}
			font.print(textPos, textScale, cl.text, false);
		}
		
	}
}
