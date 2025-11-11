#version 460

out vec4 FragColor;

in vec2 texCoord;

layout (binding = 0) uniform sampler2D uWallTexture;

void main() {
    FragColor = texture(uWallTexture, texCoord);
}
