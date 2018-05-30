#ifndef __SHADER_H__
#define __SHADER_H__

#include <glad/glad.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <streambuf>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
public:

    Shader();
  Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
  Shader(const GLchar* vertexPath, const GLchar* geometryPath, const GLchar* fragmentPath);

  inline void use() {
    glUseProgram(ID);
  }
  inline unsigned int getShaderID() {
    return ID;
  }
  inline void setBool(const std::string &name, bool value) const {
    glUniform1i(getLocation(ID, name.c_str()), (int)value);
  }
  inline void setInt(const std::string &name, int value) const {
    glUniform1i(getLocation(ID, name.c_str()), value);
  }
  inline void setFloat(const std::string &name, float value) const {
    glUniform1f(getLocation(ID, name.c_str()), value);
  }
  inline void setMat4(const std::string &name, glm::mat4 value) const {
    glUniformMatrix4fv(getLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
  }
  inline void setVec3(const std::string &name, float x, float y, float z) const {
    float v[3] = { x, y, z };
    glUniform3fv(getLocation(ID, name.c_str()), 1, v);
  }
  inline void setVec3(const std::string &name, glm::vec3 value) const {
    glUniform3fv(getLocation(ID, name.c_str()), 1, glm::value_ptr(value));
  }
  inline void setVec2(const std::string &name, float x, float y) const {
    float v[2] = { x, y };
    glUniform2fv(getLocation(ID, name.c_str()), 1, v);
  }
  inline void setVec2(const std::string &name, glm::vec2 value) const {
    glUniform2fv(getLocation(ID, name.c_str()), 1, glm::value_ptr(value));
  }

  inline int getLocation(unsigned ID, std::string name) const {
    return glGetUniformLocation(ID, name.c_str());
  }

private:
  unsigned int ID;
};



#endif // !__SHADER_H__
