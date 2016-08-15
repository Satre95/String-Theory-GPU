#version 400 core

uniform mat4 MVP;

in vec4 particle;
in vec2 texcoord;

out vec2 texCoordVarying;

void main(){
    
    texCoordVarying = texcoord;
    gl_Position = MVP * particle;
}
