#pragma once

#include "ofMain.h"
#include "ofxFX.h"
#include "ofxAutoReloadedShader.h"
#include "ofxDatGui.h"

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    
    ofxDatGui * gui;
    ofxDatGuiSlider * noiseStrengthSlider;
    ofxDatGuiSlider * noiseScaleSlider;
    ofxDatGuiLabel * framerateLabel;
    
    ofxSwapBuffer agentsPingPongBuffer;
    ofxSwapBuffer emittersPingPongBuffer;
    ofxSwapBuffer attractorsPingPongBuffer;
    
    ofVboMesh particlePoints;
    ofVboMesh emitterPoints;
    ofVboMesh attractorPoints;
    
    ofxAutoReloadedShader agentUpdateShader, emittersUpdateShader, attractorsUpdateShader;
    ofxAutoReloadedShader attractorsDrawShader, emittersDrawShader, agentsDrawShader;
    
    ofEasyCam camera;
    
    float noiseScale, noiseStrength;
    
    //should be square number.
    int numberOfParticles = 10000;
    int numberOfAttractors = 800000;
    int numberOfEmitters = 100;
    int agentsTexSize, attractorsTexSize, emittersTexSize;
    
    int maxParticleAge = 50;
    float maxAttractorSpeed = 2.7f;
    int screenDepth;
    
    void noiseChanged(ofxDatGuiSliderEvent slider);
    
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
