#ifndef __PLANE_H__
#define __PLANE_H__
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Shader.h"

struct Vertex {
    glm::vec3 positions;
    glm::vec2 coords;
    glm::vec3 normals;
};

class Plane {
public:
    Plane();
    Plane(int details);

    void draw(Shader *shader);
private:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    unsigned int VAO, VBO, EBO;
};

#endif