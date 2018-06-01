#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoords;
layout(location = 2) in vec3 aNormals;

out vec2 Coords;

void main() {
    Coords = aPos.xz * 0.5 + 0.5;
    vec3 position = aPos.xzy;
    gl_Position = vec4(position, 1.0f);
}