#include "Geometry.h"

#include <iostream>
#include <string>
#include <cmath>

const double pi = 3.14159265358979323846;

//////////
// TODO //
//////////
//
// Compute dual shapes
// Normalize Polygon size
// Allow extrusion offsets
// Allow variable layer size in spheres and layer offsets

/////////////////////////////////
// header function definitions //
/////////////////////////////////

///////////////////
//       b t2    //
//   s1 / \ s3   //
//  t1 a - c     //
///////////////////

std::vector<float> shiftOrigin_make3D(double bx, double by, double cx) {
    // compute centroid
    double ox = 0 * (bx + cx) / 3;
    double oy = 0 * by / 3;
    // create vertex vector
    const size_t num_vs = 3;
    double vs_2D[] = { 0,0,bx,by,cx,0 };
    std::vector<float> vs(3 * num_vs);
    for (size_t i = 0, j = 0; i < 3 * num_vs; i += 3, j += 2) {
        vs[i] = vs_2D[j] - ox;
        vs[i + 1] = vs_2D[j + 1] - oy;
        vs[i + 2] = 0;
    }
    return vs;
}

std::vector<float> createTriangle_SSS(double s1, double s2, double s3) {
    // compute verticies with origin at a (a and cy are simply 0)
    double bx = (s1 * s1 + s2 * s2 - s3 * s3) / (2 * s2);
    double by = std::sqrt(s1 * s1 - bx * bx);
    double cx = s2;
    // move to 3D and shift origin to centroid
    return shiftOrigin_make3D(bx,by,cx);   
}

std::vector<float> createTriangle_SAS(double s1, double theta1_rad, double s2) {
    // compute verticies with origin at a (a and cy are simply 0)
    double bx = s1 * std::cos(theta1_rad);
    double by = s1 * std::sin(theta1_rad);
    double cx = s2;
    // move to 3D and shift origin to centroid
    return shiftOrigin_make3D(bx,by,cx);
}

std::vector<float> createTriangle_ASA(double theta1_rad, double s1, double theta2_rad) {
    // compute verticies with origin at a (a and cy are simply 0)
    double bx = s1 * std::cos(theta1_rad);
    double by = s1 * std::sin(theta1_rad);
    // FIXME: should these angles be switched
    double cx = s1 * std::sin(pi - theta1_rad - theta2_rad) / std::sin(theta2_rad);
    // move to 3D and shift origin to centroid
    return shiftOrigin_make3D(bx,by,cx);
}

std::vector<float> createRectangle(float l, float w) {
    return { -l / 2, -w / 2, 0, l / 2, -w / 2, 0, l / 2, w / 2, 0, -l / 2, w / 2, 0 };
}

inline double rotate_x(double x, double y, double theta_rad) {
    return x * std::cos(theta_rad) - y * std::sin(theta_rad);
}

inline double rotate_y(double x, double y, double theta_rad) {
    return x * std::sin(theta_rad) + y * std::cos(theta_rad);
}

std::vector<float> createRegularPolygon(size_t n_sides, double circ_radius) {
    std::vector<float> vs(3 * n_sides);
    double rot_angle = 2 * pi / n_sides;
    vs[0] = 0;
    vs[1] = circ_radius;
    vs[2] = 0;
    for (size_t i = 3; i < 3 * n_sides; i += 3) {
        vs[i] = rotate_x(vs[i - 3], vs[i - 2], rot_angle);
        vs[i + 1] = rotate_y(vs[i - 3], vs[i - 2], rot_angle);
        vs[i + 2] = 0;
    }
    return vs;
}

std::vector<float> createRegularPolygon_old(size_t n_sides, double circ_radius) {
    std::vector<float> vs(3 * n_sides);
    double int_angle = (n_sides - 2.0) * pi / n_sides;
    double ext_angle = 2 * pi / n_sides;
    double side_len = 2 * circ_radius * std::cos(int_angle / 2);
    double in_radius = circ_radius * std::sin(int_angle / 2);
    vs[0] = -side_len / 2;
    vs[1] = in_radius;
    vs[2] = 0;
    double angle = 0;
    for (size_t i = 3; i < 3 * n_sides; i += 3) {
        vs[i] = vs[i - 3] + side_len * std::cos(angle);
        vs[i + 1] = vs[i - 2] - side_len * std::sin(angle);
        vs[i + 2] = 0;
        angle += ext_angle;
    }
    return vs;
}

