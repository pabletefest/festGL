#version 460

layout(location = 0) in vec4 aPosition;
layout(location = 1) in vec4 aColor;

out vec4 vColor;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main() {
    gl_Position = uProjection * uView * uModel * aPosition;
    vColor = aColor;
}
