#version 330 core
layout(location = 0) in vec3 aPos;

const float poolHeight = 1.0;
out vec3 position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    position = aPos;
    position.y += ((1.0 - position.y) * (7.0 / 12.0) - 1.0) * poolHeight;;
    gl_Position = projection * view * model * vec4(position, 1.0f);
}