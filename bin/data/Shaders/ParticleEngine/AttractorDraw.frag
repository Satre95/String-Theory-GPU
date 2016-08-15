#version 400 core

uniform sampler2D tex0;

in vec2 sampleCoord;

out vec4 fragColor;

void main() {
    fragColor = texture(tex0, sampleCoord);
}