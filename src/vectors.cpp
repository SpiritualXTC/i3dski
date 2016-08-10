/*
 * Name: Chris Snell      
 * Student ID: s3435406
 *
 * Companion to vectors.h
 */


#include "vectors.h"


#include <math.h>

point2::point2()
{
	x = 0;
	y = 0;
}

point2::point2(int ix, int iy)
{
	x = ix;
	y = iy;
}





vec2::vec2()
{
	x = 0.0f;
	y = 0.0f;
}

vec2::vec2(float fx, float fy)
{
	x = fx;
	y = fy;
}


#ifndef GENERICS
/* Constructor */
vec3::vec3()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

/* Constructor */
vec3::vec3(float fx, float fy, float fz)
{
	x = fx;
	y = fy;
	z = fz;
}

/* Unit Vector */
vec3& vec3::normalise()
{
	float len = length();

	x /= len;
	y /= len;
	z /= len;

	return *this;
}

/* Length */
float vec3::length()
{
	return sqrtf(x * x + y * y + z * z);
}

/* Make Vector of Length 'n' */
vec3& vec3::length(float len)
{
	float l = length() / len;
	x /= l;
	y /= l;
	z /= l;

	return *this;
}


/* Dot Product */
float vec3::dot(const vec3 &v)
{
	return x * v.x + y * v.y + z * v.z;
}

/* Scale the Vector */
vec3& vec3::scale(float n)
{
	x *= n;
	y *= n;
	z *= n;

	return *this;
}

/* Scale the Vector (Component form) */
vec3& vec3::scale(float nx, float ny, float nz)
{
	x *= nx;
	y *= ny;
	z *= nz;

	return *this;
}




/* Equality */
bool vec3::operator==(const vec3 &v)
{
	return x == v.x && y == v.y && z == v.z;
}

/* InEquality */
bool vec3::operator!=(const vec3 &v)
{
	return x != v.x || y != v.y || z != v.z;
}


/* Negation */
vec3 vec3::operator-(void)
{
	return vec3(-x, -y, -z);
}




/* Additive */
vec3& vec3::operator+=(const vec3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;

	return *this;
}

/* Subtractive */
vec3& vec3::operator-=(const vec3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;

	return *this;
}

/* Scale */
vec3& vec3::operator*=(const float f)
{
	x *= f;
	y *= f;
	z *= f;

	return *this;
}

/* Scale */
vec3& vec3::operator/=(const float f)
{
	x /= f;
	y /= f;
	z /= f;

	return *this;
}





/* Scale Vector */
vec3 vec3::operator*(const float f)
{
	return vec3(x * f, y * f, z * f);
}

/* Scale Vector */
vec3 vec3::operator/(const float f)
{
	return vec3(x / f, y / f, z / f);
}

/* Addition */
vec3 vec3::operator+(const vec3 &v)
{
	return vec3(x + v.x, y + v.y, z + v.z);
}

/* Subtraction */
vec3 vec3::operator-(const vec3 &v)
{
	return vec3(x - v.x, y - v.y, z - v.z);
}

/* Cross Product */
vec3 vec3::operator*(const vec3 &v)
{
	return vec3(
		y * v.z - z * v.y, 
		z * v.x - x * v.z, 
		x * v.y - y * v.x);
}
#endif








/* Constructor */
vec4::vec4()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	w = 0.0f;
}

/* Constructor */
vec4::vec4(float fx, float fy, float fz)
{
	x = fx;
	y = fy;
	z = fz;
	w = 0.0f;
}

/* Constructor */
vec4::vec4(float fx, float fy, float fz, float fw)
{
	x = fx;
	y = fy;
	z = fz;
	w = fw;
}

/* Unit Plane */
plane::plane()
{
	a = 0.0f;
	b = 1.0f;
	c = 0.0f;
	d = 0.0f;
}

/* Plane Position at Origin */
plane::plane(float nx, float ny, float nz)
{
	a = nx;
	b = ny;
	c = nz;
	d = 0.0f;
}

/* Plane Positioned at Origin */
plane::plane(const vec3 &n)
{
	a = n.x;
	b = n.y;
	c = n.z;
	d = 0.0f;
}

/* Untested */
plane::plane(const vec3 &n, const vec3 &p)
{
	/* Ax + By + Cz = D */
	a = n.x;
	b = n.y;
	c = n.z;
	d = n.x * p.x + n.y * p.y + n.z * p.z; //??
}

