#version 400 core

#pragma include "Shaders/Utils/ShaderHelpers.glslinc"

uniform mat4 modelViewProjectionMatrix;
uniform sampler2D tex0;
uniform int screenWidth;
uniform int screenHeight;

in vec4 position;
in vec2 texcoord;

out vec2 sampleCoord;

void main() {
    //Pass texture coordinates to fragment shader.
    sampleCoord = texcoord;
    
    //Determine the vertex position from the data texture itself.
    vec4 particleData = texture(tex0, texcoord);
    vec4 dataPos = vec4(particleData.x, particleData.y, 0, 1.0f);
    gl_Position = modelViewProjectionMatrix * dataPos;
}