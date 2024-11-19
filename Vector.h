#pragma once
#include <math.h>

struct Vector
{
	double x, y, z;

	Vector operator * (const double& d)
	{
		return Vector{ x * d, y * d, z * d };
	}

	double& operator () (const int index)
	{
		if (index == 0)
			return x;
		if (index == 1)
			return y; 
		if (index == 2)
			return z;
	}
};

double length(const Vector& v)
{
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

void normalize(Vector& v)
{
	double l = length(v);
	v.x /= l;
	v.y /= l;
	v.z /= l;
}

double dot(const Vector& v1, const Vector& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}