/*
	intersect():

	Intersection of a line, with an Origin Positioned Plane ONLY
*/
bool plane::intersect(vec3& out, vec3 &p0, vec3 &p1)
{
	vec3 u = p1 - p0;
	vec3 w = p0 - vec3(0,0,0);	//Vec3(0,0,0) = Origin

	vec3 n = vec3(a, b, c);

	float dot = n.dot(u);

	if (fabsf(dot) > EPSILON)
	{
		float fac = -n.dot(w) / dot;
		
		out = p0 + (u * fac);
		
		return true;
	}
	else /* Parallel */
		return false;
}

/*
# intersection function
def isect_line_plane_v3(p0, p1, p_co, p_no, epsilon=0.000001):
    """
    p0, p1: define the line
    p_co, p_no: define the plane:
        p_co is a point on the plane (plane coordinate).
        p_no is a normal vector defining the plane direction; does not need to be normalized.

    return a Vector or None (when the intersection can't be found).
    """

    u = sub_v3v3(p1, p0)
    w = sub_v3v3(p0, p_co)
    dot = dot_v3v3(p_no, u)

    if abs(dot) > epsilon:
        # the factor of the point between p0 -> p1 (0 - 1)
        # if 'fac' is between (0 - 1) the point intersects with the segment.
        # otherwise:
        #  < 0.0: behind p0.
        #  > 1.0: infront of p1.
        fac = -dot_v3v3(p_no, w) / dot
        mul_v3_fl(u, fac)
        return add_v3v3(p0, u)
    else:
        # The segment is parallel to plane
        return None
*/





/*
	Basic Region
*/
region2::region2()
{
	minimum = vec2(0,0);
	maximum = vec2(0,0);
}

/*
	Set Region Values
*/
region2::region2(const float minX, const float minY, const float maxX, const float maxY)
{
	minimum = vec2(minX, minY);
	maximum = vec2(maxX, maxY);
}






/* Constructor */
#ifdef GENERICS
template<typename T> v3<typename T>::v3()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

/* Constructor */
template<typename T> v3<T>::v3(T fx, T fy, T fz)
{
	x = fx;
	y = fy;
	z = fz;
}



/* Unit Vector */
template<typename T> v3<T>& v3<float>::normalise()
{
	T len = length();

	x = x/len;
	y = y/len;
	z = z/len;

	return *this;
}



/* Length */
template<typename T> T v3<T>::length()
{
	return sqrtf(x * x + y * y + z * z);
}



/* Dot Product */
template<typename T> T v3<T>::dot(const v3<T> &v)
{
	return x * v.x + y * v.y + z * v.z;
}



/* Equality */
template<typename T> bool v3<T>::operator==(const v3<T> &v)
{
	return x == v.x && y == v.y && z == v.z;
}


/* Negation */
template<typename T> v3<T> v3<T>::operator-(void)
{
	return v3(-x, -y, -z);
}




/* Additive */
template<typename T> v3<T>& v3<T>::operator+=(const v3<T>& v)
{
	x += v.x;
	y += v.y;
	z += v.z;

	return *this;
}

/* Subtractive */
template<typename T> v3<T>& v3<T>::operator-=(const v3<T>& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;

	return *this;
}



/* Scale */
template<typename T> v3<T>& v3<T>::operator*=(const T& f)
{
	x *= f;
	y *= f;
	z *= f;

	return *this;
}

/* Scale */
template<typename T> v3<T>& v3<T>::operator/=(const T& f)
{
	x /= f;
	y /= f;
	z /= f;

	return *this;
}





/* Scale Vector */
template<typename T> v3<T> v3<T>::operator*(const T& f)
{
	return v3(x * f, y * f, z * f);
}

/* Addition */
template<typename T> v3<T> v3<T>::operator+(const v3<T>& v)
{
	return vec3(x + v.x, y + v.y, z + v.z);
}

/* Subtraction */
template<typename T> v3<T> v3<T>::operator-(const v3<T>& v)
{
	return v3(x - v.x, y - v.y, z - v.z);
}



/* Cross Product */
template<typename T> v3<T> v3<T>::operator*(const v3<T>& v)
{
	return v3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}

#endif
