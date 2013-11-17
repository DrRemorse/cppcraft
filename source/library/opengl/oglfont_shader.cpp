#include "oglfont.hpp"

#include <string>
#include <vector>

namespace library
{
	const std::string font_vertex_shader = 
	R"(
		#version 130
		uniform mat4 mvp;
		
		in vec3 in_vertex;
		in vec3 in_texture;
		
		out vec3 texCoord;
		
		void main(void)
		{
			texCoord = in_texture;
			
			gl_Position = mvp * vec4(in_vertex.xyz, 1.0);
		}
	)";
	const std::string font_fragment_shader = 
	R"(
		#version 130
		#extension GL_EXT_gpu_shader4 : enable
		
		uniform sampler2DArray texture;
		uniform vec4 bgcolor;
		uniform vec4 fcolor;
		
		in vec3 texCoord;
		
		void main(void)
		{
			vec4 color = texture2DArray(texture, texCoord.stp);
			
			color = mix(bgcolor, fcolor, color.r);
			
			gl_FragData[0] = color;
		}
	)";
	
	void OglFont::createShader()
	{
		std::vector<std::string> attr;
		attr.emplace_back("in_vertex");
		attr.emplace_back("in_texture");
		
		shader = Shader(font_vertex_shader, font_fragment_shader, "Internal font shader", attr);
		shader.sendVec4("bgcolor", vec4(0.0, 1.0)); // black
		shader.sendVec4("fcolor", vec4(1.0)); // white
	}
}
