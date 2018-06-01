#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoords;
layout(location = 2) in vec3 aNormals;

out VS_OUT {
    vec3 normal;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform sampler2D info_texture;

void main() {
    vec4 info = texture2D(info_texture, aPos.xz * 0.5 + 0.5);
    // Debug: normal visualization
    vec3 n = vec3(info.b, sqrt(1 - info.b * info.b - info.a * info.a), info.a);
    mat3 normal_matrix = mat3(transpose(inverse(view * model)));
    vs_out.normal = normalize(vec3(projection * vec4(normal_matrix * n, 0.0f)));
    /////////////////////////////////
    vec3 position = aPos;
    position.y += info.r;
    gl_Position = projection * view * model * vec4(position, 1.0f);
}