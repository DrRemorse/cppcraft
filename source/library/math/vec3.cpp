#include "vector.hpp"

#include "../log.hpp"
#include <cmath>

namespace library
{
	static const double PI = 4 * atan(1);
	
	// vec3 constructors
	vec3::vec3()
	{
		x = y = z = 0.0;
	}
	vec3::vec3(vec3::vector_t v)
	{
		x = y = z = v;
	}
	vec3::vec3(vec3::vector_t X, vec3::vector_t Y, vec3::vector_t Z)
	{
		x = X; y = Y; z = Z;
	}
	vec3::vec3(const vec3& v)
	{
		x = v.x; y = v.y; z = v.z;
	}
	vec3::vec3(const vec4& v)
	{
		x = v.x; y = v.y; z = v.z;
	}
	
	// vec3 swizzles
	vec2 vec3::xx() const
	{
		return vec2(x, x);
	}
	vec2 vec3::yy() const
	{
		return vec2(y, y);
	}
	vec2 vec3::zz() const
	{
		return vec2(z, z);
	}
	vec2 vec3::xy() const
	{
		return vec2(x, y);
	}
	vec2 vec3::yz() const
	{
		return vec2(y, z);
	}
	vec2 vec3::xz() const
	{
		return vec2(x, z);
	}
	
	// vec3 utility
	vec3::vector_t vec3::length() const
	{
		vector_t L = x * x + y * y + z * z;
		return sqrtf(L);
	}
	
	vec3& vec3::normalize()
	{
		vector_t L = length();
		if (L == 0) return *this;
		// normalize to unit length vector
		L = 1.0 / L;
		return *this *= L;
	}
	vec3 vec3::normalized() const
	{
		return vec3(*this).normalize();
	}
	
	vec3::vector_t vec3::dot(const vec3& v) const
	{
		return x * v.x + y * v.y + z * v.z;
	}
	
	vec3 vec3::cross(const vec3& v) const
	{
		return vec3
		(
			(y * v.z) - (v.y * z),
			(z * v.x) - (v.z * x),
			(x * v.y) - (v.x * y)
		);
	}
	
	vec3 vec3::frac() const
	{
		// NB. does not work for negative numbers
		return vec3( x - (int)x, y - (int)y, z - (int)z );
	}
	
	vec3 vec3::reflect(const vec3& normal) const
	{
		// incident - 2.0 * dot(normal, incident) * normal
		return *this - vec3(normal) * 2.0f * dot(normal);
	}
	
	vec3 vec3::mix(const vec3& v, float mixlevel) const
	{
		return vec3(
			this->x * (1.0 - mixlevel) + v.x * mixlevel,
			this->y * (1.0 - mixlevel) + v.y * mixlevel,
			this->z * (1.0 - mixlevel) + v.z * mixlevel
		);
	}
	
	// rotate vector around axis angle - axis vector, angle is theta (in radians),
	// 'this' is the vector we are going to rotate
	vec3 vec3::rotateOnAxis(const vec3& axis, float angle) const
	{
		// formula: vector * cos a + dot(vector, axis) * axis * (1 - cos a) + cross(axis, vector) * sin a
		return *this * cosf(angle) + this->dot(axis) * axis * (1.0 - cosf(angle)) + axis.cross(*this) * sinf(angle);
	}
	
	// transforms this vector into a rotation expressed by two angles (rotX, rotY)
	vec3& vec3::lookVector(const vec2& rot)
	{
		x =  sinf(rot.y) * cosf(rot.x);
		y = 			  -sinf(rot.x);
		z = -cosf(rot.y) * cosf(rot.x);
		return *this;
	}
	
	vec2 vec3::toPitchYaw() const
	{
		float xzdist = xz().length();
		// pitch
		float pitch = -atan2(y, xzdist);
		
		// yaw
		float yaw = atan2(x, -z);
		if (yaw < 0) yaw += PI * 2;
		
		return vec2(pitch, yaw);
	}
	
	const vec3::vector_t vec3::min() const
	{
		if (x < y && x < z) return x;
		if (y < z) return y;
		return z;
	}
	const vec3::vector_t vec3::max() const
	{
		if (x > y && x > z) return x;
		if (y > z) return y;
		return z;
	}
	
	const vec3& vec3::min(const vec3& v) const
	{
		return (length() < v.length()) ? *this : v;
	}
	const vec3& vec3::max(const vec3& v) const
	{
		return (length() > v.length()) ? *this : v;
	}
	
	// unary - (negate)
	const vec3 vec3::operator - () const
	{
		return vec3(-x, -y, -z);
	}
	
	// arithmetic vector-vector operators
	vec3 vec3::operator+ (const vec3& v) const
	{
		return vec3(x + v.x, y + v.y, z + v.z);
	}
	vec3 vec3::operator- (const vec3& v) const
	{
		return vec3(x - v.x, y - v.y, z - v.z);
	}
	vec3 vec3::operator* (const vec3& v) const
	{
		return vec3(x * v.x, y * v.y, z * v.z);
	}
	vec3 vec3::operator/ (const vec3& v) const
	{
		return vec3(x / v.x, y / v.y, z / v.z);
	}
	
