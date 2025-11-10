#version 460

out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D uWallText;

void main() {
    FragColor = texture(uWallText, texCoord);
}
