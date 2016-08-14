#version 400 core

#pragma include "../Utils/ShaderHelpers.glslinc"
#pragma include "../Utils/Noise3D.glslinc"

uniform sampler2D emitterPos;

out vec4 newPosition;

void main() {
    newPosition = vec4(1.0f, 0, 0, 1.0f);
}