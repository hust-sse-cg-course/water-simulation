#ifndef __PLANE_H__
#define __PLANE_H__
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Vertex.h"
#include "Shader.h"

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