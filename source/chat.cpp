#include "chat.hpp"

#include <library/bitmap/colortools.hpp>
#include <library/opengl/oglfont.hpp>
#include <library/opengl/input.hpp>
#include "network.hpp"
#include "renderman.hpp"
#include "shaderman.hpp"
#include <sstream>

using namespace library;

namespace cppcraft
{
	Chatbox chatbox;
	static const float CHAT_FADEOUT = 512.0;
	static const float CHAT_FADETIME = 256.0;
	
	time_t currentTime()
	{
		time_t rawtime; // long int
		time(&rawtime);
		return rawtime;
	}
	std::string timeString(time_t rawtime)
	{
		tm* t = localtime(&rawtime);
		if (!t) return "[TIME ERROR]";
		
		std::stringstream ss;
		ss << "[" << t->tm_hour << ":" << t->tm_min << ":" << t->tm_sec << "]";
		return ss.str();
	}
	
	Chatbox::ChatLine::ChatLine(std::string Source, std::string Text, chattype_t Type)
		: source(Source), text(Text), type(Type), time(currentTime())
	{
		length = timeString(time).size() + 2 + source.length() + text.size();
		if (type == L_CHAT) length += 2;
	}
	
	void Chatbox::init(float width, float height)
	{
		fadeout = CHAT_FADEOUT;
		unsigned int col      = BGRA8(0, 0, 0, 144);
		unsigned int col_high = BGRA8(0, 0, 0, 192);
		
		struct boxvertex_t
		{
			float x, y, z;
			unsigned int color;
			
			boxvertex_t() {}
			boxvertex_t(float X, float Y, float Z, unsigned int C)
				: x(X), y(Y), z(Z), color(C) {}
		};
		boxvertex_t* boxv = new boxvertex_t[4];
		
		boxv[0] = boxvertex_t(0, 0, 0,  col);
		boxv[1] = boxvertex_t(1, 0, 0,  col);
		boxv[2] = boxvertex_t(1, 1, 0,  col);
		boxv[3] = boxvertex_t(0, 1, 0,  col_high);
		
		cbvao.begin(sizeof(boxvertex_t), 4, boxv);
		cbvao.attrib(0, 3, GL_FLOAT, GL_FALSE, 0);
		cbvao.attrib(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, offsetof(boxvertex_t, color));
		cbvao.end();
		
		delete boxv;
	}
	
	void Chatbox::add(const std::string& src, const std::string& text, chattype_t type)
	{
		mtx.lock();
		lines.emplace_back(src, text, type);
		mtx.unlock();
	}
	
	void Chatbox::renderSourcedMessage(SimpleFont& font, const vec3& spos, const vec2& scale, const std::string& time, const std::string& source, const std::string& text, float alpha)
	{
		vec3 pos(spos);
		
		// print time
		font.setColor(vec4(0.5, alpha));
		font.print(pos, scale, time + " <", false);
		
		// print source
		pos.x += scale.x * (time.size() + 2);
		font.setColor(vec4(1.0, alpha));
		font.print(pos, scale, source, false);
		
		// finish grayed out text
		pos.x += scale.x * source.size();
		font.setColor(vec4(0.5, alpha));
		font.print(pos, scale, "> ", false);
		
		// print message
		pos.x += scale.x * 2;
		font.setColor(vec4(1.0, alpha));
		font.print(pos, scale, text, false);
	}
	void Chatbox::renderInfoMessage(SimpleFont& font, const vec3& spos, const vec2& scale, const std::string& time, const std::string& from, const std::string& text, float alpha)
	{
		vec3 pos(spos);
		
		// print time
		font.setColor(vec4(0.5, alpha));
		font.print(pos, scale, time + " ", false);
		
		// print from
		pos.x += scale.x * (time.size() + 1);
		font.setColor(vec4(0.0, 0.0, 1.0, alpha));
		font.print(pos, scale, from, false);
		
		// print message
		pos.x += scale.x * from.size();
		font.setColor(vec4(1.0, alpha));
		font.print(pos, scale, " " + text, false);
	}
	void Chatbox::renderMessage(SimpleFont& font, const vec3& spos, const vec2& scale, const std::string& time, const std::string& text, float alpha)
	{
		// print time
		font.setColor(vec4(0.7, alpha));
		font.print(spos, scale, time + " ", false);
		
		// print message
		vec3 pos(spos);
		
		pos.x += scale.x * (time.size() + 1);
		font.setColor(vec4(1.0, alpha));
		font.print(pos, scale, text, false);
	}
	
	void Chatbox::render(SimpleFont& font, const mat4& ortho, const vec2& textScale, Renderer& renderer)
	{
		float SH = 1.0 / renderer.getScreen().SA;
		vec3 cbPos(0.025, SH * 0.85, 0.0);
		
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
		
		// find longest chatline
		size_t longest = 0;
		for (ChatLine& cl : copy)
		{
			if (longest < cl.length)
				longest = cl.length;
		}
		
		// minimum background size
		int bgsize = copy.size();
		
		// chatbox background
		Shader& shd = shaderman[Shaderman::GUI_COLOR];
		shd.bind();
		mat4 matbox = ortho;
		matbox.translate_xy(cbPos.x, cbPos.y - (bgsize-1) * textScale.y);
		matbox.scale(longest * textScale.x, bgsize * textScale.y, 1.0);
		shd.sendMatrix("mvp", matbox);
		
		cbvao.render(GL_QUADS);
		
		// render typing box
		if (chatOpen)
		{
			std::string ctext = input.getText() + ((((int) renderer.frametick / 50) % 2 == 0) ? "_" : " ");
			std::string now = timeString(currentTime());
			
			size_t msglen = now.size() + 2 + network.getNickname().size() + 2 + ctext.size() + 1;
			
			matbox = ortho;
			matbox.translate_xy(cbPos.x, cbPos.y + textScale.y + 0.005);
			matbox.scale(msglen * textScale.x, textScale.y, 1.0);
			shd.sendMatrix("mvp", matbox);
			
			cbvao.render(GL_QUADS);
			
			// render text
			font.bind(0);
			font.setBackColor(0.0);
			
			// print actual text typed into chatbox
			vec3 textPos(cbPos);
			textPos.y += textScale.y + 0.005;
			renderSourcedMessage(font, textPos, textScale, now, network.getNickname(), ctext, 1.0);
		}
		
		if (copy.size() == 0) return;
		
		// render chatbox text
		font.bind(0);
		font.setBackColor(0.0);
		font.setColor(vec4(1.0, 1.0));
		
		float alpha = 1.0;
		for (size_t i = 0; i < copy.size(); i++)
		{
			vec3 textPos(cbPos);
			textPos.y -= i * textScale.y;
			
			ChatLine& cl = copy[copy.size()-1 - i];
			
			if (i == copy.size()-1)
			{
				if (fadeout < CHAT_FADETIME)
					alpha = fadeout / CHAT_FADETIME;
			}
			
			switch (cl.type)
			{
			case Chatbox::L_SERVER:
				renderInfoMessage(font, textPos, textScale, timeString(cl.time), cl.source, cl.text, alpha);
				break;
			case Chatbox::L_INFO:
				renderInfoMessage(font, textPos, textScale, timeString(cl.time), cl.source, cl.text, alpha);
				break;
			case Chatbox::L_CHAT:
				renderInfoMessage(font, textPos, textScale, timeString(cl.time), cl.source, cl.text, alpha);
				break;
			}
		}
		
	}
}
