#version 400 core

uniform mat4 MVP;

in vec4 position;
in vec2 texcoord;

out vec2 sampleCoord;

void main() {
    sampleCoord = texcoord;
    gl_Position = MVP * position;
}