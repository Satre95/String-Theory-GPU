#version 400 core

uniform mat4 modelViewProjectionMatrix;
uniform sampler2D agentsPosData;
uniform int screenWidth;
uniform int screenHeight;

in vec4 position;
in vec2 texcoord;

out vec2 sampleCoord;

void main() {
    //Pass tex coords to frag shader
    sampleCoord = texcoord;
    
    //Determine vertex pos from data tex
    vec4 agentData = texture(agentsPosData, texcoord);
    
    vec3 agentPos = agentData.xyz;
    
    gl_Position = modelViewProjectionMatrix * vec4( agentPos.x, agentPos.y, agentPos.z, 1.0f);
}