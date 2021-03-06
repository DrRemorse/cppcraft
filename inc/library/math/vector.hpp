#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <iostream>
#include <cmath>

namespace library
{
	class vec4;
	class vec3;
	
	// light weight 2-vector class
	class vec2
	{
	public:
		typedef float vector_t;
		
		// fields
		vector_t x, y;
		
		// constructors
		vec2()
		{
			x = y = 0.0;
		}
		vec2(vector_t v)
		{
			x = y = v;
		}
		vec2(vector_t X, vector_t Y)
		{
			x = X; y = Y;
		}
		vec2(const vec2& v)
		{
			x = v.x; y = v.y;
		}
		
		inline vector_t length_squared() const
		{
			return x * x + y * y;
		}
		vector_t length() const
		{
			return std::sqrt(length_squared());
		}
		
		vec2& rotate(vector_t angle);
		vec2  rotated(vector_t angle) const;
		
		vec2& normalize();
		vec2  normalized() const
		{
			return vec2(*this).normalize();
		}
		
		vector_t dot(const vec2&) const;
		vector_t determinant(const vec2&) const;
		vec2 frac() const;
		vec2 reflect(const vec2& normal) const;
		
		// exponentiation
		vec2& pow(vector_t e);
		vec2& pow(const vec2& v);
		
		// linear interpolation
		vec2 mix(const vec2& v, vector_t factor) const;
		
		// -= vec2 operators =- //
		
		// unary - (negate)
		vec2 operator - () const;
		
		// arithmetic operators
		vec2& operator += (const vec2&);
		vec2& operator -= (const vec2&);
		vec2& operator *= (const vec2&);
		vec2& operator /= (const vec2&);
		
		vec2 operator + (const vec2&) const;
		vec2 operator - (const vec2&) const;
		vec2 operator * (const vec2&) const;
		vec2 operator / (const vec2&) const;
		
		vec2& operator +=(const vector_t);
		vec2& operator -=(const vector_t);
		vec2& operator *=(const vector_t);
		vec2& operator /=(const vector_t);
		
		vec2  operator + (const vector_t) const;
		vec2  operator - (const vector_t) const;
		vec2  operator * (const vector_t) const;
		vec2  operator / (const vector_t) const;
		
		// boolean equality operators
		bool operator == (const vec2&) const;
		bool operator != (const vec2&) const;
		
		// inline friends
		inline friend vec2 pow(const vec2& v1, vec2::vector_t e)
		{
			return vec2(v1).pow(e);
		}
		inline friend vec2 pow(const vec2& v1, const vec2& v2)
		{
			return vec2(v1).pow(v2);
		}
		
	};
	
	// heavy-weight 3-vector class
	class vec3
	{
	public:
		typedef float vector_t;
		
		// fields
		vector_t x, y, z;
		
		// -= vec3 constructors =- //
		
		vec3()
		{
			x = y = z = 0.0;
		}
		vec3(vec3::vector_t v)
		{
			x = y = z = v;
		}
		vec3(vector_t X, vector_t Y, vector_t Z)
		{
			x = X; y = Y; z = Z;
		}
		vec3(const vec3& v)
		{
			x = v.x; y = v.y; z = v.z;
		}
		
		// -= vec3 swizzles =- //
		
		vec2 xx() const; vec2 yy() const; vec2 zz() const;
		vec2 xy() const; vec2 yz() const; vec2 xz() const;
		
		// -= vec3 utilities =- //
		
		inline vector_t length_squared() const
		{
			return x * x + y * y + z * z;
		}
		vector_t length() const
		{
			return std::sqrt(length_squared());
		}
		vec3& normalize();
		vec3  normalized() const
		{
			return vec3(*this).normalize();
		}
		vector_t dot(const vec3&) const;
		vec3 cross(const vec3&) const;
		vec3 reflect(const vec3& normal) const;
		vec3 axis(const vec3&) const;
		// returns this vector projected onto v
		vec3 project(const vec3& v) const;
		
		// rotate this vector by (angle) on (axis)
		vec3  rotateOnAxis(const vec3& axis, float angle) const;
		vec2  toPitchYaw() const;
		
		vec3 frac() const;
		// linear interpolation
		vec3 mix(const vec3& v, vector_t factor) const;
		
		// exponentiation
		vec3& pow(vector_t e);
		vec3& pow(const vec3& v);
		
		// -= vec3 operators =- //
		
		// unary - (negate)
		vec3 operator - () const;
		
		// arithmetic operators
		vec3& operator += (const vec3&);
		vec3& operator -= (const vec3&);
		vec3& operator *= (const vec3&);
		vec3& operator /= (const vec3&);
		
		vec3 operator + (const vec3&) const;
		vec3 operator - (const vec3&) const;
		vec3 operator * (const vec3&) const;
		vec3 operator / (const vec3&) const;
		
