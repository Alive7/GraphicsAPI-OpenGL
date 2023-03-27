#ifndef GEOMETRY_HH
#define GEOMETRY_HH

#include <glm/glm/glm.hpp>

#include <vector>

enum class Triangle_type {
	SSS,
	SSA,
	SAA
};

enum class Decomp_type {
	TRI,
	WIR
};

// TODO: 
// conditionally generate colors, textures, normals
// use efficent rectangular prism methods if no textures or normals
// choose appropriate shaders based on populated attributes
// correlation between material properties and material type irrespective of material color
// wire frame decomp for all shapes
// consistent signed area decompositions for gl_FrontFacing
// efficent index packing using shape dependant draw modes

// Abstract Shape class
class Shape {
public:
	// vertex array object - reference to vertex attribute pointers
	unsigned int VAO;
	// element buffer object - reference to index draw order
	unsigned int EBO;
	// vertex buffer object - reference to vertex data in GPU
	unsigned int VBO;
	unsigned int instanceVBO;
	Decomp_type decomp;
	std::vector<unsigned int> indices;
	std::vector<float> vertices;
	std::vector<float> colors;
	std::vector<float> texcoords;
	std::vector<float> normals;

	Shape(void);

	void generateColorData(void);
	virtual void generateTexCoords(void);
	virtual void generateNormals(void) = 0;
	void sendVertexData(void);
	void sendInstancedData(const std::vector<glm::mat4>& models);
	void initalizeInstancing(size_t instances);
	void draw(void);
	void drawInstanced(size_t instances);

	~Shape(void);
};

// abstract 2D shape class
class Shape_2D : public Shape {
public:
	void generateNormals(void);
};

// 2D shape classes
class Triangle : public Shape_2D {
public:
	Triangle(float s1, float s2, float s3, Triangle_type tri);
};

class Rectangle : public Shape_2D {
public:
	Rectangle(float l, float w, Decomp_type type);
};

class RegularPolygon : public Shape_2D {
public:
	RegularPolygon(size_t n_sides, float circ_radius);
};

// 3D shape classes
class RectangularPrism : public Shape {
public:
	RectangularPrism(float l, float w, float h);
	RectangularPrism(float l, float w, float h, Decomp_type type);
	void generateTexCoords(void);
	void generateNormals(void);
};

class Sphere : public Shape {
public:
	Sphere(float r, size_t layers, size_t npts);
	void generateNormals(void);
};

#endif