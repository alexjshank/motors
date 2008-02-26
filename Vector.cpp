#include ".\vector.h"

float dist(Vector a, Vector b) {
	return (float)(a-b).len();
}

float dist2(Vector a, Vector b) {
	return (float)(a-b).len2();
}

Vector Vector::operator-() {
	Vector v;
	v.x = -x; v.y = -y; v.z = -z;
	return v;
}


//------------------------------------------------------------------
//  Scalar Ops
//------------------------------------------------------------------

// Scalar Multiplies
Vector Vector::operator*(Vector w) {
	Vector v;
	v.x = x*w.x;
	v.y = y*w.y;
	v.z = z*w.z;
	return v;
}

Vector Vector::operator*(float c) {
	Vector v;
	v.x = c * x;
	v.y = c * y;
	v.z = c * z;
	return v;
}

// Scalar Divides
Vector Vector::operator/(Vector w) {
	Vector v;
	v.x = x / w.x;
	v.y = y / w.y;
	v.z = z / w.z;
	return v;
}

Vector Vector::operator/(float c) {
	Vector v;
	v.x = x / c;
	v.y = y / c;
	v.z = z / c;
	return v;
}

//------------------------------------------------------------------
//  Arithmetic Ops
//------------------------------------------------------------------

Vector Vector::operator+( Vector w ) {
	Vector v;
	v.x = x + w.x;
	v.y = y + w.y;
	v.z = z + w.z;
	return v;
}

Vector Vector::operator-( Vector w ) {
	Vector v;
	v.x = x - w.x;
	v.y = y - w.y;
	v.z = z - w.z;
	return v;
}

//------------------------------------------------------------------
//  Products
//------------------------------------------------------------------

// Inner Dot Product
float Vector::Dot( Vector w ) {
	return (x * w.x + y * w.y + z * w.z);
}

// 3D Exterior Cross Product
Vector Vector::operator^( Vector w ) {
	Vector v;
	v.x = y * w.z - z * w.y;
	v.y = z * w.x - x * w.z;
	v.z = x * w.y - y * w.x;
	return v;
}

//------------------------------------------------------------------
//  Shorthand Ops
//------------------------------------------------------------------

Vector& Vector::operator*=( float c ) {        // vector scalar mult
	x *= c;
	y *= c;
	z *= c;
	return *this;
}

Vector& Vector::operator/=( float c ) {        // vector scalar div
	x /= c;
	y /= c;
	z /= c;
	return *this;
}

Vector& Vector::operator+=( Vector w ) {        // vector increment
	x += w.x;
	y += w.y;
	z += w.z;
	return *this;
}

Vector& Vector::operator-=( Vector w ) {        // vector decrement
	x -= w.x;
	y -= w.y;
	z -= w.z;
	return *this;
}

Vector& Vector::operator^=( Vector w ) {        // 3D exterior cross product
	float ox=x, oy=y, oz=z;
	x = oy * w.z - oz * w.y;
	y = oz * w.x - ox * w.z;
	z = ox * w.y - oy * w.x;
	return *this;
}

//------------------------------------------------------------------
//  Special Operations
//------------------------------------------------------------------

void Vector::normalize() {                      // convert to unit length
	float ln = sqrt( x*x + y*y + z*z );
	if (ln == 0) return;                    // do nothing for nothing
	x /= ln;
	y /= ln;
	z /= ln;
}

Vector Normalize(Vector v) {
	v.normalize();
	return v;
}

float DistanceBetween(Vector a, Vector b) { return (float)(a - b).len(); }