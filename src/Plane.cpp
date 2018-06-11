#include "Plane.h"

using namespace std;
using namespace glm;

Plane::Plane() : Plane(1) {}

Plane::Plane(int details) {
    int detailX = details;
    int detailY = details;

    for (int y = 0; y <= detailY; y++) {
        float t = (float)y / detailY;
        for (int x = 0; x <= detailX; x++) {
            float s = (float)x / detailX;
            Vertex v;
            v.positions = vec3(2 * s - 1, 0, 2 * t - 1);
            v.coords = vec2(s, t);
            v.normals = vec3(0, 1, 0);
            vertices.push_back(v);
            if (x < detailX && y < detailY) {
                int i = x + y * (detailX + 1);
                indices.push_back(i);
                indices.push_back(i + detailX + 1);
                indices.push_back(i + 1);
                indices.push_back(i + detailX + 1);
                indices.push_back(i + detailX + 2);
                indices.push_back(i + 1);
            }
        }
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, coords));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normals));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Plane::draw(Shader *shader) {
    shader->use();
	glClear(GL_COLOR_BUFFER_BIT);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}