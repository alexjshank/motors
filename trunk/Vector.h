#pragma once

#include <math.h>



class Vector {
public:
	Vector() { x=0; y=0; z=0; };
	Vector(float a) { x=a; y=a; z=a; };
	Vector(float a, float b, float c) { x=a; y=b; z=c; };
	Vector(float a[]) { x=a[0]; y=a[1]; z=a[2]; };
	~Vector() {};

	Vector operator-();              

	//----------------------------------------------------------
	// Scalar Multiplication
	Vector operator*(Vector);
	Vector operator*(float);
	// Scalar Division
	Vector operator/(Vector);
	Vector operator/(float);

	//----------------------------------------------------------
	// Vector Arithmetic Operations
	Vector operator+(Vector);        // vector add
	Vector operator-(Vector);        // vector subtract
	float Dot(Vector);        // inner dot product
	float operator|(Vector);        // 2D exterior perp product
	Vector operator^(Vector);        // 3D exterior cross product

	Vector& operator*=(float);      // vector scalar mult
	Vector& operator/=(float);      // vector scalar div
	Vector& operator+=(Vector);      // vector increment
	Vector& operator-=(Vector);      // vector decrement
	Vector& operator^=(Vector);      // 3D exterior cross product

	bool operator==(Vector a) { return (x == a.x && y == a.y && z == a.z); }

	//----------------------------------------------------------
	// Vector Properties
	double len() {                    // vector length
		return sqrt(x*x + y*y + z*z);
	}
	double len2() {                   // vector length squared
		return (x*x + y*y + z*z);
	}
	Vector flat() {
		return Vector(x,0,z);
	}

	//----------------------------------------------------------
	// Special Operations
	void normalize();                 // convert vector to unit length

	float x,y,z;
};

float dist(Vector a, Vector b);
float dist2(Vector a, Vector b);

Vector Normalize(Vector v);
float DistanceBetween(Vector a, Vector b);