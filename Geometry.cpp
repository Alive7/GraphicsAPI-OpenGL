#include "Geometry.h"

const double pi = 3.14159265358979323846;

void Shape::texCoords(int tex_dim) {
    for (int i = 0; i < indices.size(); i++) std::cout << indices[i] << std::endl;
}

Triangle::Triangle(float s1, float s2, float s3, Triangle_type tri) {
    if (tri == Triangle_type::SSS) vertices = createTriangle_SSS(s1, s2, s3);
    else if (tri == Triangle_type::SSA) vertices = createTriangle_SSA(s1, s2, s3);
    else vertices = createTriangle_SAA(s1, s2, s3);
    indices = trianglularDecomp_2D(vertices.size());
}

void Triangle::texCoords(int tex_dim) {
    // insert two tex coords
    std::vector<float> vs(5 * vertices.size() / 3);
    for (int i = 0; i < vs.size(); i++) {
        std::cout << i << "\t" << i % 5 << "\t" << i / 5 << std::endl;
        if (i % 5 < 3) vs[i] = vertices[i - 2 * (i / 5)];
        else vs[i] = vs[i - 3]/2 + .5;
    }
    vertices.swap(vs);
    //for (std::vector<float>::iterator itr = vertices.begin(); itr != vertices.end(); itr++) std::cout << *itr << std::endl;
}

Rectangle::Rectangle(float l, float w, Decomp_type type) {
    vertices = createRectangle(l, w);
    if (type == Decomp_type::TRI) indices = trianglularDecomp_2D(vertices.size());
    else if (type == Decomp_type::WIR) indices = wireFrameDecomp_2D(vertices.size());
}

void Rectangle::texCoords(int tex_dim) {
    for (int i = 0; i < vertices.size(); i++) std::cout << vertices[i] << std::endl;
}

RegularPolygon::RegularPolygon(void) {}

RegularPolygon::RegularPolygon(int n_sides, float circ_radius) {
    vertices = createRegularPolygon(n_sides, circ_radius);
    indices = trianglularDecomp_2D(vertices.size());
}

void RegularPolygon::texCoords(int tex_dim) {
    for (int i = 0; i < indices.size(); i++) std::cout << indices[i] << std::endl;
}

RectangularPrism::RectangularPrism(float l, float w, float h, Decomp_type type) {
    vertices = createRectangularPrism(l, w, h);
    if (type == Decomp_type::TRI) indices = trianglularDecomp_SQ(vertices.size());
    else if (type == Decomp_type::WIR) indices = wireFrameDecomp_SQ(vertices.size());
}

void RectangularPrism::texCoords(int tex_dim) {
    for (int i = 0; i < indices.size(); i++) std::cout << indices[i] << std::endl;
}

Sphere::Sphere(float r, int layers, int npts) {
    vertices = createSphere(r, layers, npts);
    indices = trianglularDecomp_Sphere(vertices.size(), layers, npts);
}

std::vector<float> createTriangle_SSS(float s1, float s2, float s3) {
    // origin at centroid
    float a1 = 0;
    float a2 = 0;
    float b1 = (s1 * s1 + s2 * s2 - s3 * s3) / (2 * s2);
    float b2 = std::sqrt(s1 * s1 - b1 * b1);
    float c1 = s2;
    float c2 = 0;
    float o1 = (b1 + c1) / 3;
    float o2 = b2 / 3;
    float vs_raw[] = { a1,a2,b1,b2,c1,c2 };
    int num_vs = 3;
    std::vector<float> vs(num_vs * 3);
    for (int i = 0; i < 3 * num_vs; i++) {
        if (i % 3 == 2) vs[i] = 0;
        else {
            if ((i % 3) % 2 == 0) vs[i] = vs_raw[i - i / 3] - o1;
            else vs[i] = vs_raw[i - i / 3] - o2;
        }
    }
    return vs;
}

