#ifndef MESH_H
#define MESH_H

#include "Shader.h"

#include <glm/glm/glm.hpp>

#include <string>
#include <vector>

#define MAX_BONE_INFLUENCE 4

struct VertexData {
    // keep a vector of each individually
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec3 Tangent;
    glm::vec3 Bitangent;
    //bone indexes which will influence this vertex
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    //weights from each bone
    float m_Weights[MAX_BONE_INFLUENCE];
};

struct TextureData {
    // this should just be a texture
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh {
public:
    // mesh Data
    unsigned int VAO;
    std::vector<unsigned int> indices;
    std::vector<VertexData>   vertices;
    std::vector<TextureData>  textures;

    Mesh(std::vector<VertexData> vs, std::vector<unsigned int> inds, std::vector<TextureData> texs);

    void Draw(Shader& shader);
private:
    unsigned int VBO, EBO;
    void setupMesh();
};
#endif