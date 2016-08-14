#version 330 core

uniform mat4 modelViewProjectionMatrix;
uniform mat4 textureMatrix;

in vec4 position;
in vec2 texcoord;

out vec2 texCoordVarying;

void main(){
    
    texCoordVarying = texcoord;
    gl_Position = modelViewProjectionMatrix * position;
}