std::vector<float> createTriangle_SSA(float s1, float s2, float theta) {
    // origin at centroid
    float a1 = 0;
    float a2 = 0;
    float b1 = s1 * std::cos(theta);
    float b2 = s1 * std::sin(theta);
    float c1 = s2;
    float c2 = 0;
    float o1 = (b1 + c1) / 3;
    float o2 = b2 / 3;
    float vs_raw[] = { a1,a2,b1,b2,c1,c2 };
    int num_vs = 3;
    std::vector<float> vs(num_vs * 3);
    for (int i = 0; i < 3 * num_vs; i++) {
        if (i % 3 == 2) vs[i] = 0;
        else {
            if ((i % 3) % 2 == 0) vs[i] = vs_raw[i - i / 3] - o1;
            else vs[i] = vs_raw[i - i / 3] - o2;
        }
    }
    return vs;
}

std::vector<float> createTriangle_SAA(float s, float theta1, float theta2) {
    // origin at centroid
    float a1 = 0;
    float a2 = 0;
    float b1 = s * std::cos(theta1);
    float b2 = s * std::sin(theta1);
    float c1 = s * std::sin(pi - theta1 - theta2) / std::sin(theta2);
    float c2 = 0;
    float o1 = (b1 + c1) / 3;
    float o2 = b2 / 3;
    float vs_raw[] = { a1,a2,b1,b2,c1,c2 };
    int num_vs = 3;
    std::vector<float> vs(num_vs * 3);
    for (int i = 0; i < 3 * num_vs; i++) {
        if (i % 3 == 2) vs[i] = 0;
        else {
            if ((i % 3) % 2 == 0) vs[i] = vs_raw[i - i / 3] - o1;
            else vs[i] = vs_raw[i - i / 3] - o2;
        }
    }
    return vs;
}


std::vector<float> createRectangle(float l, float w) {
    int num_vs = 4;
    std::vector<float> vs(num_vs * 3);
    for (int i = 0; i < 3 * num_vs; i += 3) {
        vs[i] = std::pow(-1, i / 2 + 1) * l / 2; // - + + -
        vs[i + 1] = std::pow(-1, (i + 1) / 2 + 1) * w / 2; // - - + +
        vs[i + 2] = 0;
    }
    return vs;
}

std::vector<float> createRegularPolygon(int n_sides, float circ_radius) {
    int num_vs = n_sides;
    std::vector<float> vs(num_vs * 3);
    float int_angle = (n_sides - 2.0) * pi / n_sides;
    float ext_angle = 2 * pi / n_sides;
    float side_len = 2 * circ_radius * std::cos(int_angle / 2);
    float in_radius = circ_radius * std::sin(int_angle / 2);
    vs[0] = -side_len / 2;
    vs[1] = in_radius;
    vs[2] = 0;
    float angle = 0;
    for (int i = 3; i < 3 * n_sides; i += 3) {
        vs[i] = vs[i - 3] + side_len * std::cos(angle);
        vs[i + 1] = vs[i - 2] - side_len * std::sin(angle);
        vs[i + 2] = 0;
        angle += ext_angle;
    }
    return vs;
}


std::vector<unsigned int> trianglularDecomp_2D(int vs_size) {
    std::vector<unsigned int> indices(vs_size - 6);
    for (int i = 0; i < indices.size(); i++) {
        if (i % 3 == 0) indices[i] = 0;
        else indices[i] = i - 2 * (i / 3);
    }
    return indices;
}

std::vector<unsigned int> wireFrameDecomp_2D(int vs_size) {
    int dim = 3;
    int n_vs = vs_size / dim;
    std::vector<unsigned int> indices(n_vs);
    for (int i = 0; i < n_vs; i++) indices[i] = i;
    //for (std::vector<unsigned int>::iterator itr = indices.begin(); itr != indices.end(); itr++) std::cout << *itr << std::endl;
    return indices;
}

// 3D graphics

std::vector<float> createRectangularPrism(float l, float w, float h) {
    std::vector<float> vs = createRectangle(l, w);
    int size = vs.size();
    vs.reserve(2 * size);
    for (int i = 0; i < size; i++) {
        if (i % 3 == 2) {
            vs[i] = -h / 2;
            vs.emplace_back(h / 2);
        }
        else vs.emplace_back(vs[i]);
    }
    return vs;
}

