#include "Geometry.h"

#include <glad/glad.h>

#include <iostream>
#include <string>
#include <cmath>

const double pi = 3.14159265358979323846;

std::vector<float> createTriangle_SSS(float s1, float s2, float s3);
std::vector<float> createTriangle_SSA(float s1, float s2, float theta);
std::vector<float> createTriangle_SAA(float s, float theta1, float theta2);

std::vector<float> createRectangle(float l, float w);

std::vector<float> createRegularPolygon(size_t n_sides, float circ_radius);

std::vector<unsigned int> trianglularDecomp_2D(size_t vs_size);
std::vector<unsigned int> wireFrameDecomp_2D(size_t vs_size);

std::vector<float> createRectangularPrism(float l, float w, float h);
std::vector<unsigned int> trianglularDecomp_RP(size_t vs_size);

std::vector<float> shift_z(std::vector<float> vs, float shift);
std::vector<float> createSphere(float r, size_t layers, size_t npts);
std::vector<unsigned int> trianglularDecomp_Sphere(size_t vs_size, size_t layers, size_t npts);

// probably should be deprecated
std::vector<float> createRectangularPrism_Limited(float l, float w, float h);
std::vector<unsigned int> trianglularDecomp_RP_Limited(size_t vs_size);
std::vector<unsigned int> wireFrameDecomp_RP_Limited(size_t vs_size);

/////////////////////////
/// Shape Definitions ///
/////////////////////////

Shape::Shape(void) {
    decomp = Decomp_type::TRI;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
}

void Shape::generateColorData(void) {
    // fun placeholder. Will want more exact controll over colors eventually.
    size_t num_vs = vertices.size() / 3;
    float step = 1.0 / (num_vs - 1);
    float r = 1.0;
    float g = 0.0;
    float b = 0.0;
    std::vector<float> cols(vertices.size(), 0.0);
    for (size_t i = 0; i < vertices.size(); i++) {
        if (i % 3 == 0) {
            cols[i] = r;
            r -= step;
        }
        else if (i % 3 == 1) {
            cols[i] = g;
            if (vertices.size() % 2) {
                if (i < vertices.size() / 2) g += 2 * step;
                else g -= 2 * step;
            }
            else {
                if (i < vertices.size() / 2 - 3) g += 2 * step;
                else if (i > vertices.size() / 2) g -= 2 * step;
            }
        }
        else {
            cols[i] = b;
            b += step;
        }
    }
    colors = cols;
}

void Shape::generateTexCoords(void) {
    // Mostly meant for 2D shapes
    std::vector<float> tmp(2*vertices.size()/3);
    for (size_t i = 0, j = 0; j < vertices.size(); j++) {
        if (j % 3 != 2) {
            tmp[i] = vertices[j] + .5;
            i++;
        }
    }
    texcoords = tmp;
}

void Shape::sendVertexData(void) {
    glBindVertexArray(VAO);

    // array sizes
    size_t vSize = vertices.size();
    size_t iSize = indices.size();
    size_t cSize = colors.size();
    size_t tSize = texcoords.size();
    size_t nSize = normals.size();

    // pack vertex data
    if (vSize) {
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * (vSize + cSize + tSize + nSize), 0, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices[0]) * vSize, vertices.data());
    }
    if (cSize && vSize) {
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vSize, sizeof(colors[0]) * cSize, colors.data());
    }
    if (tSize && vSize) {
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * (vSize + cSize), sizeof(texcoords[0]) * tSize, texcoords.data());
    }
    if (nSize && vSize) {
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * (vSize + cSize + tSize), sizeof(normals[0]) * nSize, normals.data());
    }
    // pack index data
    if (iSize && vSize) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * iSize, indices.data(), GL_STATIC_DRAW);
    }
    // construct vertex attribute pointer
    if (vSize) {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(vertices[0]), (void*)0);
        glEnableVertexAttribArray(0);

        // potential for junk data if colors, textures, or normals aren't specified
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(vertices[0]), (void*)(sizeof(vertices[0]) * vSize));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(vertices[0]), (void*)(sizeof(vertices[0]) * (vSize + cSize)));
        glEnableVertexAttribArray(2);
        
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(vertices[0]), (void*)(sizeof(vertices[0]) * (vSize + cSize + tSize)));
        glEnableVertexAttribArray(3);
    }

    glBindVertexArray(0);
}

