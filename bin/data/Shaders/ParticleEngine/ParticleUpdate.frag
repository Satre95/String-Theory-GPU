#version 400 core

uniform sampler2D agentsPosAndAge;
uniform sampler2D emitterPositions;
uniform sampler2D attractorPositions;

out vec4 newPosAndAge;

void main(){
    
    newPosAndAge = vec4(0, 0, 1.0, 1.0);
}
