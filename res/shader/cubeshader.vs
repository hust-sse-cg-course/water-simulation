#version 330 core
const float poolHeight = 1.0;

layout(location = 0) in vec3 aPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec4 FragPos;
out vec3 position;

void main() {
	position = aPos.xyz;
	position.y = ((1.0 - position.y) * (7.0 / 12.0) - 1.0) * poolHeight;
	FragPos = projection * view * model * vec4(position, 1.0);
}