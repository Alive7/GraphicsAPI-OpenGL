#ifndef GEOMETRY_HH
#define GEOMETRY_HH

#include <vector>

std::vector<float> createTriangle_SSS(double s1, double s2, double s3);
std::vector<float> createTriangle_SAS(double s1, double theta1_rad, double s2);
std::vector<float> createTriangle_ASA(double theta1_rad, double s1, double theta2_rad);

std::vector<float> createRectangle(float l, float w);

std::vector<float> createRegularPolygon(size_t n_sides, double circ_radius);
std::vector<float> createRegularPolygon_old(size_t n_sides, double circ_radius);

std::vector<float> createCone(size_t base_points, double base_radius, float z);
std::vector<float> createBicone(size_t base_points, double base_radius, float zp, float zm);
std::vector<float> createPrism(size_t base_points, double base_radius, double z);

std::vector<float> createSphere(float r, size_t layers, size_t npts);


std::vector<float> insertMidpoints_polygon(std::vector<float> vs);
std::vector<float> insertMidpoints_prism(std::vector<float> vs);

#endif