	vec3& vec3::operator += (const vec3& v)
	{
		this->x += v.x;
		this->y += v.y;
		this->z += v.z;
		return *this;
	}
	vec3& vec3::operator -= (const vec3& v)
	{
		this->x -= v.x;
		this->y -= v.y;
		this->z -= v.z;
		return *this;
	}
	vec3& vec3::operator *= (const vec3& v)
	{
		this->x *= v.x;
		this->y *= v.y;
		this->z *= v.z;
		return *this;
	}
	vec3& vec3::operator /= (const vec3& v)
	{
		this->x /= v.x;
		this->y /= v.y;
		this->z /= v.z;
		return *this;
	}
	
	// vector arithmetic literal operators
	vec3 vec3::operator+ (const vector_t v) const
	{
		return vec3(x + v, y + v, z + v);
	}
	vec3 vec3::operator- (const vector_t v) const
	{
		return vec3(x - v, y - v, z - v);
	}
	vec3 vec3::operator* (const vector_t v) const
	{
		return vec3(x * v, y * v, z * v);
	}
	vec3 vec3::operator/ (const vector_t v) const
	{
		return vec3(x / v, y / v, z / v);
	}
	
	// vector compound operators
	vec3& vec3::operator +=(const vector_t v)
	{
		x += v; y += v; z += v;
		return *this;
	}
	vec3& vec3::operator -=(const vector_t v)
	{
		x -= v; y -= v; z -= v;
		return *this;
	}
	vec3& vec3::operator *=(const vector_t v)
	{
		x *= v; y *= v; z *= v;
		return *this;
	}
	vec3& vec3::operator /=(const vector_t v)
	{
		x /= v; y /= v; z /= v;
		return *this;
	}
	
	// exponentiate "operator"
	vec3& vec3::pow(double e)
	{
		x = ::pow(x, e); y = ::pow(y, e); z = ::pow(z, e);
		return *this;
	}
	
	vec3& vec3::pow(const vec3& v)
	{
		x = ::pow(x, v.x); y = ::pow(y, v.y); z = ::pow(z, v.z);
		return *this;
	}
	
	// boolean equality operator
	bool vec3::operator == (const vec3& v) const
	{
		return (x == v.x && y == v.y && z == v.z);
	}
	
	// boolean inequality operator
	bool vec3::operator != (const vec3& v) const
	{
		return !(*this == v);
	}
	
	// vector language functions
	vec3 distance(const vec3& va, const vec3& vb)
	{
		return (va - vb).length();
	}
	
	vec3::vector_t dot(const vec3& va, const vec3& vb)
	{
		return va.dot(vb);
	}
	
	vec3 normalize(const vec3& v)
	{
		return vec3(v).normalize();
	}
	
	vec3 cross(const vec3& va, const vec3& vb)
	{
		return va.cross(vb);
	}
	
	/*
		http://www.opengl.org/sdk/docs/manglsl/xhtml/reflect.xml
		For a given incident vector I and surface normal N reflect returns the reflection direction calculated as
		R = I - 2.0 * dot(N, I) * N
	*/
	vec3 reflect(const vec3& I, const vec3& N)
	{
		return I - 2.0 * dot(N, I) * N;
	}
	
	/*
		http://www.opengl.org/sdk/docs/manglsl/xhtml/refract.xml
		For a given incident vector I, surface normal N and ratio of indices of refraction, eta, refract returns the refraction vector, R.
		R is calculated as:
		k = 1.0 - eta * eta * (1.0 - dot(N, I) * dot(N, I));
		if (k < 0.0)
			R = genType(0.0);       // or genDType(0.0)
		else
			R = eta * I - (eta * dot(N, I) + sqrt(k)) * N;
	*/
	vec3 refract(const vec3& I, const vec3& N, const vec3::vector_t eta)
	{
		// incident vector I, normal vector N (both normalized)
		vec3::vector_t dotNI = dot(N, I);
		
		vec3::vector_t k = 1.0 - eta * eta * (1.0 - dotNI * dotNI);
		
		if (k < 0.0) return (vec3::vector_t)0.0;
		
		return eta * I - (eta * dotNI + sqrtf(k)) * N;
	}
	
	// log output functions
	
	// write vector-3 to log using format (x, y, z)
	Log& operator<< (Log& out, const vec3& v)
	{
		return out << "(" << v.x << ", " << v.y << ", " << v.z << ")";
	}
	
	// write vector-3 to cout using format (x, y, z)
	std::ostream& operator<< (std::ostream& out, const vec3& v)
	{
		return out << "(" << v.x << ", " << v.y << ", " << v.z << ")";
	}
	
}
