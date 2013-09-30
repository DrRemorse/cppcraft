#ifndef VOXELIZER_HPP
#define VOXELIZER_HPP

namespace library
{
	class Bitmap;
	class vec3;
	
	class XModel
	{
	public:
		typedef unsigned int xcolor_t;
		
		struct xvertex_t
		{
			float x, y, z;
			char nx, ny, nz, p1;
			xcolor_t c;
			
		};
		
		XModel();
		XModel(XModel& mod);
		~XModel();
		
		xvertex_t* data();
		int        vertices();
		
		void extrude(Bitmap& fromImage, vec3& offset, vec3& scale);
		
		static void initVoxelizer();
		
	private:
		static const int VOXELIZER_MAX_VERTICES = 32768;
		static xvertex_t* xv_dump;
		
		short cull2D(Bitmap& img, int x, int y);
		void putv2D(vec3& offset, vec3& scale, int x, int y, int fid, int vid, xcolor_t vcolor);
		
		xvertex_t* vdata;
		int verts;
	};
}
#endif
