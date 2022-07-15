#ifndef GEOMETRY_HH
#define GEOMETRY_HH

#include <iostream>
#include <vector>
#include <cmath>
#include <string>

enum class Triangle_type {
	SSS,
	SSA,
	SAA
};

enum class Decomp_type {
	TRI,
	WIR
};


class Shape {
public:
	std::vector<unsigned int> indices;
	std::vector<float> vertices;

	virtual void texCoords(int tex_dim);
};

class Triangle : public Shape {
public:
	Triangle(float s1, float s2, float s3, Triangle_type tri);
	void texCoords(int tex_dim);
};

class Rectangle : public Shape {
public:
	Rectangle(float l, float w, Decomp_type type);
	void texCoords(int tex_dim);
};

class RegularPolygon : public Shape {
public:
	RegularPolygon(void);
	RegularPolygon(int n_sides, float circ_radius);
	void texCoords(int tex_dim);
};

class RectangularPrism : public Shape {
public:
	RectangularPrism(float l, float w, float h, Decomp_type type);
	void texCoords(int tex_dim);
};

class Sphere : public Shape {
public:
	Sphere(float r, int layers, int npts);
	//void texCoords(int tex_dim);
};

std::vector<float> createTriangle_SSS(float s1, float s2, float s3);
std::vector<float> createTriangle_SSA(float s1, float s2, float theta);
std::vector<float> createTriangle_SAA(float s, float theta1, float theta2);
std::vector<float> createRectangle(float l, float w);
std::vector<float> createPoint(void);
std::vector<float> shift_z(std::vector<float> vs, float shift);
std::vector<float> createRegularPolygon(int n_sides, float circ_radius);
std::vector<unsigned int> trianglularDecomp_2D(int vs_size);
std::vector<unsigned int> wireFrameDecomp_2D(int vs_size);
std::vector<float> createRectangularPrism(float l, float w, float h);
std::vector<unsigned int> trianglularDecomp_SQ(int vs_size);
std::vector<unsigned int> wireFrameDecomp_SQ(int vs_size);
std::vector<float> createSphere(float r, int layers, int npts);
std::vector<unsigned int> trianglularDecomp_Sphere(int vs_size, int layers, int npts);

#endif