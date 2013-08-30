#ifndef CAMERA_FRUSTUM_HPP
#define CAMERA_FRUSTUM_HPP

#include "library/math/matrix.hpp"
#include "library/math/frustum.hpp"

namespace library
{
	class WindowClass;
}

namespace cppcraft
{
	class Camera
	{
	private:
		float FOV;         // field of view (angle in degrees)
		float znear, zfar; // znear and zfar planes
		int gridsize;
		
		// the viewing frustum
		library::Frustum frustum;
		
		// camera matrices
		library::Matrix matrot;  // R
		library::Matrix matview; // MV
		library::Matrix matproj; // P
		library::Matrix matviewproj; // MVP
		library::Matrix matproj_long; // PL
		
		library::vec3 worldOffset;
		
	public:
		bool recalc;   // recalculate viewing frustum
		bool rotated;  // camera was rotated, resend rotation matrices
		bool ref;      // run occlusion tests
		int  needsupd; // update render list
		int  cameraViewSectors;
		
		void init(library::WindowClass& wnd);
		void calculateFrustum();
		const library::Frustum& camera() const;
		
		float getZNear()  const { return this->znear; }
		float getZFar()   const { return this->zfar; }
		int getGridsize() const { return this->gridsize; }
		
		void setRotation(float rx, float ry, float rz);
		void setTranslation(float tx, float ty, float tz);
		
		void setWorldOffset(double posX, double posZ);
		
		const library::Matrix& getProjection() const;
		const library::Matrix& getProjectionLong() const;
		const library::Matrix& getViewMatrix() const;
		const library::Matrix& getRotationMatrix() const;
		const library::Matrix& getMVP_Matrix() const;
		
		const library::vec3&   getWorldOffset() const;
		
	};
	extern Camera camera;
}

#endif
