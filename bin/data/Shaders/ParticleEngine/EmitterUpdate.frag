#version 400 core

#pragma include "Shaders/Utils/ShaderHelpers.glslinc"
#pragma include "Shaders/Utils/Noise3D.glslinc"

uniform sampler2D emitterPos;

in vec2 sampleCoord;

out vec4 newPosition;

void main() {
    newPosition = vec4(1.0f, 0, 0, 1.0f);
}