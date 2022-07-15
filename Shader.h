#ifndef SHADER_HH
#define SHADER_HH

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include <string>

class Shader {
public:
    unsigned int ID;

    Shader(const std::string vertexPath, const std::string fragmentPath);
    void set(void);
    // utility uniform functions
    void setUniform_Bool(const std::string& name, bool value) const;
    void setUniform_Int(const std::string& name, int value) const;
    void setUniform_Float(const std::string& name, float value) const;
    void setUniform_Vec4(const std::string& name, glm::vec4 value) const;
    void setUniform_Mat4(const std::string& name, glm::mat4 value) const;

    ~Shader(void);

private:
    void checkCompileErrors(unsigned int shader, std::string type);
};

#endif