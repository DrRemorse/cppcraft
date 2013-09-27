#include "camera.hpp"

#include "../config.hpp"
#include "../log.hpp"
#include "window.hpp"

namespace library
{
	Camera::Camera(float FOV, float aspect, float znear, float zfar)
	{
		if (znear > zfar) throw std::string("Camera::Camera(): Znear must be less than Zfar");
		if (znear <= 0)   throw std::string("Camera::Camera(): Znear must be greater than zero");
		
		this->FOV = FOV;
		this->znear = znear;
		this->zfar  = zfar;
		
		// projection matrix
		matproj.perspective(FOV, aspect, znear, zfar);
		
		// initialize matrices
		setRotation(0, 0, 0);
		setTranslation(0, 0, 0);
	}
	
	void Camera::calculateFrustum()
	{
		frustum.calculate(matviewproj);
	}
	const Frustum& Camera::getFrustum() const
	{
		return this->frustum;
	}
	
	void Camera::setRotation(float rx, float ry, float rz)
	{
		matrot.rotateZYX(rx, ry, rz);
	}
	
	void Camera::setTranslation(float tx, float ty, float tz)
	{
		// view matrix = rotation & translation
		matview = matrot;
		matview.translated(tx, ty, tz);
		
		// modelview projection matrix
		matviewproj = matproj * matview;
	}
	
	void Camera::setRotationMatrix(const Matrix& rot)
	{
		matrot = rot;
	}
	void Camera::setViewMatrix(const Matrix& view)
	{
		matview = view;
		matviewproj = matproj * matview;
	}
	
	const Matrix& Camera::getProjection() const
	{
		return matproj;
	}
	const Matrix& Camera::getViewMatrix() const
	{
		return matview;
	}
	const Matrix& Camera::getRotationMatrix() const
	{
		return matrot;
	}
	const Matrix& Camera::getMVP() const
	{
		return matviewproj;
	}
}
