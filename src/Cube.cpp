#include "Cube.h"

using namespace std;
using namespace glm;

Cube::Cube() : Cube(1) {}

float cubeVertices[] = {
	// positions
	-0.5f, -0.5f, -0.5f,
	0.5f,  0.5f, -0.5f, 
	0.5f, -0.5f, -0.5f, 
	0.5f,  0.5f, -0.5f, 
	-0.5f, -0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,

	-0.5f, -0.5f,  0.5f,
	0.5f, -0.5f,  0.5f, 
	0.5f,  0.5f,  0.5f, 
	0.5f,  0.5f,  0.5f, 
	-0.5f,  0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,

	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,

	0.5f,  0.5f,  0.5f, 
	0.5f, -0.5f, -0.5f, 
	0.5f,  0.5f, -0.5f, 
	0.5f, -0.5f, -0.5f, 
	0.5f,  0.5f,  0.5f, 
	0.5f, -0.5f,  0.5f, 

	-0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f, 
	0.5f, -0.5f,  0.5f, 
	0.5f, -0.5f,  0.5f, 
	-0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f, -0.5f,

	-0.5f,  0.5f, -0.5f,
	0.5f,  0.5f,  0.5f, 
	0.5f,  0.5f, -0.5f, 
	0.5f,  0.5f,  0.5f, 
	-0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f,  0.5f
};

Cube::Cube(int details) {

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Cube::draw(Shader *shader) {
    shader->use();

    glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}