void Shape::initalizeInstancing(size_t instances) {
    glBindVertexArray(VAO);
    glGenBuffers(1, &instanceVBO);

    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * instances, 0, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
    glEnableVertexAttribArray(4);
    glVertexAttribDivisor(4, 1);

    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(float) * 4));
    glEnableVertexAttribArray(5);
    glVertexAttribDivisor(5, 1);

    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(float) * 8));
    glEnableVertexAttribArray(6);
    glVertexAttribDivisor(6, 1);

    glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(float) * 12));
    glEnableVertexAttribArray(7);
    glVertexAttribDivisor(7, 1);

    glBindVertexArray(0);
}

void Shape::sendInstancedData(const std::vector<glm::mat4>& models) {
    glBindVertexArray(VAO);
    // might be faster to use glMapBuffer
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(models[0]) * models.size(), models.data(), GL_DYNAMIC_DRAW);

    glBindVertexArray(0);
}

void Shape::draw(void) {
    glBindVertexArray(VAO);
    // some shapes could achive more effivent indice packing with strip, loop, or fan options
    if (decomp == Decomp_type::TRI) glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    else if (decomp == Decomp_type::WIR) glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);
    else std::cout << "Decomp type not implemented.\n";
    glBindVertexArray(0);
}

void Shape::drawInstanced(size_t instances) {
    glBindVertexArray(VAO);
    // some shapes could achive more effivent indice packing with strip, loop, or fan options
    if (decomp == Decomp_type::TRI) glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0, instances);
    else if (decomp == Decomp_type::WIR) glDrawElementsInstanced(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0, instances);
    else std::cout << "Decomp type not implemented.\n";
    glBindVertexArray(0);
}

Shape::~Shape(void) {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &instanceVBO);
}

////////////////////////////
/// 2D Shape Definitions ///
////////////////////////////

void Shape_2D::generateNormals(void) {
    // 2D shapes defined in xy plane so normal is +-z
    std::vector<float> norms(vertices.size(), 0.0);
    for (size_t i = 2; i < vertices.size(); i += 3) {
        norms[i] = 1.0;
    }
    normals = norms;
}

Triangle::Triangle(float s1, float s2, float s3, Triangle_type tri) {
    if (tri == Triangle_type::SSS) vertices = createTriangle_SSS(s1, s2, s3);
    else if (tri == Triangle_type::SSA) vertices = createTriangle_SSA(s1, s2, s3);
    else if (tri == Triangle_type::SAA) vertices = createTriangle_SAA(s1, s2, s3);
    else std::cout << "Triangle type not implemented.\n";
    indices = trianglularDecomp_2D(vertices.size());
    generateColorData();
    generateTexCoords();
    generateNormals();
    sendVertexData();
}

Rectangle::Rectangle(float l, float w, Decomp_type type) {
    decomp = type;
    vertices = createRectangle(l, w);
    if (type == Decomp_type::TRI) indices = trianglularDecomp_2D(vertices.size());
    else if (type == Decomp_type::WIR) indices = wireFrameDecomp_2D(vertices.size());
    else std::cout << "Decomp type not implemented.\n";
    generateColorData();
    generateTexCoords();
    generateNormals();
    sendVertexData();
}

RegularPolygon::RegularPolygon(size_t n_sides, float circ_radius) {
    vertices = createRegularPolygon(n_sides, circ_radius);
    indices = trianglularDecomp_2D(vertices.size());
    generateColorData();
    generateTexCoords();
    generateNormals();
    sendVertexData();
}

////////////////////////////
/// 3D Shape Definitions ///
////////////////////////////

RectangularPrism::RectangularPrism(float l, float w, float h) {
    vertices = createRectangularPrism(l, w, h);
    indices = trianglularDecomp_RP(vertices.size());
    generateColorData();
    generateTexCoords();
    generateNormals();
    sendVertexData();
}

