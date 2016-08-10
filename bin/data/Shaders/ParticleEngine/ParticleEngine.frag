#version 410 core

#pragma include "../Utils/ShaderHelpers.glslinc"
#pragma include "../Utils/Noise4D.glslinc"

uniform sampler2D tex0;
uniform sampler2D tex1;

out vec4 fragColor;

void main() {

  fragColor = vec4(1.0, 1.0, 1.0, 1.0);
}
