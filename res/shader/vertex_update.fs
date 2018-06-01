#version 330 core
in vec2 Coords;
out vec4 FragColor;

uniform sampler2D texture;
uniform vec2 delta;

void main() {
    vec4 info = texture2D(texture, Coords);

    vec2 dx = vec2(delta.x, 0.0);
    vec2 dy = vec2(0.0, delta.y);
    float average = (
        texture2D(texture, Coords - dx).r +
        texture2D(texture, Coords - dy).r +
        texture2D(texture, Coords + dx).r +
        texture2D(texture, Coords + dy).r
    ) * 0.25;

    info.g += (average - info.r) * 2.0;
    info.g *= 0.995;
    info.r += info.g;

    FragColor = info;
}