RectangularPrism::RectangularPrism(float l, float w, float h, Decomp_type type) {
    // legacy constructor for wireframe decomp compatibility
    // uses more efficent data packing at the cost of ill defined textures and normals
    decomp = type;
    vertices = createRectangularPrism_Limited(l, w, h);
    if (type == Decomp_type::TRI) indices = trianglularDecomp_RP_Limited(vertices.size());
    else if (type == Decomp_type::WIR) indices = wireFrameDecomp_RP_Limited(vertices.size());
    else std::cout << "Decomp type not implemented.\n";
    generateColorData();
    sendVertexData();
}

void RectangularPrism::generateTexCoords(void) {
    // might want the texture to scale independantly from the prism size
    std::vector<float> tmp(2 * vertices.size() / 3);
    for (size_t i = 0; i < tmp.size(); i+=8) {
        tmp[i] = 0;
        tmp[i + 1] = 0;

        tmp[i + 2] = 1;
        tmp[i + 3] = 0;

        tmp[i + 4] = 1;
        tmp[i + 5] = 1;

        tmp[i + 6] = 0;
        tmp[i + 7] = 1;
    }
    texcoords = tmp;
}

void RectangularPrism::generateNormals(void) {
    std::vector<float> tmp(vertices.size(), 0.0);
    size_t face_size = 4 * 3;
    size_t offset = 2;
    // first two faces along z, then y, then x
    // alternating normals with the first positive
    for (size_t face = 0; face < 6; face++) {
        for (size_t i = face * face_size; i < (face + 1) * face_size; i += 3) {
            if (face % 2) tmp[i + offset] = -1.0;
            else tmp[i + offset] = 1.0;
        }
        if (face % 2) offset--;
    }
    normals = tmp;
}

Sphere::Sphere(float r, size_t layers, size_t npts) {
    vertices = createSphere(r, layers, npts);
    indices = trianglularDecomp_Sphere(vertices.size(), layers, npts);
    generateColorData();
    generateTexCoords();
    generateNormals();
    sendVertexData();
}

void Sphere::generateNormals(void) {
    std::vector<float> norms(vertices.size(), 0.0);
    for (size_t i = 0; i < vertices.size(); i += 3) {
        float norm = sqrt(vertices[i] * vertices[i] + vertices[i + 1] * vertices[i + 1] + vertices[i + 2] * vertices[i + 2]);
        norms[i] = vertices[i] / norm;
        norms[i + 1] = vertices[i + 1] / norm;
        norms[i + 2] = vertices[i + 2] / norm;
    }
    normals = norms;
}

