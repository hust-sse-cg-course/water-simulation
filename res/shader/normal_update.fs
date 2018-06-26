#version 330 core
in vec2 Coords;
out vec4 FragColor;

uniform sampler2D texture;
uniform vec2 delta;

void main() {
    vec4 info = texture2D(texture, Coords);

    //vec3 dx = vec3(delta.x, texture2D(texture, vec2(Coords.x + delta.x, Coords.y)).r - info.r, 0.0);
    //vec3 dy = vec3(0.0, texture2D(texture, vec2(Coords.x, Coords.y + delta.y)).r - info.r, delta.y);
    //info.ba = normalize(cross(dy, dx)).xz;

    FragColor = info;
}