std::vector<float> createCone(size_t base_points, double base_radius, float z) {
    std::vector<float> vs = createRegularPolygon(base_points, base_radius);
    std::vector<float> p{ 0,0,z };
    vs.insert(vs.end(), p.begin(), p.end());
    return vs;
}

std::vector<float> createBicone(size_t base_points, double base_radius, float zp, float zm) {
    // should one of the zs be the first vertex for index generation? Might be able to use sphere decomp if so
    std::vector<float> vs = createRegularPolygon(base_points, base_radius);
    std::vector<float> p1{ 0,0,zp };
    std::vector<float> p2{ 0,0,zm };
    vs.insert(vs.end(), p1.begin(), p1.end());
    vs.insert(vs.end(), p2.begin(), p2.end());
    return vs;
}

std::vector<float> createPrism(size_t base_points, double base_radius, double z) {
    std::vector<float> vs_bot = createRegularPolygon(base_points, base_radius);
    std::vector<float> vs_top = createRegularPolygon(base_points, base_radius);
    for (size_t i = 2; i < vs_top.size(); i += 3) {
        vs_bot[i] = -z/2;
        vs_top[i] = z/2;
    }
    vs_bot.insert(vs_bot.end(), vs_top.begin(), vs_top.end());
    return vs_bot;
}

std::vector<float> createSphere(float r, size_t layers, size_t npts) {
    // generate isohedron by allowing intermidiate layers to be offset from each other?
    std::vector<float> vs(3, 0.0);
    vs[2] = r;
    for (size_t i = 1; i <= layers; i++) {
        double z_coord = r * cos(pi * i / (layers + 1));
        std::vector<float> tmp = createRegularPolygon(npts, sqrt(r * r - z_coord * z_coord));
        for (size_t j = 2; j < tmp.size(); j += 3) tmp[j] = z_coord;
        vs.insert(vs.end(), tmp.begin(), tmp.end());
    }
    std::vector<float> tmp{ 0,0,-r };
    vs.insert(vs.end(), tmp.begin(), tmp.end());
    return vs;
}

///////////////////////////////////////

std::vector<float> insertMidpoints_polygon(std::vector<float> vs) {
    size_t n = vs.size();
    std::vector<float> midpoint(3, 0.0);
    for (size_t i = 0; i < n; i+=3) {
        midpoint[0] = (vs[i] + vs[i >= n-3 ? 0 : i + 3]) / 2;
        midpoint[1] = (vs[i + 1] + vs[i >= n-3 ? 1 : i + 4]) / 2;
        //midpoint[2] = (vs[i + 2] + vs[i >= n-3 ? 2 : i + 5]) / 2;
        vs.insert(vs.end(), midpoint.begin(), midpoint.end());
    }
    return vs;
}

std::vector<float> insertMidpoints_prism(std::vector<float> vs) {
    size_t n = vs.size() / 2;
    std::vector<float> midpoint(9, 0.0);
    for (size_t i = 0; i < n; i += 3) {
        midpoint[0] = (vs[i] + vs[i >= n - 3 ? 0 : i + 3]) / 2;
        midpoint[1] = (vs[i + 1] + vs[i >= n - 3 ? 1 : i + 4]) / 2;
        midpoint[2] = (vs[i + 2] + vs[i >= n - 3 ? 2 : i + 5]) / 2;

        midpoint[3] = (vs[i + n] + vs[i >= n - 3 ? n : i + n + 3]) / 2;
        midpoint[4] = (vs[i + n + 1] + vs[i >= n - 3 ? n + 1 : i + n + 4]) / 2;
        midpoint[5] = (vs[i + n + 2] + vs[i >= n - 3 ? n + 2 : i + n + 5]) / 2;

        midpoint[6] = (vs[i] + vs[i + n]) / 2;
        midpoint[7] = (vs[i + 1] + vs[i + n + 1]) / 2;
        midpoint[8] = (vs[i + 2] + vs[i + n + 2]) / 2;

        vs.insert(vs.end(), midpoint.begin(), midpoint.end());
    }
    return vs;
}