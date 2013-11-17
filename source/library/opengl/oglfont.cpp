#include "oglfont.hpp"

#include "../bitmap/bitmap.hpp"
#include "../math/matrix.hpp"
#include "opengl.hpp"

namespace library
{
	OglFont::OglFont()
	{
		this->size = 0;
		this->lastUnit = -1;
	}
	OglFont::OglFont(std::string filename, int size) : OglFont()
	{
		if (load(filename, size) == false)
			throw "Error: Could not load font image";
	}
	
	bool OglFont::load(std::string filename, int size)
	{
		// create shader, if it doesn't exist
		if (shader.getShader() == 0)
		{
			this->createShader();
		}
		
		if (size <= 0) return false;
		this->size = size;
		
		Bitmap fontImage(filename, Bitmap::PNG);
		if (fontImage.data() == nullptr) return false;
		
		fontImage.parse2D(size, size);
		
		// create texture, upload image data
		font = Texture(GL_TEXTURE_2D_ARRAY);
		font.create(fontImage, true, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
		
		return true;
	}
	
	void OglFont::bind(GLenum unit)
	{
		// bind font shader
		shader.bind();
		if (this->lastUnit != unit)
		{
			this->lastUnit = unit;
			// resend texture slot to shader
			shader.sendInteger("texture", unit);
		}
		// bind texture
		font.bind(unit);
	}
	
	void OglFont::print(const vec3& location, const vec2& size, std::string text)
	{
		if (text.length() == 0) return;
		
		// bind texture
		font.bind(0);
		
		// convert text to font array index positions
		char converted[text.length()];
		for (unsigned int i = 0; i < text.length(); i++)
		{
			converted[i] = text[i] - 32;
		}
		
		// create vertex data
		struct font_vertex_t
		{
			GLfloat x, y, z;
			GLshort s, t, p, q;
			
		};
		// vertex count
		int vertices = text.length() * 4;
		// vertex data
		font_vertex_t* vdata = new font_vertex_t[vertices];
		font_vertex_t* vertex = vdata;
		
		// emit vertices as quads
		for (size_t i = 0; i < text.length(); i++)
		{
			vertex->x = location.x + (0 + i) * size.x;
			vertex->y = location.y + 0.0;
			vertex->z = location.z + 0.0;
			vertex->s = 0;
			vertex->t = 1;
			vertex->p = converted[i];
			vertex++;
			
			vertex->x = location.x + (1 + i) * size.x;
			vertex->y = location.y + 0.0;
			vertex->z = location.z + 0.0;
			vertex->s = 1;
			vertex->t = 1;
			vertex->p = converted[i];
			vertex++;
			
			vertex->x = location.x + (1 + i) * size.x;
			vertex->y = location.y + size.y;
			vertex->z = location.z + 0.0;
			vertex->s = 1;
			vertex->t = 0;
			vertex->p = converted[i];
			vertex++;
			
			vertex->x = location.x + (0 + i) * size.x;
			vertex->y = location.y + size.y;
			vertex->z = location.z + 0.0;
			vertex->s = 0;
			vertex->t = 0;
			vertex->p = converted[i];
			vertex++;
		}
		
		// upload data to vao
		vao.begin(sizeof(font_vertex_t), vertices, vdata, GL_STREAM_DRAW);
		vao.attrib(0, 3, GL_FLOAT, GL_FALSE, offsetof(font_vertex_t, x));
		vao.attrib(1, 4, GL_SHORT, GL_FALSE, offsetof(font_vertex_t, s));
		vao.end();
		
		// render
		glDrawArrays(GL_QUADS, 0, vertices);
	}
	
	vec2 OglFont::measure(std::string text) const
	{
		return vec2(this->size * text.length(), this->size);
	}
	int OglFont::getSize() const
	{
		return this->size;
	}
	
	void OglFont::sendMatrix(Matrix& matrix)
	{
		shader.sendMatrix("mvp", matrix);
	}
	
	void OglFont::setBackColor(const vec4& color)
	{
		shader.sendVec4("bgcolor", color);
	}
	void OglFont::setColor(const vec4& color)
	{
		shader.sendVec4("fcolor", color);
	}
	
}
