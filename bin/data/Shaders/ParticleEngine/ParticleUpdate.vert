#version 400 core

#pragma include "Shaders/Utils/ShaderHelpers.glslinc"

uniform mat4 modelViewProjectionMatrix;

in vec4 position;
in vec2 texcoord;

out vec2 sampleCoord;

void main() {
    sampleCoord = texcoord;
    gl_Position = modelViewProjectionMatrix * position;
}