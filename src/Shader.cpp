#include "Shader.h"

Shader::Shader() : Shader("shader/simple.vert", "shader/simple.frag") 
{
}

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath) :
  Shader(vertexPath, NULL, fragmentPath)
{
}

Shader::Shader(const GLchar* vertexPath, const GLchar* geometryPath, const GLchar* fragmentPath) {
  std::string vertexCode;
  std::string geometryCode;
  std::string fragmentCode;
  std::ifstream vertFile;
  std::ifstream geomFile;
  std::ifstream fragFile;

  vertFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fragFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  try {
    vertFile.open(vertexPath);
    fragFile.open(fragmentPath);
    std::stringstream vStream, fStream;
    vStream << vertFile.rdbuf();
    fStream << fragFile.rdbuf();
    vertFile.close();
    fragFile.close();
    vertexCode = vStream.str();
    fragmentCode = fStream.str();
  }
  catch (std::ifstream::failure e) {
    std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
  }

  if (geometryPath) {
    geomFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
      geomFile.open(geometryPath);
      std::stringstream gStream;
      gStream << geomFile.rdbuf();
      geomFile.close();
      geometryCode = gStream.str();
    }
    catch (std::ifstream::failure e) {
    std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
    }
  }

  const char* vCode = vertexCode.c_str();
  const char* fCode = fragmentCode.c_str();

  unsigned int vertexShader, fragmentShader, geometryShader;
  int success;
  char infoLog[512];

  // Compile vertex shader
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vCode, NULL);
  glCompileShader(vertexShader);

  // Check if the shader compile successfully
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  // Compile fragment shader
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fCode, NULL);
  glCompileShader(fragmentShader);

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  if (geometryPath) {
    const char* gCode = geometryCode.c_str();
    geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(geometryShader, 1, &gCode, NULL);
    glCompileShader(geometryShader);

    glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(geometryShader, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
  }

  // Create and link a shader program
  ID = glCreateProgram();
  glAttachShader(ID, vertexShader);
  glAttachShader(ID, fragmentShader);
  if (geometryPath) {
    glAttachShader(ID, geometryShader);
  }
  glLinkProgram(ID);

  glGetProgramiv(ID, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(ID, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}