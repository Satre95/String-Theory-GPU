#version 400 core

#pragma include "Shaders/Utils/ShaderHelpers.glslinc"
#pragma include "Shaders/Utils/Noise3D.glslinc"

//The source texture that has the emitter data from last frame.
//vec4 holds position and speed
uniform sampler2D emitterData;

uniform float noiseScale;
uniform float noiseStrength;
uniform int screenWidth;
uniform int screenHeight;
uniform int screenDepth;
uniform float maxSpeed;

in vec2 sampleCoord;

out vec4 newData;

/** declare functions **/
//Calculate the noise angle based off the current position.
float noiseAngleFromPosition(vec3 position);

void main() {
    vec4 posAndSpeed = texture(emitterData, sampleCoord);
    float speed = posAndSpeed.a;
    
    vec3 position = posAndSpeed.xyz;
    float angle = noiseAngleFromPosition(position);
    
    //Calc new position of emitter.
    position.x += cos(angle) * speed;
    position.y += sin(angle) * speed;
    position.z += sin(angle) * cos(angle) * speed * 2.0f;
    
    //Out of bounds check.
    bool outOfBoundsX = position.x > screenWidth || position.x < 0;
    bool outOfBoundsY = position.y > screenHeight || position.y < 0;
    bool outOfBoundsZ = position.z > screenDepth || position.z < 0;
    
    if ( outOfBoundsX || outOfBoundsY || outOfBoundsZ ) {
        position.xyz = vec3(0);
    }
    
    //set new data
    newData = vec4(0);
    newData.r = position.x;
    newData.g = position.y;
    newData.b = position.z;
    newData.a = speed;
    
}


float noiseAngleFromPosition(vec3 position) {
    vec3 noisePosition = vec3(position.x / noiseScale, position.y / noiseScale, position.z / noiseScale);
    
    return snoise(noisePosition) * noiseStrength;
}

