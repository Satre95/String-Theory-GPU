#pragma once

#include "ofMain.h"
#include "ofxFX.h"
#include "ofxAutoReloadedShader.h"

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    
    ofTrueTypeFont font;
    
    ofxSwapBuffer agentsPingPongBuffer;
    ofxSwapBuffer emittersPingPongBuffer;
    ofxSwapBuffer attractorsPingPongBuffer;
    
    ofVboMesh particlePoints;
    ofVboMesh emitterPoints;
    ofVboMesh attractorPoints;
    
    ofxAutoReloadedShader agentUpdateShader, emittersUpdateShader, attractorsUpdateShader;
    ofxAutoReloadedShader attractorsDrawShader, emittersDrawShader, agentsDrawShader;
    
    float noiseScale, noiseStrength;
    
    //should be square number.
    int numberOfParticles = 10000;
    int numberOfAttractors = 50000;
    int numberOfEmitters = 100;
    int agentsTexSize, attractorsTexSize, emittersTexSize;
    
    int maxParticleAge = 50;
    float maxAttractorSpeed = 2.7f;
    
private:
    
    void setupAgentsPingPongBuffer(int agentsTexSize);
    void setupEmittersPingPongBuffer(int emittersTexSize);
    void setupAttractorsPingPongBuffer(int attractorsTexSize);
    
    void initParticleData(int agentsTexSize);
    void initAttractorData(int attractorsTexSize);
    void initEmitterData( int emittersTexSize);
    
    void updateEmitters();
    void updateAttractors();
    void updateAgents();
    void updateCommonNoiseParams(ofShader &);
    
    void drawAttractorDebugData();
};
