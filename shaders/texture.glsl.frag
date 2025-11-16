#version 460

out vec4 FragColor;

in vec2 texCoord;

layout (binding = 0) uniform sampler2D uWallTexture;

uniform vec4 uColor;

void main() {
    FragColor = texture(uWallTexture, texCoord) * uColor;
}
