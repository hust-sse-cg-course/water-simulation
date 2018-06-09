#ifndef __CUBE_H__
#define __CUBE_H__
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Shader.h"

class Cube {
public:
	Cube();
	Cube(int details);
    void draw(Shader *shader);
private:
    unsigned int VAO, VBO;
};

#endif