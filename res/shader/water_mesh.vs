#version 330 core
layout(location = 0) in vec3 aPos;

out vec3 position;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform sampler2D water;

out vec4 temp;
void main() {
    position = aPos;
    position.y += texture2D(water, aPos.xz * 0.5 + 0.5).r;
    gl_Position = projection * view * model * vec4(position, 1.0f);
    //gl_Position = vec4(aPos.xzy, 1.0f);
}