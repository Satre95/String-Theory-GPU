#pragma once

#include "ofMain.h"
#include "ofxFX.h"
#include "ofxGaussianBlur.h"
#include "ofxLUT.h"


class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    ofVec3f randomPointOnSphere();
    ofxFXObject particleData;
    
    ofxSwapBuffer swapBuffer;
    ofVboMesh particlePoints;
    
    ofEasyCam camera;
    
    
    //should be square number.
    int numberOfParticles = 10000;
    int maxParticleAge = 50;
};
