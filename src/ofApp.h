#pragma once

#include "ofMain.h"
#include "ofxFX.h"


class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    ofVec3f randomPointOnSphere();
    
    ofxSwapBuffer agentsPingPongBuffer;
    ofxSwapBuffer emittersPingPongBuffer;
    ofxSwapBuffer attractorsPingPongBuffer;
    
    ofVboMesh particlePoints;
    ofShader agentUpdateShader, emittersUpdateShader, attractorsUpdateShader;
    ofShader drawShader;
    ofEasyCam camera;
    
    float noiseScale, noiseStrength;
    
    //should be square number.
    int numberOfParticles = 10000;
    int numberOfAttractors = 100;
    int numberOfEmitters = 100;
    
    int maxParticleAge = 50;
    
private:
    
    void setupAgentsPingPongBuffer(int agentsTexSize);
    void setupEmittersPingPongBuffer(int emittersTexSize);
    void setupAttractorsPingPongBuffer(int attractorsTexSize);
    
    void initParticleData(int agentsTexSize);
    
    void updateEmitters();
    void updateAttractors();
    void updateAgents();
};
