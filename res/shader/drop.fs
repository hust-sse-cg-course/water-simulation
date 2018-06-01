#version 330 core
in vec2 Coords;
out vec4 FragColor;

const float PI = 3.141592653589793;

uniform sampler2D texture;
uniform vec2 center;
uniform float radius;
uniform float strength;

void main() {
    vec4 info = texture2D(texture, Coords);

    float drop = max(0.0, 1.0 - length(center * 0.5 + 0.5 - Coords) / radius);
    drop = 0.5 - cos(drop * PI) * 0.5;
    info.r += drop * strength;

    FragColor = info;
}