#version 400 core
#pragma include "Shaders/Utils/ShaderHelpers.glslinc"


uniform sampler2D agentsPosData;
uniform int screenWidth;
uniform int screenHeight;
uniform int screenDepth;


in vec2 sampleCoord;

out vec4 fragColor;

void main() {
    vec4 data = texture(agentsPosData, sampleCoord);
    float r = map(data.x, 0, screenWidth, 0, 1.0f);
    float g = map(data.y, 0, screenHeight, 0, 1.0f);
    float b = map(data.z, 0, screenDepth, 0, 1.0f);
    float a = 0.5f;
    
    fragColor = vec4(r, g, b, a);
    
}