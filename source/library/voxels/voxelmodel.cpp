#include "voxelmodel.hpp"

#include "../opengl/opengl.hpp"
#include "voxelizer.hpp"

namespace library
{
	void VoxelModel::create(GLsizei vertices, GLvoid* data)
	{
		vao.begin(sizeof(XModel::xvertex_t), vertices, data);
		vao.attrib(0, 3, GL_FLOAT,         GL_FALSE, 0);
		vao.attrib(1, 4, GL_BYTE,          GL_TRUE, 12);
		vao.attrib(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, 16);
		vao.end();
	}
	
	void VoxelModel::render()
	{
		vao.render(GL_QUADS);
	}
	
}