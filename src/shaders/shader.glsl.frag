#version 330 core

layout(location = 0) out vec4 FragColor;

uniform vec4 uColor;

void main() {
    FragColor = uColor;
}