		vec3& operator +=(const vector_t);
		vec3& operator -=(const vector_t);
		vec3& operator *=(const vector_t);
		vec3& operator /=(const vector_t);
		
		vec3  operator + (const vector_t) const;
		vec3  operator - (const vector_t) const;
		vec3  operator * (const vector_t) const;
		vec3  operator / (const vector_t) const;
		
		// boolean equality operators
		bool operator == (const vec3&) const;
		bool operator != (const vec3&) const;
		
		// inline friends
		inline friend vec3 pow(const vec3& v1, vec3::vector_t e)
		{
			return vec3(v1).pow(e);
		}
		inline friend vec3 pow(const vec3& v1, const vec3& v2)
		{
			return vec3(v1).pow(v2);
		}
		
	};
	
	// special homogenous 4-vector class
	class vec4 : public vec3
	{
	public:
		// homogenous coordinate
		vector_t w;
		
		// -= vec4 constructors =- //
		
		vec4()
			: vec3(), w(0.0) {}
		vec4(vector_t v)
			: vec3(v), w(v) {}
		vec4(vector_t v, vector_t W)
			: vec3(v), w(W) {}
		vec4(vector_t x, vector_t y, vector_t z, vector_t W)
			: vec3(x, y, z), w(W) {}
		vec4(const vec2& a, const vec2& b)
			: vec3(a.x, a.y, b.x), w(b.y) {}
		vec4(const vec3& xyz, vector_t W)
			: vec3(xyz), w(W) {}
		vec4(const vector_t* v)
			: vec4(v[0], v[1], v[2], v[3]) {}
		
		// -= vec4 swizzles =- //
		vec3 xyz() const
		{
			return vec3(x, y, z);
		}
		
		// -= vec4 operators =- //
		
		// unary - (negate)
		const vec4 operator - () const
		{
			return vec4(-x, -y, -z, -w);
		}
		
	};
	
	// vec2 functions
	inline vec2::vector_t dot(const vec2& va, const vec2& vb)
	{
		return va.dot(vb);
	}
	inline vec2::vector_t distance(const vec2& va, const vec2& vb)
	{
		return (va - vb).length();
	}
	inline vec2 normalize(const vec2& v)
	{
		return v.normalized();
	}
	vec2 slerp(const vec2& va, const vec2& vb, vec2::vector_t t);
	vec2 rotateTowards(const vec2& from, const vec2& to, vec2::vector_t dTheta);
	
	// vec3 language functions
	inline vec3::vector_t dot(const vec3& va, const vec3& vb)
	{
		return va.dot(vb);
	}
	inline vec3::vector_t distance(const vec3& va, const vec3& vb)
	{
		return (va - vb).length();
	}
	inline vec3 normalize(const vec3& v)
	{
		return v.normalized();
	}
	inline vec3 cross(const vec3& va, const vec3& vb)
	{
		return va.cross(vb);
	}
	vec3 reflect(const vec3& I, const vec3& N);
	vec3 refract(const vec3& I, const vec3& N, const vec3::vector_t eta);
	// transform this vector from pitch and yaw
	vec3 lookVector(const vec2& radians);
	
	// additional arithmetic operators
	template <typename T>
	vec2 operator + (const T t, const vec2& v)
	{
		return vec2(v.x + t, v.y + t);
	}
	template <typename T>
	vec2 operator - (const T t, const vec2& v)
	{
		return vec2(v.x - t, v.y - t);
	}
	template <typename T>
	vec2 operator * (const T t, const vec2& v)
	{
		return vec2(v.x * t, v.y * t);
	}
	template <typename T>
	vec2 operator / (const T t, const vec2& v)
	{
		return vec2(v.x / t, v.y / t);
	}
	
	template <typename T>
	vec3 operator + (const T t, const vec3& v)
	{
		return vec3(v.x + t, v.y + t, v.z + t);
	}
	template <typename T>
	vec3 operator - (const T t, const vec3& v)
	{
		return vec3(v.x - t, v.y - t, v.z - t);
	}
	template <typename T>
	vec3 operator * (const T t, const vec3& v)
	{
		return vec3(v.x * t, v.y * t, v.z * t);
	}
	template <typename T>
	vec3 operator / (const T t, const vec3& v)
	{
		return vec3(v.x / t, v.y / t, v.z / t);
	}
	
	// log output functions
	class Log;
	Log& operator<< (Log& out, const vec2& v);
	Log& operator<< (Log& out, const vec3& v);
	Log& operator<< (Log& out, const vec4& v);
}

// cout output functions
std::ostream& operator<< (std::ostream& out, const library::vec2& v);
std::ostream& operator<< (std::ostream& out, const library::vec3& v);
std::ostream& operator<< (std::ostream& out, const library::vec4& v);

#endif
