#version 400 core

uniform mat4 MVP;

in vec4 position;
in vec2 texcoord;

out vec2 texCoordVarying;
void main() {
    texCoordVarying = texcoord
    gl_Position = vec4(1.0f);
}