#pragma once
#ifndef __VERTEX_H__
#define __VERTEX_H__
#include <glm/glm.hpp>
struct Vertex {
    glm::vec3 positions;
    glm::vec2 coords;
    glm::vec3 normals;
};

#endif // !__VERTEX_H__