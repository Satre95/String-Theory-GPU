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
    
    //NOTE: For debugging, in order to see the values on screen, data values need to normalized, otherwise OpenGL will always clamp them.
    //set new data
    newData = vec4(0);
//    newData.r = position.x;
//    newData.g = position.y;
//    newData.b = speed;
//    newData.a = 1.0f;
    
    //DEBUG:
//    newData.r = map(position.x, 0, screenWidth, 0, 1.0f);
//    newData.g = map(position.y, 0, screenHeight, 0, 1.0f);
//    newData.b = map(speed, 0, maxSpeed, 0, 1.0f);
//    newData.a = 1.0f;
    newData = vec4(vec3(angle), 1.0f);
}


float noiseAngleFromPosition(vec2 position) {
    vec2 noisePosition = vec2(position.x / noiseScale, position.y / noiseScale);
    
    return snoise(noisePosition) * noiseStrength;
}

