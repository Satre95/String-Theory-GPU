#version 400 core

uniform mat4 MVP;

in vec3 emitter;
in vec2 texcoord;

out vec2 sampleCoord;
void main() {
    
    sampleCoord = texcoord;
    gl_Position = vec4(emitter, 1.0f);
}