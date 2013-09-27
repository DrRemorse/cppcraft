#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "../math/matrix.hpp"
#include "../math/frustum.hpp"

namespace library
{
	class WindowClass;
	
	class Camera
	{
	protected:
		float FOV;         // field of view (angle in degrees)
		float znear, zfar; // znear and zfar planes
		
		// viewing frustum representation
		library::Frustum frustum;
		
		// camera matrices
		library::Matrix matproj; // P
		library::Matrix matrot;  // R
		library::Matrix matview; // MV
		library::Matrix matviewproj; // MVP
		
	public:
		Camera() {}
		Camera(float FOV, float aspect, float znear, float zfar);
		
		void calculateFrustum();
		const library::Frustum& getFrustum() const;
		
		inline float getFOV()   const { return this->FOV; }
		inline float getZNear() const { return this->znear; }
		inline float getZFar()  const { return this->zfar; }
		
		void setRotation   (float rx, float ry, float rz);
		void setTranslation(float tx, float ty, float tz);
		
		void setRotationMatrix(const Matrix& rot);
		void setViewMatrix(const Matrix& view);
		
		const library::Matrix& getProjection() const;
		const library::Matrix& getViewMatrix() const;
		const library::Matrix& getRotationMatrix() const;
		const library::Matrix& getMVP() const;
	};
	
}

#endif