///////////////////////////////
/// Geometry Helper Methods ///
///////////////////////////////

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
    size_t num_vs = 3;
    std::vector<float> vs(num_vs * 3);
    for (size_t i = 0; i < 3 * num_vs; i++) {
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
    size_t num_vs = 3;
    std::vector<float> vs(num_vs * 3);
    for (size_t i = 0; i < 3 * num_vs; i++) {
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
    size_t num_vs = 3;
    std::vector<float> vs(num_vs * 3);
    for (size_t i = 0; i < 3 * num_vs; i++) {
        if (i % 3 == 2) vs[i] = 0;
        else {
            if ((i % 3) % 2 == 0) vs[i] = vs_raw[i - i / 3] - o1;
            else vs[i] = vs_raw[i - i / 3] - o2;
        }
    }
    return vs;
}

std::vector<float> createRectangle(float l, float w) {
    size_t num_vs = 4;
    std::vector<float> vs(num_vs * 3);
    for (size_t i = 0; i < 3 * num_vs; i += 3) {
        vs[i] = std::pow(-1, i / 2 + 1) * l / 2; // - + + -
        vs[i + 1] = std::pow(-1, (i + 1) / 2 + 1) * w / 2; // - - + +
        vs[i + 2] = 0;
    }
    return vs;
}

std::vector<float> createRegularPolygon(size_t n_sides, float circ_radius) {
    size_t num_vs = n_sides;
    std::vector<float> vs(num_vs * 3);
    float int_angle = (n_sides - 2.0) * pi / n_sides;
    float ext_angle = 2 * pi / n_sides;
    float side_len = 2 * circ_radius * std::cos(int_angle / 2);
    float in_radius = circ_radius * std::sin(int_angle / 2);
    vs[0] = -side_len / 2;
    vs[1] = in_radius;
    vs[2] = 0;
    float angle = 0;
    for (size_t i = 3; i < 3 * n_sides; i += 3) {
        vs[i] = vs[i - 3] + side_len * std::cos(angle);
        vs[i + 1] = vs[i - 2] - side_len * std::sin(angle);
        vs[i + 2] = 0;
        angle += ext_angle;
    }
    return vs;
}

std::vector<unsigned int> trianglularDecomp_2D(size_t vs_size) {
    std::vector<unsigned int> indices(vs_size - 6);
    for (size_t i = 0; i < indices.size(); i++) {
        if (i % 3 == 0) indices[i] = 0;
        else indices[i] = i - 2 * (i / 3);
    }
    return indices;
}

std::vector<unsigned int> wireFrameDecomp_2D(size_t vs_size) {
    size_t dim = 3;
    size_t n_vs = vs_size / dim;
    std::vector<unsigned int> indices(n_vs);
    for (size_t i = 0; i < n_vs; i++) indices[i] = i;
    //for (std::vector<unsigned int>::iterator itr = indices.begin(); itr != indices.end(); itr++) std::cout << *itr << std::endl;
    return indices;
}

// 3D graphics

std::vector<float> createRectangularPrism(float l, float w, float h) {
    // face order: back, front, top, bottom, right, left
    // create faces
    std::vector<float> z_faces = createRectangle(l, w);
    std::vector<float> y_faces = createRectangle(l, h);
    std::vector<float> x_faces = createRectangle(h, w);
    // rearrange x and y data
    for (size_t i = 0; i < y_faces.size(); i += 3) {
        y_faces[i + 2] = y_faces[i + 1];
        y_faces[i + 1] = w / 2;
    }
    for (size_t i = 0; i < x_faces.size(); i += 3) {
        x_faces[i + 2] = x_faces[i];
        x_faces[i] = l / 2;
    }
    // construct vertex vector
    size_t size = z_faces.size();
    std::vector<float> vs(6 * size, 0.0);
    size_t vs_index = 0;
    // copy the z faces into vs
    for (size_t i = 0; i < size; i++, vs_index++) {
        vs[vs_index] = z_faces[i];
        vs[vs_index+size] = z_faces[i];
        if (i % 3 == 2) {
            vs[vs_index] = z_faces[i] - h/2;
            vs[vs_index + size] = z_faces[i] + h/2;
        }

    }
    vs_index += size;
    // copy the y faces
    for (size_t i = 0; i < size; i++, vs_index++) {
        vs[vs_index] = y_faces[i];
        vs[vs_index + size] = y_faces[i];
        if (i % 3 == 1) vs[vs_index + size] = y_faces[i] - w;

    }
    vs_index += size;
    // copy the x faces
    for (size_t i = 0; i < size; i++, vs_index++) {
        vs[vs_index] = x_faces[i];
        vs[vs_index + size] = x_faces[i];
        if (i % 3 == 0) vs[vs_index + size] = x_faces[i] - l;

    }
    return vs;
}

std::vector<unsigned int> trianglularDecomp_RP(size_t vs_size) {
    size_t dim = 3;
    size_t n_vs = vs_size / dim;
    // 6 indices a face, 6 faces
    std::vector<unsigned int> indices(36);
    // index the first face
    for (size_t i = 0; i < 3; i++) {
        indices[i] = i;
        indices[i + 3] = i + 1;
        if (i == 0) indices[i + 3] = i;
    }
    // the indices for every other face are incremented by 4
    for (size_t i = 6; i < indices.size(); i++) {
        indices[i] = indices[i - 6] + 4;
    }
    // the back, top, and right faces must have their winding numbers reversed for face culling
    for (size_t i = 0; i < indices.size(); i+=12) {
        unsigned int tmp = indices[i + 1];
        indices[i + 1] = indices[i + 2];
        indices[i + 2] = tmp;

        tmp = indices[i + 4];
        indices[i + 4] = indices[i + 5];
        indices[i + 5] = tmp;
    }
    return indices;
}

std::vector<float> shift_z(std::vector<float> vs, float shift) {
    for (size_t i = 2; i < vs.size(); i+=3) vs[i] = shift;
    return vs;
}

std::vector<float> createSphere(float r, size_t layers, size_t npts) {
    std::vector<float> vs(3, 0.0);
    vs[2] = r;
    for (size_t i = 1; i <= layers; i++) {
        float z_coord = r * cos(pi * i / (layers + 1));
        std::vector<float> tmp = createRegularPolygon(npts, sqrt(r*r-z_coord*z_coord));
        tmp = shift_z(tmp, z_coord);
        vs.insert(vs.end(), tmp.begin(), tmp.end());
    }
    std::vector<float> tmp(3, 0.0);
    tmp = shift_z(tmp, -r);
    vs.insert(vs.end(), tmp.begin(), tmp.end());
    return vs;
}

std::vector<unsigned int> trianglularDecomp_Sphere(size_t vs_size, size_t layers, size_t npts) {
    size_t num_vs = npts * layers + 2; // vs_size / 3; both same
    size_t i_max = 3 * 2 * npts * layers;
    std::vector<unsigned int> indices(i_max);
    int offset = 1;
    for (size_t i = 0, v = 0; i < 3 * npts; i += 3) {
        indices[i] = v;
        indices[i + 2] = offset;
        if (offset % npts) indices[i + 1] = offset + 1;
        else indices[i + 1] = v + 1;
        offset++;
    }
    for (size_t i = 3 * npts, v = 1; i < i_max - 3 * npts; i += 6, v++) {
        offset = v % npts ? offset : v + 1;
        indices[i] = v;
        indices[i + 2] = v + npts;
        if (v % npts) indices[i + 1] = v + npts + 1;
        else indices[i + 1] = offset;

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
    for (size_t i = i_max - 3 * npts, v = num_vs - 1 - npts; i < i_max; i += 3, v++) {
        indices[i] = v;
        if ((v+1)-(num_vs - 1)) indices[i + 1] = v+1;
        else indices[i + 1] = offset;
        indices[i + 2] = num_vs - 1;
    }
    return indices;
}

std::vector<float> createRectangularPrism_Limited(float l, float w, float h) {
    std::vector<float> vs = createRectangle(l, w);
    size_t size = vs.size();
    vs.reserve(2 * size);
    for (size_t i = 0; i < size; i++) {
        if (i % 3 == 2) {
            vs[i] = -h / 2;
            vs.emplace_back(h / 2);
        }
        else vs.emplace_back(vs[i]);
    }
    return vs;
}

std::vector<unsigned int> trianglularDecomp_RP_Limited(size_t vs_size) {
    size_t dim = 3;
    size_t n_vs = vs_size / dim;
    std::vector<unsigned int> indices(9 * 4);
    for (size_t i = 0, v = 0; v < n_vs / 2; i += 9, v++) {
        indices[i] = (v + 4 * (v % 2)) % 8;
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

std::vector<unsigned int> wireFrameDecomp_RP_Limited(size_t vs_size) {
    size_t dim = 3;
    size_t n_vs = vs_size / dim;
    std::vector<unsigned int> indices(24);
    for (size_t i = 0, v = 0; i < 2 * n_vs; i += 2, v++) {
        indices[i] = v / 2;
        if (i % 4 == 0) indices[i + 1] = (v / 2 + 1) % 4;
        if (i % 4 == 2) indices[i + 1] = v / 2 + 4;

    }
    for (size_t i = 2 * n_vs, v = 4; i < 24; i += 2, v++) {
        indices[i] = v;
        indices[i + 1] = (v + 1) % 4 + 4;
    }
    //for (std::vector<unsigned int>::iterator itr = indices.begin(); itr != indices.end(); itr++) std::cout << *itr << std::endl;
    return indices;
}