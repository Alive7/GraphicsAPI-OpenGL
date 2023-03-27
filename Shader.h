#ifndef SHADER_HH
#define SHADER_HH

#include <glm/glm/glm.hpp>

#include <string>

class Shader {
public:
    unsigned int ID;

    Shader(const std::string vertexPath, const std::string geometryPath, const std::string fragmentPath);

    void set(void);
    // utility uniform functions
    void setUniform_Bool(const char* name, bool value);
    void setUniform_Int(const char* name, int value);
    void setUniform_Float(const char* name, float value);
    void setUniform_Vec4(const char* name, glm::vec4 value);
    void setUniform_Vec3(const char* name, glm::vec3 value);
    void setUniform_Mat4(const char* name, glm::mat4 value);
    void setUniform_Mat3(const char* name, glm::mat3 value);

    ~Shader(void);

private:
    void checkCompileErrors(unsigned int shader, std::string type);
};

#endif