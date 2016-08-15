#version 400 core

#pragma include "Shaders/Utils/ShaderHelpers.glslinc"
#pragma include "Shaders/Utils/Noise2D.glslinc"

//The source texture that has the attractor data from last frame.
//vec4 holds position and speed
uniform sampler2D attractorData;

uniform float noiseScale;
uniform float noiseStrength;
uniform int screenWidth;
uniform int screenHeight;
uniform float maxSpeed;

in vec2 sampleCoord;

out vec4 newData;

/** declare functions **/
//Calculate the noise angle based off the current position.
float noiseAngleFromPosition(vec2 position);

void main() {
    vec4 posAndSpeed = texture(attractorData, sampleCoord);
    float speed = posAndSpeed.a;
    
    vec2 position = vec2(posAndSpeed.x, posAndSpeed.y);
    float angle = noiseAngleFromPosition(position);
    
    //Calc new position of attractor.
    position.x += cos(angle) * speed;
    position.y += sin(angle) * speed;
    
    //Out of bounds check.
    if (position.x > screenWidth || position.x < 0 || position.y > screenHeight || position.y < 0) {
        position.xy = vec2(0);
    }
    
    //set new data
    newData = vec4(0);
    newData.r = position.x;
    newData.g = position.y;
    newData.b = 0;
    newData.a = speed;
    
}


float noiseAngleFromPosition(vec2 position) {
    vec2 noisePosition = vec2(position.x / noiseScale, position.y / noiseScale);
    
    return snoise(noisePosition) * noiseStrength;
}