std::vector<unsigned int> trianglularDecomp_SQ(int vs_size) {
    int dim = 3;
    int n_vs = vs_size / dim;
    std::vector<unsigned int> indices(9 * 4);
    for (int i = 0, v = 0; v < n_vs / 2; i += 9, v++) {
        indices[i]     = (v + 4 * (v % 2)) % 8;
        indices[i + 1] = ((v + 1) % 4 + 4 * (v % 2)) % 8;
        indices[i + 2] = ((v + 3) % 4 + 4 * (v % 2)) % 8;

        indices[i + 3] = (v + 4 * (v % 2)) % 8;
        indices[i + 4] = ((v + 3) % 4 + 4 * (v % 2)) % 8;
        indices[i + 5] = ((v + 4) % 8 + 4 * (v % 2)) % 8;

        indices[i + 6] = (v + 4 * (v % 2)) % 8;
        indices[i + 7] = ((v + 1) % 4 + 4 * (v % 2)) % 8;
        indices[i + 8] = ((v + 4) % 8 + 4 * (v % 2)) % 8;
    }
    //for (std::vector<unsigned int>::iterator itr = indices.begin(); itr != indices.end(); itr++) std::cout << *itr << std::endl;
    return indices;
}

std::vector<unsigned int> wireFrameDecomp_SQ(int vs_size) {
    int dim = 3;
    int n_vs = vs_size / dim;
    std::vector<unsigned int> indices(24);
    for (int i = 0,v = 0; i < 2*n_vs; i+=2, v++) {
        indices[i] = v/2;
        if (i % 4 == 0) indices[i + 1] = (v / 2 + 1)%4;
        if (i % 4 == 2) indices[i + 1] = v / 2 + 4;

    }
    for (int i = 2 * n_vs, v = 4; i < 24; i+=2, v++) {
        indices[i] = v;
        indices[i + 1] = (v + 1) % 4 + 4;
    }
    //for (std::vector<unsigned int>::iterator itr = indices.begin(); itr != indices.end(); itr++) std::cout << *itr << std::endl;
    return indices;
}

std::vector<float> createPoint(void) {
    std::vector<float> vs(3,0.0);
    return vs;
}

std::vector<float> shift_z(std::vector<float> vs, float shift) {
    for (int i = 2; i < vs.size(); i+=3) vs[i] = shift;
    return vs;
}

std::vector<float> createSphere(float r, int layers, int npts) {
    std::vector<float> vs = createPoint();
    vs[2] = r;
    for (int i = 1; i <= layers; i++) {
        float z_coord = r * cos(pi * i / (layers + 1));
        std::vector<float> tmp = createRegularPolygon(npts, sqrt(r*r-z_coord*z_coord));
        tmp = shift_z(tmp, z_coord);
        vs.insert(vs.end(), tmp.begin(), tmp.end());
    }
    std::vector<float> tmp = createPoint();
    tmp = shift_z(tmp, -r);
    vs.insert(vs.end(), tmp.begin(), tmp.end());
    return vs;
}

std::vector<unsigned int> trianglularDecomp_Sphere(int vs_size, int layers, int npts) {
    int num_vs = npts * layers + 2; // vs_size / 3; both same
    int i_max = 3 * 2 * npts * layers;
    std::vector<unsigned int> indices(i_max);
    int offset = 1;
    for (int i = 0, v = 0; i < 3 * npts; i += 3) {
        indices[i] = v;
        indices[i + 1] = offset;
        if (offset % npts) indices[i + 2] = offset + 1;
        else indices[i + 2] = v + 1;
        offset++;
    }
    for (int i = 3 * npts, v = 1; i < i_max - 3 * npts; i += 6, v++) {
        offset = v % npts ? offset : v + 1;
        indices[i] = v;
        indices[i + 1] = v + npts;
        if (v % npts) indices[i + 2] = v + npts + 1;
        else indices[i + 2] = offset;

        indices[i + 3] = v;
        if (v % npts) {
            indices[i + 4] = v + 1;
            indices[i + 5] = v + npts + 1;
        }
        else {
            indices[i + 4] = offset - npts;
            indices[i + 5] = offset;
        }
    }
    for (int i = i_max - 3 * npts, v = num_vs - 1 - npts; i < i_max; i += 3, v++) {
        indices[i] = v;
        if ((v+1)-(num_vs - 1)) indices[i + 1] = v+1;
        else indices[i + 1] = offset;
        indices[i + 2] = num_vs - 1;
    }
    return indices;
}